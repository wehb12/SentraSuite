/*
  ==============================================================================

    EnvelopeGui.cpp
    Created: 4 May 2026 8:04:17pm
    Author:  Will Hinds

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EnvelopeGui.h"

//==============================================================================
EnvelopeGui::EnvelopeGui(juce::AudioProcessorValueTreeState& tree)
{
	setSize (200, 200);
	
	attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	attackSlider.setRange(0.1f, 5000.0f);
	attackSlider.setValue(500.0f);
	attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&attackSlider);

	decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	decaySlider.setRange(1.0f, 2000.0f);
	decaySlider.setValue(500.0f);
	decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&decaySlider);

	sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	sustainSlider.setRange(0.0f, 1.0f);
	sustainSlider.setValue(0.8f);
	sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&sustainSlider);

	releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	releaseSlider.setRange(0.1f, 5000.0f);
	releaseSlider.setValue(200.0f);
	releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&releaseSlider);
	
	attackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "ATTACKSLIDER", attackSlider);
	decayValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "DECAYSLIDER", decaySlider);
	sustainValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "SUSTAINSLIDER", sustainSlider);
	releaseValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "RELEASESLIDER", releaseSlider);
}

EnvelopeGui::~EnvelopeGui()
{
}

void EnvelopeGui::paint (juce::Graphics& g)
{
	//fancy stuff for the UI background etc
	juce::Rectangle<int> titleArea (0, 10, getWidth(), 20);
	
	g.fillAll (juce::Colours::black);
	g.setColour(juce::Colours::white);
	g.drawText("Envelope", titleArea, juce::Justification::centredTop);
	
	//static positioning for now due to time, make dynamic later
	g.drawText ("Att", 34, 150, 30, 20, juce::Justification::centredTop);
	g.drawText ("Dec", 69, 150, 30, 20, juce::Justification::centredTop);
	g.drawText ("Sus", 106, 150, 30, 20, juce::Justification::centredTop);
	g.drawText ("Rel", 142, 150, 30, 20, juce::Justification::centredTop);
	
	juce::Rectangle <float> area (25, 25, 150, 150);
	
	g.setColour(juce::Colours::yellow);
	g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void EnvelopeGui::resized()
{
	//draws the sliders...we use a rectangle object to dynamically size the UI (if we want to resize for IPad etc without needing to change ALL settings
	juce::Rectangle<int> area = getLocalBounds().withTrimmedTop(50).withTrimmedBottom(50).withTrimmedLeft(30).withTrimmedRight(30);
	
	int sliderWidth = 35;
	int sliderHeight = 175;
	
	//draw sliders by reducing area from rectangle above
	attackSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	decaySlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	sustainSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	releaseSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
}
