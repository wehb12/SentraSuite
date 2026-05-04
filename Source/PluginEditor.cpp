/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAudioProcessorEditor::WavetableSynthAudioProcessorEditor (WavetableSynthAudioProcessor& p)
    : AudioProcessorEditor (&p)
	, audioProcessor (p)
	, wavetableTypeSelector(p.tree)
	, envelopeGui(p.tree)
{
    setSize (400, 200);
	addAndMakeVisible(wavetableTypeSelector);
	addAndMakeVisible(envelopeGui);
}

WavetableSynthAudioProcessorEditor::~WavetableSynthAudioProcessorEditor()
{
}

//==============================================================================
void WavetableSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void WavetableSynthAudioProcessorEditor::resized()
{
	juce::Rectangle<int> area = getLocalBounds();
	
	const int componentWidth = 200;
	const int componentHeight = 200;
	
	wavetableTypeSelector.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	envelopeGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
}
