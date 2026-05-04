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
	juce::ComboBox typeComboBox;
	
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeComboBoxChoice;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableTypeSelector)
};
