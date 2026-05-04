//
//  WavetableSynth.cpp
//  WavetableSynth
//
//  Created by Will Hinds on 24/04/2026.
//

#include "WavetableSynth.h"
#include "SquareWaveOscillator.h"
#include "SineWaveOscillator.h"


WavetableSynth::WavetableSynth(juce::AudioProcessorValueTreeState& inTree)
	: tree(inTree)
{
}

void WavetableSynth::prepareToPlay(double inSampleRate, int samplesPerBlock)
{
	sampleRate = inSampleRate;
	
	initialiseOsciallators(inSampleRate,  samplesPerBlock);
	
	reset();
	
	filter.prepareToPlay(inSampleRate, samplesPerBlock);
}

void WavetableSynth::reset()
{
	filter.reset();
}

constexpr auto OSCILLATORS_COUNT = 128;
void WavetableSynth::initialiseOsciallators(double inSampleRate, int samplesPerBlock)
{
	oscillators.clear();
	oscillators.reserve(OSCILLATORS_COUNT);
	for (int i = 0; i < OSCILLATORS_COUNT; ++i)
	{
		oscillators.emplace_back(std::make_shared<SquareWaveOscillator>());
		oscillators.back()->init(inSampleRate);
		envelopes.emplace_back();
	}
}

void WavetableSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	setOscillators();
	
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
	
	doFilter(buffer, midiMessages);
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
	float* firstChannel = buffer.getWritePointer(0);
	
	for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
	{
		std::shared_ptr<WavetableOscillatorBase> osc = oscillators[oscillatorId];
		maxiEnv& env = getEnvelope(oscillatorId);

		if (osc->isPlaying())
		{
			for (int sample = startSample; sample < endSample; ++sample)
			{
				const float sampleEnv = env.adsr(osc->getSample(), env.trigger) * 0.1f;
				
				firstChannel[sample] += sampleEnv;
				
				if (env.trigger == 0 && (sampleEnv < 0.000001 && sampleEnv > -0.000001))
				{
					osc->stop();
				}
			}
		}
		if (env.trigger)
		{
			filter.setCutoffFrequency(std::min(filter.getCutoffFrequency() + 6.0f, 400.0f));
		}
	}

	
	for(int channel = 1; channel < buffer.getNumChannels(); ++channel)
	{
		std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
	}
	
}

void WavetableSynth::doFilter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
//	filter.setCutoffFrequency(tree.getRawParameterValue("CUTOFFSLIDER")->load());
	filter.setResonance(tree.getRawParameterValue("RESONANCESLIDER")->load());
	filter.setFilterType(static_cast<FilterType>(static_cast<int>(tree.getRawParameterValue("FILTERTYPECOMBOBOX")->load())));
	filter.processBlock(buffer, midiMessages);
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn())
	{
		const int oscillatorId = midiEvent.getNoteNumber();
		const auto frequency = midiNoteNumberTofrequency(oscillatorId);
		oscillators[oscillatorId]->setFrequency(frequency);
		envelopes[oscillatorId].trigger = 1;
		
		filter.setCutoffFrequency(20.0f);
	}
	else if (midiEvent.isNoteOff())
	{
		const int oscillatorId = midiEvent.getNoteNumber();
		envelopes[oscillatorId].trigger = 0;
	}
	else if (midiEvent.isAllNotesOff())
	{
		for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
		{
			oscillators[oscillatorId]->stop();
			envelopes[oscillatorId].trigger = 0;
		}
	}
}

void WavetableSynth::setOscillators()
{
	const WavetableType newType = static_cast<WavetableType>(static_cast<int>(tree.getParameter("TYPECOMBOBOX")->getValue()));
	if (oscType != newType)
	{
		oscType = newType;
		switch(oscType)
		{
			case SineWave:
					oscillators.clear();
					oscillators.reserve(OSCILLATORS_COUNT);
					for (int i = 0; i < OSCILLATORS_COUNT; ++i)
					{
						oscillators.emplace_back(std::make_shared<SineWaveOscillator>());
						oscillators.back()->init(sampleRate);
						envelopes.emplace_back();
					}
				break;
			case SquareWave:
					oscillators.clear();
					oscillators.reserve(OSCILLATORS_COUNT);
					for (int i = 0; i < OSCILLATORS_COUNT; ++i)
					{
						oscillators.emplace_back(std::make_shared<SquareWaveOscillator>());
						oscillators.back()->init(sampleRate);
						envelopes.emplace_back();
					}
				break;
			default:
				break;
		}
	}
}

maxiEnv& WavetableSynth::getEnvelope(int oscillatorId)
{
	maxiEnv& env = envelopes[oscillatorId];
	
	env.setAttack(tree.getRawParameterValue("ATTACKSLIDER")->load());
	env.setDecay(tree.getRawParameterValue("DECAYSLIDER")->load());
	env.setSustain(tree.getRawParameterValue("SUSTAINSLIDER")->load());
	env.setRelease(tree.getRawParameterValue("RELEASESLIDER")->load());
	
	return env;
}
