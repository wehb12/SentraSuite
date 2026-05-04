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
	cutoffFrequency = inCutoffFrequency;
}

void Filter::setResonance(float inResonance)
{
	resonance = inResonance;
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

void Filter::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
	dspFilter.setCutoffFrequency(cutoffFrequency);
	dspFilter.setResonance(resonance);
	juce::dsp::AudioBlock<float> block(buffer);
	juce::dsp::ProcessContextReplacing<float> context(block);
	dspFilter.process(context);
}

void Filter::reset()
{
	dspFilter.reset();
}
