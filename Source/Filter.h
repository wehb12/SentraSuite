/*
  ==============================================================================

    LowpassHighpassFilter.h
    Created: 25 Apr 2026 11:01:25pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <vector>
#include "JuceHeader.h"
#include "juce_dsp/processors/juce_StateVariableTPTFilter.h"

enum FilterType
{
	LowPass,
	HighPass,
	BandPass,
	
	FilterType_END
};

class Filter
{
public:
	void setFilterType(FilterType filterType);
	void setCutoffFrequency(float inCutoffFrequency);
	void setResonance(float inResonance);
	
	inline float getCutoffFrequency() { return cutoffFrequency; };
	void setSamplingRate(float samplingRate);
	
	void prepareToPlay(double inSampleRate, int samplesPerBlock);
	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);
	void reset();
	
private:
	bool isHighpass = false;
	float cutoffFrequency = 20.0f;
	float samplingRate = 0.0f;;
	float resonance = 1.0f;
	
	juce::dsp::StateVariableTPTFilter<float> dspFilter;
};

