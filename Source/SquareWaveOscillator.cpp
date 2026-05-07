/*
  ==============================================================================

    SquareWaveOscillator.cpp
    Created: 2 May 2026 11:29:46pm
    Author:  Will Hinds

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SquareWaveOscillator.h"

void SquareWaveOscillator::generateWavetable()
{
	waveTable.reserve(WAVETABLE_LENGTH);
	
	for (int i = 0; i < (WAVETABLE_LENGTH / 2); ++i)
	{
		waveTable.emplace_back(-1);
	}
	for (int i = (WAVETABLE_LENGTH / 2); i < WAVETABLE_LENGTH; ++i)
	{
		waveTable.emplace_back(1);
	}
}
