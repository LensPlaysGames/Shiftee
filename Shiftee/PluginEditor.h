#pragma once

#include "PluginProcessor.h"

//==============================================================================
class ShifteeProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit ShifteeProcessorEditor (ShifteeProcessor&);
    ~ShifteeProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ShifteeProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShifteeProcessorEditor)
};
