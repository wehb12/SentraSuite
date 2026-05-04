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
	
	typeComboBox.setJustificationType(juce::Justification::centred);
	typeComboBox.addItem("Sine Wave", 1);
	typeComboBox.addItem("Square Wave", 2);
	typeComboBox.setSelectedId (2);
	addAndMakeVisible(&typeComboBox);
	
	typeComboBoxChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "TYPECOMBOBOX", typeComboBox);
}

WavetableTypeSelector::~WavetableTypeSelector()
{
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
	juce::Rectangle<int> area = getLocalBounds().reduced(40);
	
	typeComboBox.setBounds(area.removeFromTop(20));
}
