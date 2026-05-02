/*
  ==============================================================================

    SquareWaveOscillator.h
    Created: 2 May 2026 11:29:46pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableOscillatorBase.h"

//==============================================================================
/*
*/
class SquareWaveOscillator  : public WavetableOscillatorBase
{
public:
	virtual void generateWavetable() override;
};
