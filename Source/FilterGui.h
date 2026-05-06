/*
  ==============================================================================

    FilterGui.h
    Created: 4 May 2026 8:57:52pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>

//==============================================================================
/*
*/
class FilterGui  : public juce::Component
{
public:
    FilterGui(juce::AudioProcessorValueTreeState& tree);
    ~FilterGui() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	juce::Slider cutoffSlider;
	juce::Slider resonanceSlider;
	juce::ComboBox filterTypeComboBox;
	juce::ToggleButton openFilterButton;
	juce::Slider openFilterSpeedSlider;
	juce::ToggleButton keyTrackButton;
	
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeComboBoxChoice;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> openFilterButtonState;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> openFilterSpeedValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> keyTrackButtonState;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGui)
};
