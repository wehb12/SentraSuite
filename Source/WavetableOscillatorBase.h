/*
  ==============================================================================

    WavetableOscillator.h
    Created: 24 Apr 2026 9:08:47pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <vector>

using WaveTable = std::vector<float>;

class WavetableOscillatorBase
{
public:
	WavetableOscillatorBase();
	virtual ~WavetableOscillatorBase();
	
	void init(double inSampleRate);
	
	void setFrequency(float frequency);
	float getSample();
	
	void stop() { index = 0.0f; indexIncrement = 0.0f; };
	bool isPlaying() { return indexIncrement != 0.0f; };
	
protected:
	float interpolateLinearly();
	
	virtual void generateWavetable() = 0;
	
	WaveTable waveTable;
	double sampleRate = 0.0;
	float index = 0.0f;
	float indexIncrement = 0.0f;
	
	static constexpr int WAVETABLE_LENGTH = 64;
};
