//
//  WavetableSynth.h
//  WavetableSynth
//
//  Created by Will Hinds on 24/04/2026.
//

#pragma once

#include <JuceHeader.h>
#include "WavetableOscillatorBase.h"
#include "Maximilian/maximilian.h"
#include "Filter.h"

#include <memory>

class WavetableSynth
{
public:
	void prepareToPlay (double sampleRate, int samplesPerBlock);
	void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);
	void reset();
	
private:
	void initialiseOsciallators(double inSampleRate, int samplesPerBlock);
	
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
	void handleMidiEvent(const juce::MidiMessage& midiMessage);
	float midiNoteNumberTofrequency(int midiNoteNumber) { return 440.0f * std::powf(2.0f, (midiNoteNumber - 69.0f) / 12.0f); }
	
	double sampleRate;
	std::vector<std::shared_ptr<WavetableOscillatorBase>> oscillators;
	std::vector<maxiEnv> envelopes;
	
	Filter filter;
};
