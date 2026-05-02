//
//  WavetableSynth.cpp
//  WavetableSynth
//
//  Created by Will Hinds on 24/04/2026.
//

#include "WavetableSynth.h"
#include "SquareWaveOscillator.h"
#include "SineWaveOscillator.h"

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

void WavetableSynth::initialiseOsciallators(double inSampleRate, int samplesPerBlock)
{
	constexpr auto OSCILLATORS_COUNT = 128;
	
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
	
	filter.processBlock(buffer, midiMessages);
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
	float* firstChannel = buffer.getWritePointer(0);
	
	for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
	{
		std::shared_ptr<WavetableOscillatorBase> osc = oscillators[oscillatorId];
		maxiEnv& env = envelopes[oscillatorId];
		
		env.setAttack(500);
		env.setDecay(500);
		env.setSustain(0.8);
		env.setRelease(200);
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
			filter.setCutoffFrequency(std::min(filter.getCutoffFrequency() + 2.0f, 100.0f));
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
		oscillators[oscillatorId]->setFrequency(frequency);
		envelopes[oscillatorId].trigger = 1;
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
