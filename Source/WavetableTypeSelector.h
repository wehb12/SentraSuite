/*
  ==============================================================================

    WavetableTypeSelector.h
    Created: 3 May 2026 10:03:25pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>

//==============================================================================
/*
*/
class WavetableTypeSelector  : public juce::Component
{
public:
    WavetableTypeSelector(juce::AudioProcessorValueTreeState& tree);
    ~WavetableTypeSelector() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	void addOscsToComboBox(juce::ComboBox& comboBox);
	
	juce::ComboBox typeComboBox;
	juce::ToggleButton secondOscButton;
	juce::Slider secondOscAmount;
	juce::ComboBox secondOscTypeComboBox;
	
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeComboBoxChoice;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> secondOscButtonState;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> secondOscAmountValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> secondOscTypeComboBoxChoice;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableTypeSelector)
};
