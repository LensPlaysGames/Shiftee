#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "BitShifter.h"

//==============================================================================
class ShifteeProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ShifteeProcessor();
    ~ShifteeProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

    //==============================================================================
    struct ChainSettings {
        float gain_dB { 0.0f };
        int bitShifterOffset { 0 };
    };

    ChainSettings getChainSettings();

    // ChainPositions is an enum that details the organization of processors within a mono chain.
    enum ChainPositions { Gain, BitShifter };
    // MonoChain is the audio processing chain that will be used to process each audio channel.
    using MonoChain = juce::dsp::ProcessorChain<juce::dsp::Gain<float>, juce::dsp::BitShifter>;

    void UpdateAll();

private:
    MonoChain leftChain, rightChain;
    void UpdateChainFromSettings(MonoChain& chain, ChainSettings& settings);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShifteeProcessor)
};
