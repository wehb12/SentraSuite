/*
  ==============================================================================

    UnisonGui.h
    Created: 5 Jun 2026 3:14:07pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UIKeyNames.h"
#include <memory>

//==============================================================================
/*
*/
class UnisonGui  : public juce::Component
{
public:
    UnisonGui(juce::AudioProcessorValueTreeState& tree);
    ~UnisonGui() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnisonGui)
	
	juce::Slider unisonNumVoices;
	juce::Slider unisonNumVoices2ndOsc;
	juce::Slider unisonDetuneAmount;
	juce::Slider unisonDetuneAmount2ndOsc;
	juce::Slider unisonStereoAmount;
	juce::Slider unisonStereoAmount2ndOsc;
	
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonNumVoicesValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonNumVoicesValue2ndOsc;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonDetuneAmountValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonDetuneAmountValue2ndOsc;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonStereoAmountValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonStereoAmountValue2ndOsc;
};
