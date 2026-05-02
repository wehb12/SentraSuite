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

class WavetableOscillator
{
	public:
	WavetableOscillator(std::vector<float> wavetable, double inSampleRate);
	
	void setFrequency(float frequency);
	float getSample();
	
	void stop() { index = 0.0f; indexIncrement = 0.0f; };
	bool isPlaying() { return indexIncrement != 0.0f; };
private:
	float interpolateLinearly();
	
	WaveTable waveTable;
	double sampleRate = 0.0;
	float index = 0.0f;
	float indexIncrement = 0.0f;
};
