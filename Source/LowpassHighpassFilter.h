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

class LowpassHighpassFilter
{
	public:
	// setters
	void setHighpass(bool highpass);
	void setCutoffFrequency(float cutoffFrequency);
	void setSamplingRate(float samplingRate);
	
	// Does not necessarily need to use JUCE's audio buffer
	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);
	
	private:
	bool highpass = false;
	float cutoffFrequency = 100.0f;
	float samplingRate = 0.0f;;
	
	// allpass filters' buffers: 1 sample per channel
	std::vector<float> dnBuffer;
};

