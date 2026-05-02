/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableSynth.h"
#include "LowpassHighpassFilter.h"

//==============================================================================
/**
*/
class WavetableSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    WavetableSynthAudioProcessor();
    ~WavetableSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
	// Sine wave oscillator
	void setFrequency(double inFrequency) { targetFrequency = inFrequency; }
	double getFrequency() { return targetFrequency; }
	double getCurrentSampleRate() { return currentSampleRate; }
	void updateAngleDelta();
	// Sine wave oscillator
	
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthAudioProcessor)
	
	void createWavetable();
	virtual void reset() override;
	
	// Sine wave oscillator
	double currentSampleRate = 0.0;
	double currentAngle = 0.0;
	double angleDelta = 0.0;
	double currentFrequency = 60.0;
	double targetFrequency = 60.0;
	// Sine wave oscillator
	
	//const unsigned int tableSize = 1 << 7;
	//float level = 0.0f;
	//juce::AudioSampleBuffer sineTable;
	//juce::OwnedArray<WavetableOscillator> oscillators;
	
	WavetableSynth synth;
	
	// our plugin's parameters
//	juce::AudioProcessorValueTreeState parameters;
	std::atomic<float>* cutoffFrequencyParameter = nullptr;
	std::atomic<float>* highpassParameter = nullptr;
	
	// the filter implemented in listings 1-3
	LowpassHighpassFilter filter;
};
