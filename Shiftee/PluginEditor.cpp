#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShifteeProcessorEditor::ShifteeProcessorEditor (ShifteeProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    juce::ignoreUnused (audioProcessor);
    setSize (400, 300);
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
}
