/*
  ==============================================================================

    SawWaveOscillator.h
    Created: 7 May 2026 9:28:46pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableOscillatorBase.h"

//==============================================================================
/*
*/
class SawWaveOscillator  : public WavetableOscillatorBase
{
public:
	virtual void generateWavetable() override;
};
