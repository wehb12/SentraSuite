/*
  ==============================================================================

    FilterGui.cpp
    Created: 4 May 2026 8:57:52pm
    Author:  Will Hinds

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterGui.h"

//==============================================================================
FilterGui::FilterGui(juce::AudioProcessorValueTreeState& tree)
{
	setSize (200, 200);
	
	cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	cutoffSlider.setRange(20.0f, 20000.0f);
	cutoffSlider.setValue(200.0f);
	cutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	cutoffSlider.setSkewFactor(0.2f);
	addAndMakeVisible(&cutoffSlider);

	resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	resonanceSlider.setRange(0.5f, 5.0f);
	resonanceSlider.setValue(1.0f);
	resonanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&resonanceSlider);
	
	filterTypeComboBox.setJustificationType(juce::Justification::centred);
	filterTypeComboBox.addItem("Low Pass", 1);
	filterTypeComboBox.addItem("High Pass", 2);
	filterTypeComboBox.addItem("Band Pass", 3);
	filterTypeComboBox.setSelectedId(1);
	addAndMakeVisible(&filterTypeComboBox);
	
	openFilterSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	openFilterSpeedSlider.setRange(1.0f, 20.0f);
	openFilterSpeedSlider.setValue(1.0f);
	openFilterSpeedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&openFilterSpeedSlider);
	
	openFilterButton.setButtonText("Wahh");
	addAndMakeVisible(openFilterButton);

	keyTrackButton.setButtonText("KeyTrk");
	addAndMakeVisible(keyTrackButton);
	
	cutoffValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "CUTOFFSLIDER", cutoffSlider);
	resonanceValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "RESONANCESLIDER", resonanceSlider);
	filterTypeComboBoxChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "FILTERTYPECOMBOBOX", filterTypeComboBox);
	openFilterButtonState = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(tree, "OPENFILTERSTATE", openFilterButton);
	openFilterSpeedValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "OPENFILTERSPEEDSLIDER", openFilterSpeedSlider);
	keyTrackButtonState = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(tree, "KEYTRACKSTATE", keyTrackButton);
}

FilterGui::~FilterGui()
{
}

void FilterGui::paint (juce::Graphics& g)
{
	juce::Rectangle<int> titleArea (0, 10, getWidth(), 20);
	
	g.fillAll (juce::Colours::black);
	g.setColour(juce::Colours::white);
	g.drawText("Filter", titleArea, juce::Justification::centredTop);
	
	juce::Rectangle <float> area (25, 25, 150, 150);
	
	g.setColour(juce::Colours::yellow);
	g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void FilterGui::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(35);
	
	filterTypeComboBox.setBounds(area.removeFromTop(20));
	
	area.removeFromTop(5);
	juce::Rectangle<int> buttons = area.removeFromTop(20);
	openFilterButton.setBounds(buttons.removeFromLeft(65));
	keyTrackButton.setBounds(buttons);
	
	openFilterSpeedSlider.setBounds(area.removeFromTop(20));
	
	area.expand(10, 10);
	cutoffSlider.setBounds(area.removeFromLeft(75));
	resonanceSlider.setBounds(area);
}
