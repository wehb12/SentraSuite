//
//  WavetableSynth.h
//  WavetableSynth
//
//  Created by Will Hinds on 24/04/2026.
//

#pragma once

#include <JuceHeader.h>
#include "WavetableOscillator.h"
#include "maximilian.h"
#include "juce_dsp/processors/juce_StateVariableTPTFilter.h"

//template<> class juce::dsp::StateVariableTPTFilter<float>;

class WavetableSynth
{
public:
	void prepareToPlay (double sampleRate, int samplesPerBlock);
	void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);
	void reset();
	
//	void setCutoffFrequency(float inFreq) {cutoffFrequency = inFreq;}
	
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
	std::vector<maxiFilter> filters;
	std::vector<float> cutoffFrequencies;
	std::vector<juce::dsp::StateVariableTPTFilter<float>> dspFilters;
};
