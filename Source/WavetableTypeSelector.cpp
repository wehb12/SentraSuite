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
	
	oscSemitoneAmount.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	oscSemitoneAmount.setRange(-48.0f, 48.0f, 1.0f);
	oscSemitoneAmount.setValue(0.0f);
	oscSemitoneAmount.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&oscSemitoneAmount);
	
	secondOscAmount.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	secondOscAmount.setRange(0.0f, 1.0f);
	secondOscAmount.setValue(0.0f);
	secondOscAmount.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&secondOscAmount);
	
	addOscsToComboBox(secondOscTypeComboBox);
	
	secondOscSemitoneAmount.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
	secondOscSemitoneAmount.setRange(-48.0f, 48.0f, 1.0f);
	secondOscSemitoneAmount.setValue(0.0f);
	secondOscSemitoneAmount.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
	addAndMakeVisible(&secondOscSemitoneAmount);
	
	typeComboBoxChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "TYPECOMBOBOX", typeComboBox);
	secondOscAmountValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "SECONDOSCAMOUNT", secondOscAmount);
	secondOscTypeComboBoxChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "SECONDOSCTYPECOMBOBOX", secondOscTypeComboBox);
	oscSemitoneValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "OSCSEMITONEAMOUNT", oscSemitoneAmount);
	secondOscSemitoneValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "SECONDOSCSEMITONEAMOUNT", secondOscSemitoneAmount);
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
	secondOscTypeComboBox.setBounds(area.removeFromBottom(20));
	secondOscAmount.setBounds(area.removeFromLeft(65));
	oscSemitoneAmount.setBounds(area.removeFromTop(30));
	secondOscSemitoneAmount.setBounds(area.removeFromBottom(30));
}
