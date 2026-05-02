//
//  WavetableSynth.h
//  WavetableSynth
//
//  Created by Will Hinds on 24/04/2026.
//

#pragma once

#include <JuceHeader.h>
#include "WavetableOscillator.h"
#include "Maximilian/maximilian.h"
#include "Filter.h"

class WavetableSynth
{
public:
	void prepareToPlay (double sampleRate, int samplesPerBlock);
	void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);
	void reset();
	
private:
	void initialiseOsciallators(double inSampleRate, int samplesPerBlock);
	const WaveTable generateSineWaveTable();
	const WaveTable generateSquareWaveTable();
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
	void handleMidiEvent(const juce::MidiMessage& midiMessage);
	float midiNoteNumberTofrequency(int midiNoteNumber) { return 440.0f * std::powf(2.0f, (midiNoteNumber - 69.0f) / 12.0f); }
	
	double sampleRate;
	std::vector<WavetableOscillator> oscillators;
	std::vector<maxiEnv> envelopes;
	
	Filter filter;
};
