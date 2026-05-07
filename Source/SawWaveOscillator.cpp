/*
  ==============================================================================

    SawWaveOscillator.cpp
    Created: 7 May 2026 9:28:46pm
    Author:  Will Hinds

  ==============================================================================
*/

#include "SawWaveOscillator.h"
#include <JuceHeader.h>

void SawWaveOscillator::generateWavetable()
{
	waveTable.reserve(WAVETABLE_LENGTH);
	
	float amplitude = 0.0f;
	for (int i = 0; i < WAVETABLE_LENGTH; ++i)
	{
		waveTable.emplace_back(amplitude);
		amplitude += 2.0f / static_cast<float>(WAVETABLE_LENGTH);
		
		if (i == WAVETABLE_LENGTH / 2)
		{
			amplitude -= 2.0f;
		}
	}
}
