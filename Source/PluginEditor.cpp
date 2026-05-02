/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAudioProcessorEditor::WavetableSynthAudioProcessorEditor (WavetableSynthAudioProcessor& p/*, juce::AudioProcessorValueTreeState& vts*/)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
	
	frequencySlider.setSliderStyle (juce::Slider::LinearBarVertical);
	// midiVolume.setRange (0.0, 127.0, 1.0);
	frequencySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
	frequencySlider.setPopupDisplayEnabled (true, false, this);
	frequencySlider.setTextValueSuffix (" Frequency");
	frequencySlider.setValue (50.0);
	// this function adds the slider to the editor
	// addAndMakeVisible (&midiVolume);
	// midiVolume.addListener(this);
	
	frequencySlider.onValueChange = [this]
	{
		if (audioProcessor.getCurrentSampleRate() > 0.0)
		{
			audioProcessor.setFrequency(frequencySlider.getValue());
			audioProcessor.updateAngleDelta();
		}
	};
	
	frequencySlider.setRange (50.0, 5000.0);
	frequencySlider.setSkewFactorFromMidPoint (500.0);
	frequencySlider.setValue(audioProcessor.getFrequency());
	addAndMakeVisible (&frequencySlider);
}

WavetableSynthAudioProcessorEditor::~WavetableSynthAudioProcessorEditor()
{
}

//==============================================================================
void WavetableSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Slower Wavetable Synth", getLocalBounds(), juce::Justification::centredTop, 1);
}

void WavetableSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	
	// sets the position and size of the slider with arguments (x, y, width, height)
	frequencySlider.setBounds (40, 30, 20, getHeight() - 60);
}
