//
//  WavetableSynth.cpp
//  WavetableSynth
//
//  Created by Will Hinds on 24/04/2026.
//

#include "WavetableSynth.h"

void WavetableSynth::prepareToPlay (double inSampleRate, int samplesPerBlock)
{
	sampleRate = inSampleRate;
	
	initialiseOsciallators( inSampleRate,  samplesPerBlock);
	
	reset();
}

void WavetableSynth::reset()
{
	for (juce::dsp::StateVariableTPTFilter<float>& filter : dspFilters) filter.reset();
}

void WavetableSynth::initialiseOsciallators(double inSampleRate, int samplesPerBlock)
{
	constexpr auto OSCILLATORS_COUNT = 128;
	
//	const WaveTable waveTable = generateSineWaveTable();
	const WaveTable waveTable = generateSquareWaveTable();
	
	oscillators.clear();
	oscillators.reserve(OSCILLATORS_COUNT);
	for (int i = 0; i < OSCILLATORS_COUNT; ++i)
	{
		oscillators.emplace_back(waveTable, sampleRate);
		envelopes.emplace_back();
		filters.emplace_back();
		cutoffFrequencies.emplace_back();
		dspFilters.emplace_back();
		juce::dsp::ProcessSpec spec;
		spec.maximumBlockSize = samplesPerBlock;
		spec.sampleRate = sampleRate;
		spec.numChannels = 2;
		dspFilters.back().prepare(spec);
		dspFilters.back().setType(juce::dsp::StateVariableTPTFilterType::lowpass);
	}
}

const WaveTable WavetableSynth::generateSineWaveTable()
{
	constexpr int WAVETABLE_LENGTH = 64;
	WaveTable sineWaveTable(WAVETABLE_LENGTH);
	
	for (int i = 0; i < WAVETABLE_LENGTH; ++i)
	{
		sineWaveTable[i] = std::sinf(juce::MathConstants<float>::twoPi * static_cast<float>(i) / static_cast<float>(WAVETABLE_LENGTH));
	}
	
	return sineWaveTable;
}

const WaveTable WavetableSynth::generateSquareWaveTable()
{
	constexpr int WAVETABLE_LENGTH = 64;
	WaveTable squareWaveTable(WAVETABLE_LENGTH);
	
	for (int i = 0; i < (WAVETABLE_LENGTH / 2); ++i)
	{
		squareWaveTable[i] = -1;
	}
	for (int i = (WAVETABLE_LENGTH / 2); i < WAVETABLE_LENGTH; ++i)
	{
		squareWaveTable[i] = 1;
	}
	
	return squareWaveTable;
}

//float cutoffFrequency = 0.0f;
bool noteOn = false;
void WavetableSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	int currentSample = 0;
	
	for (const juce::MidiMessageMetadata midiMessage : midiMessages)
	{
		const juce::MidiMessage midiEvent = midiMessage.getMessage();
		const int midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
		
		render(buffer, currentSample, midiEventSample);
		handleMidiEvent(midiEvent);
		
		currentSample = midiEventSample;
	}
	
	render(buffer, currentSample, buffer.getNumSamples());
	
//	for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
	{
		juce::dsp::StateVariableTPTFilter<float>& dspFilter = dspFilters.back();
		float& cutoffFreq = cutoffFrequencies.back();
		
		dspFilter.setCutoffFrequency(cutoffFreq);
		dspFilter.setResonance(3);
		juce::dsp::AudioBlock<float> block(buffer);
		juce::dsp::ProcessContextReplacing<float> context(block);
		dspFilter.process(context);
	}
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
	float* firstChannel = buffer.getWritePointer(0);
	
	for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
	{
		WavetableOscillator& osc = oscillators[oscillatorId];
		maxiEnv& env = envelopes[oscillatorId];
		maxiFilter& filter = filters[oscillatorId];
		juce::dsp::StateVariableTPTFilter<float>& dspFilter = dspFilters[oscillatorId];
		float& cutoffFreq = cutoffFrequencies.back();
		
		env.setAttack(500);
		env.setDecay(500);
		env.setSustain(0.8);
		env.setRelease(200);
		if (osc.isPlaying())
		{
			for (int sample = startSample; sample < endSample; ++sample)
			{
				const float sampleEnv = env.adsr(osc.getSample(), env.trigger) * 0.1f;
				
//				const float sampleFiltered = filter.lores(sampleEnv, cutoffFreq, 1.5);
				firstChannel[sample] += sampleEnv;
				
				if (env.trigger == 0 && (sampleEnv < 0.000001 && sampleEnv > -0.000001))
				{
					osc.stop();
				}
			}
		}
		if (env.trigger)
		{
			cutoffFreq = std::min(cutoffFreq + 2.0f, 100.0f);
		}
	}

	
	for(int channel = 1; channel < buffer.getNumChannels(); ++channel)
	{
		std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
	}
	
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn())
	{
		const int oscillatorId = midiEvent.getNoteNumber();
		const auto frequency = midiNoteNumberTofrequency(oscillatorId);
		oscillators[oscillatorId].setFrequency(frequency);
		envelopes[oscillatorId].trigger = 1;
//		filters[oscillatorId] = maxiFilter();
		cutoffFrequencies.back() = 40.0f;
	}
	else if (midiEvent.isNoteOff())
	{
		const int oscillatorId = midiEvent.getNoteNumber();
//		oscillators[oscillatorId].stop();
		envelopes[oscillatorId].trigger = 0;
	}
	else if (midiEvent.isAllNotesOff())
	{
		for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
		{
			oscillators[oscillatorId].stop();
			envelopes[oscillatorId].trigger = 0;
		}
	}
}
