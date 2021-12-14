#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShifteeProcessorEditor::ShifteeProcessorEditor (ShifteeProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
        gainAttachment(audioProcessor.apvts, "Gain", gainSlider),
        bitShifterOffsetAttachment(audioProcessor.apvts, "BitShifterOffset", bitShifterOffsetSlider)
{
    /* Set slider colours */
    auto& defaultLookAndFeel = getLookAndFeel();
    defaultLookAndFeel.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    defaultLookAndFeel.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    defaultLookAndFeel.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::darkgrey);

    /* Setup Components */
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 20);
    gainSlider.setRange(-48.0, 48.0, 0.1);

    bitShifterOffsetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    bitShifterOffsetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 20);
    bitShifterOffsetSlider.setRange(0, 32, 1);

    /* Make all components a child to the current editor, showing them */
    for (auto& component : getComponents())
        addAndMakeVisible(component);
    
    /* Size in pixels of the plugin */
    setSize (360, 120);
}

ShifteeProcessorEditor::~ShifteeProcessorEditor() {}

//==============================================================================
/* Draw labels in the label bounds section of content bounds */
void ShifteeProcessorEditor::paint (juce::Graphics& g)
{
    /* Default background color */
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    /* Where the bulk of the plugins content will be drawn */
    auto contentBounds = getLocalBounds();
    /* Save space on the right side of the content bounds to draw labels in the paint function */
    auto labelBounds = contentBounds.removeFromRight(contentBounds.getWidth() * 0.18);

    g.setColour(juce::Colours::white);
    g.setFont(18);
    /* Put gain label centred in top half of label bounds */
    g.drawFittedText("Gain", labelBounds.removeFromTop(labelBounds.getHeight() * 0.5), juce::Justification::centred, 1);
    /* Put bit offset label centred in bottom half of label bounds */
    g.drawFittedText("Bit Offset", labelBounds, juce::Justification::centred, 2);
}

/* Place components within content bounds */
void ShifteeProcessorEditor::resized()
{
    /* Where the bulk of the plugins content will be drawn */
    auto contentBounds = getLocalBounds();
    /* Save space on the right side of the content bounds to draw labels in the paint function */
    auto labelBounds = contentBounds.removeFromRight(contentBounds.getWidth() * 0.18);

    /* Place gain slider in top half of content bounds */
    gainSlider.setBounds(contentBounds.removeFromTop(contentBounds.getHeight() * 0.5));
    /* Put bit offset slider in bottom half of content bounds */
    bitShifterOffsetSlider.setBounds(contentBounds);
}
