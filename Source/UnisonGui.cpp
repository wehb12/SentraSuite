/*
  ==============================================================================

    UnisonGui.cpp
    Created: 5 Jun 2026 3:14:07pm
    Author:  Will Hinds

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UnisonGui.h"

//==============================================================================
UnisonGui::UnisonGui(juce::AudioProcessorValueTreeState& tree)
{
	setSize (300, 200);
	
	unisonNumVoices.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	unisonNumVoices.setRange(-48.0f, 48.0f, 1.0f);
	unisonNumVoices.setValue(0.0f);
	unisonNumVoices.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&unisonNumVoices);
	
	unisonDetuneAmount.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	unisonDetuneAmount.setRange(-48.0f, 48.0f, 1.0f);
	unisonDetuneAmount.setValue(0.0f);
	unisonDetuneAmount.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&unisonDetuneAmount);

	unisonStereoAmount.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	unisonStereoAmount.setRange(0.0f, 100.0f, 1.0f);
	unisonStereoAmount.setValue(25.0f);
	unisonStereoAmount.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&unisonStereoAmount);
	
	unisonNumVoices2ndOsc.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	unisonNumVoices2ndOsc.setRange(-48.0f, 48.0f, 1.0f);
	unisonNumVoices2ndOsc.setValue(0.0f);
	unisonNumVoices2ndOsc.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&unisonNumVoices2ndOsc);
	
	unisonDetuneAmount2ndOsc.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	unisonDetuneAmount2ndOsc.setRange(-48.0f, 48.0f, 1.0f);
	unisonDetuneAmount2ndOsc.setValue(0.0f);
	unisonDetuneAmount2ndOsc.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&unisonDetuneAmount2ndOsc);
	
	unisonStereoAmount2ndOsc.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	unisonStereoAmount2ndOsc.setRange(0.0f, 100.0f, 1.0f);
	unisonStereoAmount2ndOsc.setValue(25.0f);
	unisonStereoAmount2ndOsc.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&unisonStereoAmount2ndOsc);
	
	
	
	unisonNumVoicesValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, UNISON_NUM_VOICES, unisonNumVoices);
	unisonDetuneAmountValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, UNISON_DETUNE_AMOUNT, unisonDetuneAmount);
	unisonStereoAmountValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, UNISON_STEREO_AMOUNT, unisonStereoAmount);
	
	unisonNumVoicesValue2ndOsc = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, UNISON_NUM_VOICES_OSC2, unisonNumVoices2ndOsc);
	unisonDetuneAmountValue2ndOsc = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, UNISON_DETUNE_AMOUNT_OSC2, unisonDetuneAmount2ndOsc);
	unisonStereoAmountValue2ndOsc = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, UNISON_STEREO_AMOUNT_OSC2, unisonStereoAmount2ndOsc);
}

UnisonGui::~UnisonGui()
{
}

void UnisonGui::paint (juce::Graphics& g)
{
	juce::Rectangle<int> titleArea (0, 10, getWidth(), 20);
	
	g.fillAll (juce::Colours::black);
	g.setColour(juce::Colours::white);
	g.drawText("Unison", titleArea, juce::Justification::centredTop);
	
	juce::Rectangle <float> area (25, 25, 250, 150);
	
	g.setColour(juce::Colours::yellow);
	g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void UnisonGui::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(35);
	
	juce::Rectangle<int> area1stOsc = area.removeFromTop(30);
	const int widthThird = area1stOsc.getWidth() / 3;
	
	unisonNumVoices.setBounds(area1stOsc.removeFromLeft(widthThird));
	unisonDetuneAmount.setBounds(area1stOsc.removeFromLeft(widthThird));
	unisonStereoAmount.setBounds(area1stOsc);
	
	juce::Rectangle<int> area2ndOsc = area.removeFromBottom(30);
	
	unisonNumVoices2ndOsc.setBounds(area2ndOsc.removeFromLeft(widthThird));
	unisonDetuneAmount2ndOsc.setBounds(area2ndOsc.removeFromLeft(widthThird));
	unisonStereoAmount2ndOsc.setBounds(area2ndOsc);
}
