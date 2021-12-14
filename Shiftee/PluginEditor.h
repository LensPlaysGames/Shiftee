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
    ShifteeProcessor& audioProcessor;

    /* Components */
    juce::Slider gainSlider;
    juce::Slider bitShifterOffsetSlider;

    /* APVTS attachments - must be initialized in constructor */
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment, bitShifterOffsetAttachment;

    /* Return all components to be drawn within the editor */
    std::vector<juce::Component*> getComponents() { 
        return {
            &gainSlider,
            &bitShifterOffsetSlider 
        };
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShifteeProcessorEditor)
};
