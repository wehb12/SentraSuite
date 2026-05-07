/*
  ==============================================================================

    SineWaveOscillator.cpp
    Created: 2 May 2026 11:40:28pm
    Author:  Will Hinds

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SineWaveOscillator.h"

void SineWaveOscillator::generateWavetable()
{
	waveTable.reserve(WAVETABLE_LENGTH);
	
	for (int i = 0; i < WAVETABLE_LENGTH; ++i)
	{
		waveTable.emplace_back(std::sinf(juce::MathConstants<float>::twoPi * static_cast<float>(i) / static_cast<float>(WAVETABLE_LENGTH)));
	}
}
