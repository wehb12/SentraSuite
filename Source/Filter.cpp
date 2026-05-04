/*
 ==============================================================================
 
 LowpassHighpassFilter.cpp
 Created: 25 Apr 2026 11:01:25pm
 Author:  Will Hinds
 
 ==============================================================================
 */

#include "Filter.h"

void Filter::setFilterType(FilterType filterType)
{
	juce::dsp::StateVariableTPTFilterType juceFilterType = static_cast<juce::dsp::StateVariableTPTFilterType>(filterType);
	
	if (juceFilterType != dspFilter.getType())
	{
		dspFilter.reset();
	}
	
	dspFilter.setType(juceFilterType);
}

void Filter::setCutoffFrequency(float inCutoffFrequency)
{
	if (!openFilter)
	{
		cutoffFrequency = inCutoffFrequency;
	}
	else
	{
		if (destinationCutoffFrequency != inCutoffFrequency)
		{
			destinationCutoffFrequency = inCutoffFrequency;
			cutoffFrequency = 20.0f;
			distanceBetween = destinationCutoffFrequency - cutoffFrequency;
		}
	}
}

void Filter::setResonance(float inResonance)
{
	resonance = inResonance;
}

void Filter::setOpenFilter(bool inOpenFilter)
{
	// turning off
	if (openFilter && !inOpenFilter)
	{
		cutoffFrequency = destinationCutoffFrequency;
		destinationCutoffFrequency = 20.0f;
		distanceBetween = 0.0f;
	}
	// turning on
	else if (!openFilter && inOpenFilter)
	{
		destinationCutoffFrequency = cutoffFrequency;
		cutoffFrequency = 20.0f;
		distanceBetween = destinationCutoffFrequency - cutoffFrequency;
	}
	
	openFilter = inOpenFilter;
}

void Filter::setOpenFilterSpeed(float inOpenFilterSpeed)
{
	openFilterSpeed = inOpenFilterSpeed;
}

void Filter::setSamplingRate(float inSamplingRate)
{
	samplingRate = inSamplingRate;
}

void Filter::prepareToPlay(double inSampleRate, int samplesPerBlock)
{
	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = inSampleRate;
	spec.numChannels = 2;
	dspFilter.prepare(spec);
}

void Filter::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	for (const juce::MidiMessageMetadata midiMessage : midiMessages)
	{
		const juce::MidiMessage midiEvent = midiMessage.getMessage();
		
		if (midiEvent.isNoteOn() && openFilter)
		{
			cutoffFrequency = 20.0f;
		}
	}
	
	dspFilter.setCutoffFrequency(cutoffFrequency);
	dspFilter.setResonance(resonance);
	juce::dsp::AudioBlock<float> block(buffer);
	juce::dsp::ProcessContextReplacing<float> context(block);
	dspFilter.process(context);
	
	if (openFilter)
	{
		const float increment = distanceBetween * openFilterSpeed * static_cast<float>(buffer.getNumSamples()) / 100000.0f;
		cutoffFrequency += increment;
		cutoffFrequency = std::min(cutoffFrequency, destinationCutoffFrequency);
	}
		
}

void Filter::reset()
{
	dspFilter.reset();
}
