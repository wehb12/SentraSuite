/*
  ==============================================================================

    EnvelopeGui.h
    Created: 4 May 2026 8:04:17pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>

//==============================================================================
/*
*/
class EnvelopeGui  : public juce::Component
{
public:
    EnvelopeGui(juce::AudioProcessorValueTreeState& tree);
    ~EnvelopeGui() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	juce::Slider attackSlider;
	juce::Slider decaySlider;
	juce::Slider sustainSlider;
	juce::Slider releaseSlider;
	
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainValue;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseValue;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeGui)
};
