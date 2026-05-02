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

class Filter
{
public:
	void setIsHighpass(bool highpass);
	void setCutoffFrequency(float cutoffFrequency);
	inline float getCutoffFrequency() { return cutoffFrequency; };
	void setSamplingRate(float samplingRate);
	
	void prepareToPlay(double inSampleRate, int samplesPerBlock);
	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);
	void reset();
	
private:
	bool isHighpass = false;
	float cutoffFrequency = 100.0f;
	float samplingRate = 0.0f;;
	float resonance = 3.0f;
	
	juce::dsp::StateVariableTPTFilter<float> dspFilter;
};

