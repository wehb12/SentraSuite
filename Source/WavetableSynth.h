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

enum WavetableType
{
	SineWave = 0,
	SquareWave,
	
	WavetableType_END
};

class WavetableSynth
{
public:
	WavetableSynth(juce::AudioProcessorValueTreeState& inTree);
	
	void prepareToPlay (double sampleRate, int samplesPerBlock);
	void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);
	void reset();
	
private:
	void initialiseOsciallators(double inSampleRate, int samplesPerBlock);
	void setOscillators();
	maxiEnv& getEnvelope(int oscillatorId);
	
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
	void doFilter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
	void handleMidiEvent(const juce::MidiMessage& midiMessage);
	float midiNoteNumberTofrequency(int midiNoteNumber) { return 440.0f * std::powf(2.0f, (midiNoteNumber - 69.0f) / 12.0f); }
	
	juce::AudioProcessorValueTreeState& tree;
	
	bool isPlaying = false;
	
	double sampleRate;
	std::vector<std::shared_ptr<WavetableOscillatorBase>> oscillators;
	std::vector<maxiEnv> envelopes;
	
	Filter filter;
	
	WavetableType oscType = SineWave;
};
