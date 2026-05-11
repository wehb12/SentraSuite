/*
  ==============================================================================

    WavetableTypeSelector.cpp
    Created: 3 May 2026 10:03:25pm
    Author:  Will Hinds

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavetableTypeSelector.h"

//==============================================================================
WavetableTypeSelector::WavetableTypeSelector(juce::AudioProcessorValueTreeState& tree)
{
	setSize (200, 200);
	
	addOscsToComboBox(typeComboBox);
	
	secondOscButton.setButtonText("2nd Osc");
	addAndMakeVisible(secondOscButton);
	
	secondOscAmount.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	secondOscAmount.setRange(0.0f, 1.0f);
	secondOscAmount.setValue(0.0f);
	secondOscAmount.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	secondOscAmount.setSkewFactor(0.2f);
	addAndMakeVisible(&secondOscAmount);
	
	addOscsToComboBox(secondOscTypeComboBox);
	
	typeComboBoxChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "TYPECOMBOBOX", typeComboBox);
	secondOscButtonState = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(tree, "SECONDOSCBUTTONSTATE", secondOscButton);
	secondOscAmountValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "SECONDOSCAMOUNT", secondOscAmount);
	secondOscTypeComboBoxChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "SECONDOSCTYPECOMBOBOX", secondOscTypeComboBox);
}

WavetableTypeSelector::~WavetableTypeSelector()
{
}

void WavetableTypeSelector::addOscsToComboBox(juce::ComboBox& comboBox)
{
	comboBox.setJustificationType(juce::Justification::centred);
	comboBox.addItem("Sine Wave", 1);
	comboBox.addItem("Square Wave", 2);
	comboBox.addItem("Saw Wave", 3);
	comboBox.setSelectedId (1);
	addAndMakeVisible(&comboBox);
}

void WavetableTypeSelector::paint (juce::Graphics& g)
{
	juce::Rectangle<int> titleArea (0, 10, getWidth(), 20);
	
	g.fillAll (juce::Colours::black);
	g.setColour(juce::Colours::white);
	g.drawText("Oscillator One", titleArea, juce::Justification::centredTop);
	
	
	juce::Rectangle <float> area (25, 25, 150, 150);
	
	g.setColour(juce::Colours::yellow);
	g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void WavetableTypeSelector::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(35);
	
	typeComboBox.setBounds(area.removeFromTop(20));
	area.removeFromTop(5);
	secondOscButton.setBounds(area.removeFromTop(20));
	secondOscAmount.setBounds(area.removeFromTop(60));
	secondOscTypeComboBox.setBounds(area.removeFromTop(20));
}
