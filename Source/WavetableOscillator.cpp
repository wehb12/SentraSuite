/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 24 Apr 2026 9:08:56pm
    Author:  Will Hinds

  ==============================================================================
*/

#include "WavetableOscillator.h"
#include <cmath>

WavetableOscillator::WavetableOscillator(std::vector<float> wavetable, double inSampleRate)
: waveTable(std::move(wavetable))
, sampleRate(inSampleRate)
{
	
}

void WavetableOscillator::setFrequency(float frequency)
{
	indexIncrement = frequency * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
}

float WavetableOscillator::getSample()
{
	const auto sample = interpolateLinearly();
	index += indexIncrement;
	index = std::fmod(index, static_cast<float>(waveTable.size()));
	return sample;
}

float WavetableOscillator::interpolateLinearly()
{
	const int truncatedIndex = static_cast<int>(index);
	const int nextIndex = (truncatedIndex + 1) % static_cast<int>(waveTable.size());
	
	const float nextIndexWeight = index - static_cast<float>(truncatedIndex);
	const float truncatedIndexWeight = 1.0f - nextIndexWeight;
	
	return truncatedIndexWeight * waveTable[truncatedIndex] + nextIndexWeight * waveTable[nextIndex];
}
