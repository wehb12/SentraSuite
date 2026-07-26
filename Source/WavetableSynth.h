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
	SawWave,
	
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
	
	template<class OscType> void setOscillator()
	{
		envelopes.clear();
		envelopes.reserve(OSCILLATORS_COUNT);
		oscillators.clear();
		oscillators.reserve(OSCILLATORS_COUNT);
		for (int i = 0; i < OSCILLATORS_COUNT; ++i)
		{
			oscillators.emplace_back(std::make_shared<OscType>());
			oscillators.back()->init(sampleRate);
			envelopes.emplace_back();
		}
	}
	
	template<class OscType> void setSecondaryOscillator()
	{
		secondaryEnvelopes.clear();
		secondaryEnvelopes.reserve(OSCILLATORS_COUNT);
		secondaryOscillators.clear();
		secondaryOscillators.reserve(OSCILLATORS_COUNT);
		for (int i = 0; i < OSCILLATORS_COUNT; ++i)
		{
			secondaryOscillators.emplace_back(std::make_shared<OscType>());
			secondaryOscillators.back()->init(sampleRate);
			secondaryEnvelopes.emplace_back();
		}
	}
	
	void setOscillators();
	void buildUnisonOscillators(bool forceResetOscs = false);
	maxiEnv& getEnvelope(int oscillatorId);
	maxiEnv& getEnvelope2(int oscillatorId);
	
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
	void doFilter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
	void handleMidiEvent(const juce::MidiMessage& midiMessage);
	float midiNoteNumberTofrequency(int midiNoteNumber, float semitoneChange = 0.0f, float detuneAmount = 0.0f)
	{
		return 440.0f * std::powf(2.0f, (midiNoteNumber + semitoneChange + (detuneAmount / 100.0f) - 69.0f) / 12.0f);
	}
	
	juce::AudioProcessorValueTreeState& tree;
	
	bool isPlaying = false;
	
	double sampleRate;
	std::vector<std::shared_ptr<WavetableOscillatorBase>> oscillators;
	std::vector<std::shared_ptr<WavetableOscillatorBase>> secondaryOscillators;
	std::vector<maxiEnv> envelopes;
	std::vector<maxiEnv> secondaryEnvelopes;
	
	std::vector<std::shared_ptr<WavetableOscillatorBase>> unisonOscillators;
	float unisonNumVoices = 1.0f;
	float unisonDetuneAmount = 0.0f;
	float unisonStereoAmount = 0.0f;
	
	Filter filter;
	
	WavetableType oscType = SquareWave;
	WavetableType secondOscType = SawWave;
	float secondOscAmount = 0.0f;
	float oscSemitoneAmount = 0.0f;
	float secondOscSemitoneAmount = 0.0f;
	
	static constexpr auto OSCILLATORS_COUNT = 128;
};
