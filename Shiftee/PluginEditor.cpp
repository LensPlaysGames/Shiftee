#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShifteeProcessorEditor::ShifteeProcessorEditor (ShifteeProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
        gainAttachment(audioProcessor.apvts, "Gain", gainSlider),
        bitShifterOffsetAttachment(audioProcessor.apvts, "BitShifterOffset", bitShifterOffsetSlider)
{
    auto& defaultLookAndFeel = getLookAndFeel();
    // Slider colours
    defaultLookAndFeel.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    defaultLookAndFeel.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    defaultLookAndFeel.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::darkgrey);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setRange(-48.0, 48.0, 0.1);

    bitShifterOffsetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setRange(2, 32, 1);

    /* Make all components a child to the current editor (effectively showing them) */
    for (auto& component : getComponents())
        addAndMakeVisible(component);
    
    /* Size in pixels of the plugin */
    setSize (360, 120);
}

ShifteeProcessorEditor::~ShifteeProcessorEditor()
{
}

//==============================================================================
void ShifteeProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void ShifteeProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    /* Save space on the top of the plugin for a title (test) */
    auto contentBounds = bounds.removeFromBottom(bounds.getHeight() * 0.9);
    /* Save space on the right side to draw the labels in the paint function */
    auto labelBounds = contentBounds.removeFromRight(contentBounds.getWidth() * 0.18);

    /* Place gain slider in top half of content bounds */
    gainSlider.setBounds(contentBounds.removeFromTop(contentBounds.getHeight() * 0.5));
    bitShifterOffsetSlider.setBounds(contentBounds);
}
