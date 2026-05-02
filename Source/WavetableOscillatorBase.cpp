/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 24 Apr 2026 9:08:56pm
    Author:  Will Hinds

  ==============================================================================
*/

#include "WavetableOscillatorBase.h"
#include <cmath>

WavetableOscillatorBase::WavetableOscillatorBase()
{
}

WavetableOscillatorBase::~WavetableOscillatorBase()
{
	
}

void WavetableOscillatorBase::init(double inSampleRate)
{
	sampleRate = inSampleRate;
	generateWavetable();
}

void WavetableOscillatorBase::setFrequency(float frequency)
{
	indexIncrement = frequency * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
}

float WavetableOscillatorBase::getSample()
{
	const auto sample = interpolateLinearly();
	index += indexIncrement;
	index = std::fmod(index, static_cast<float>(waveTable.size()));
	return sample;
}

float WavetableOscillatorBase::interpolateLinearly()
{
	const int truncatedIndex = static_cast<int>(index);
	const int nextIndex = (truncatedIndex + 1) % static_cast<int>(waveTable.size());
	
	const float nextIndexWeight = index - static_cast<float>(truncatedIndex);
	const float truncatedIndexWeight = 1.0f - nextIndexWeight;
	
	return truncatedIndexWeight * waveTable[truncatedIndex] + nextIndexWeight * waveTable[nextIndex];
}
