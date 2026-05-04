/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WavetableTypeSelector.h"
#include "EnvelopeGui.h"
#include "FilterGui.h"

//==============================================================================
/**
*/
class WavetableSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WavetableSynthAudioProcessorEditor (WavetableSynthAudioProcessor& processor);
    ~WavetableSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WavetableSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthAudioProcessorEditor)
	
	WavetableTypeSelector wavetableTypeSelector;
	EnvelopeGui envelopeGui;
	FilterGui filterGui;
};
