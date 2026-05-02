/*
 ==============================================================================
 
 LowpassHighpassFilter.cpp
 Created: 25 Apr 2026 11:01:25pm
 Author:  Will Hinds
 
 ==============================================================================
 */

#include "Filter.h"

void Filter::setIsHighpass(bool highpass)
{
	this->isHighpass = highpass;
}

void Filter::setCutoffFrequency(float cutoffFrequency)
{
	this->cutoffFrequency = cutoffFrequency;
}

void Filter::setSamplingRate(float samplingRate)
{
	this->samplingRate = samplingRate;
}

void Filter::prepareToPlay(double inSampleRate, int samplesPerBlock)
{
	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = inSampleRate;
	spec.numChannels = 2;
	dspFilter.prepare(spec);
	dspFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
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
