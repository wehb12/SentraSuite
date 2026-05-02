/*
  ==============================================================================

    SineWaveOscillator.h
    Created: 2 May 2026 11:40:28pm
    Author:  Will Hinds

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableOscillatorBase.h"

//==============================================================================
/*
*/
class SineWaveOscillator  : public WavetableOscillatorBase
{
public:
	virtual void generateWavetable() override;
};
