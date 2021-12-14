#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShifteeProcessor::ShifteeProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

ShifteeProcessor::~ShifteeProcessor()
{
}

//==============================================================================
const juce::String ShifteeProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ShifteeProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ShifteeProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ShifteeProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ShifteeProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ShifteeProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ShifteeProcessor::getCurrentProgram()
{
    return 0;
}

void ShifteeProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }
const juce::String ShifteeProcessor::getProgramName (int index) { juce::ignoreUnused (index); return {}; }
void ShifteeProcessor::changeProgramName (int index, const juce::String& newName) { juce::ignoreUnused (index, newName); }

//==============================================================================
/* Called when the plugin is first opened; used to initialize the plugin */
void ShifteeProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /* Initialize DSP Chain */
    // Make and initialize processor specifications.
    juce::dsp::ProcessSpec spec {};
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    // Prepare processors within chains.
    leftChain.prepare(spec);
    rightChain.prepare(spec);
}

/* Called when the plugin is closing for good; used to deallocate memory where needed */
void ShifteeProcessor::releaseResources() {}

bool ShifteeProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

/* Process a single audio block; a group of floating point audio samples */
void ShifteeProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // MIDI data is unused in this plugin.
    juce::ignoreUnused (midiMessages);

    /* Denormals: 
    *   More commonly referred to as subnormal numbers are 
    *   Numbers so close to zero yet under the precision of a floating point number. 
    *   Turning these off basically just means reducing the complexity of floats for the CPU.
    */
    juce::ScopedNoDenormals noDenormals;

    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();

    // This is here to avoid people getting screaming feedback when they first compile a plugin.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    /* Update all processor chains from value tree. */
    UpdateAll();

    /* Process Audio */
    // Convert sample buffer to dsp audio block.
    juce::dsp::AudioBlock<float> block(buffer);
    // Separate left and right audio blocks.
    juce::dsp::AudioBlock<float> leftBlock = block.getSingleChannelBlock(0);
    juce::dsp::AudioBlock<float> rightBlock = block.getSingleChannelBlock(1);
    // Create contexts from block channels left and right, respectively.
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    // Process contexts through the corresponding processor chain.
    leftChain.process(leftContext);
    rightChain.process(rightContext);
}

//==============================================================================
bool ShifteeProcessor::hasEditor() const{ return true; }
juce::AudioProcessorEditor* ShifteeProcessor::createEditor() { return new ShifteeProcessorEditor (*this); }

//==============================================================================
/* Save/Load plugin parameters */
void ShifteeProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}
void ShifteeProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.replaceState(tree);
        UpdateAll();
    }
}

//==============================================================================
/* Return a new ChainSettings created from the parameters within the APVTS */
ShifteeProcessor::ChainSettings ShifteeProcessor::getChainSettings()
{
    ChainSettings settings;

    settings.gain_dB = apvts.getRawParameterValue("Gain")->load();
    settings.bitShifterOffset = apvts.getRawParameterValue("BitShifterOffset")->load();

    return settings;
}

/* Update all chains within processor from a new settings */
void ShifteeProcessor::UpdateAll()
{
    ChainSettings settings = getChainSettings();
    UpdateChainFromSettings(leftChain, settings);
    UpdateChainFromSettings(rightChain, settings);
}

/* Update a single processor chain from a struct of settings */
void ShifteeProcessor::UpdateChainFromSettings(MonoChain& chain, ChainSettings& settings)
{
    chain.get<ChainPositions::Gain>().setGainDecibels(settings.gain_dB);
    chain.get<ChainPositions::BitShifter>().setBitOffset(settings.bitShifterOffset);
}

/* Create parameter layout within APVTS */
juce::AudioProcessorValueTreeState::ParameterLayout ShifteeProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("Gain", "Gain", -48.0f, 48.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterInt>("BitShifterOffset", "Bit Offset", 0, 32, 0));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ShifteeProcessor(); }