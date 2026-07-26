//
//  WavetableSynth.cpp
//  WavetableSynth
//
//  Created by Will Hinds on 24/04/2026.
//

#include "WavetableSynth.h"
#include "SquareWaveOscillator.h"
#include "SineWaveOscillator.h"
#include "SawWaveOscillator.h"
#include "UIKeyNames.h"


WavetableSynth::WavetableSynth(juce::AudioProcessorValueTreeState& inTree)
	: tree(inTree)
{
}

void WavetableSynth::prepareToPlay(double inSampleRate, int samplesPerBlock)
{
	sampleRate = inSampleRate;
	
	initialiseOsciallators(inSampleRate,  samplesPerBlock);
	
	reset();
	
	filter.prepareToPlay(inSampleRate, samplesPerBlock);
}

void WavetableSynth::reset()
{
	filter.reset();
}

void WavetableSynth::initialiseOsciallators(double inSampleRate, int samplesPerBlock)
{
	oscillators.clear();
	secondaryOscillators.clear();
	oscillators.reserve(OSCILLATORS_COUNT);
	secondaryOscillators.reserve(OSCILLATORS_COUNT);
	envelopes.reserve(OSCILLATORS_COUNT);
	secondaryEnvelopes.reserve(OSCILLATORS_COUNT);
	for (int i = 0; i < OSCILLATORS_COUNT; ++i)
	{
		oscillators.emplace_back(std::make_shared<SquareWaveOscillator>());
		secondaryOscillators.emplace_back(std::make_shared<SawWaveOscillator>());
		oscillators.back()->init(inSampleRate);
		secondaryOscillators.back()->init(inSampleRate);
		envelopes.emplace_back();
		secondaryEnvelopes.emplace_back();
	}
}

void WavetableSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	setOscillators();
	buildUnisonOscillators();
	
	int currentSample = 0;
	
	for (const juce::MidiMessageMetadata midiMessage : midiMessages)
	{
		const juce::MidiMessage midiEvent = midiMessage.getMessage();
		const int midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
		
		render(buffer, currentSample, midiEventSample);
		handleMidiEvent(midiEvent);
		
		currentSample = midiEventSample;
	}
	
	render(buffer, currentSample, buffer.getNumSamples());
	
	if (isPlaying)
	{
		doFilter(buffer, midiMessages);
	}
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
	float* firstChannel = buffer.getWritePointer(0);
	
	isPlaying = false;
	for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
	{
		std::shared_ptr<WavetableOscillatorBase> osc1 = oscillators[oscillatorId];
		std::shared_ptr<WavetableOscillatorBase> osc2 = secondaryOscillators[oscillatorId];
		maxiEnv& env1 = getEnvelope(oscillatorId);
		maxiEnv& env2 = getEnvelope2(oscillatorId);

		if (osc1->isPlaying() || osc2->isPlaying())
		{
			isPlaying = true;
			for (int sample = startSample; sample < endSample; ++sample)
			{
				float osc1SampleLeft = 0.0f;
				float osc1SampleRight = 0.0f;
				for (int i = 0; i < unisonNumVoices; ++i)
				{
					const float middleIndex = std::floor(unisonNumVoices / 2);
					const bool noBias = (i == middleIndex) && ((unisonNumVoices / 2.0f) - middleIndex) > 0.0f;
					
					float leftChannelMultipler = 1.0f;
					float rightChannelMultipler = 1.0f;
					if (!noBias)
					{
						const bool leftBias = i < middleIndex;
						const float unisonStereoMultiplier = (unisonStereoAmount / 200.0f) + 0.5f;
						leftChannelMultipler = leftBias ? unisonStereoMultiplier : (1.0f - unisonStereoMultiplier);
						rightChannelMultipler = leftBias ? (1.0f - unisonStereoMultiplier) : unisonStereoMultiplier;
					}
					
					const float sample = unisonOscillators[i]->getSample();
					osc1SampleLeft += sample * leftChannelMultipler;
					osc1SampleRight += sample * rightChannelMultipler;
				}
				
				const float sampleEnv1Left = env1.adsr((osc1SampleLeft / 1.0f), env1.trigger) * 0.1f;
				const float sampleEnv1Right = env1.adsr((osc1SampleRight / 1.0f), env1.trigger) * 0.1f;
				
				const float sampleEnv2 = env2.adsr(osc2->getSample(), env2.trigger) * 0.1f;
				
				firstChannel[sample] += ((sampleEnv1Left * (1.0f - secondOscAmount)) + (sampleEnv2 * secondOscAmount));
				buffer.getWritePointer(1)[sample] += ((sampleEnv1Right * (1.0f - secondOscAmount)) + (sampleEnv2 * secondOscAmount));
				
				// This stopping code doesn't work properly and is buggy
				// TODO: Swap to some sort of timer based on the release value and the time the key was pressed. Maybe some envelope wrapper class could handle it
				if (env1.trigger == 0 && (sampleEnv1Left < 0.000001 && sampleEnv1Left > -0.000001))
				{
					osc1->stop();
				}
				if (env2.trigger == 0 && (sampleEnv2 < 0.000001 && sampleEnv2 > -0.000001))
				{
					osc2->stop();
				}
			}
		}
	}

	
//	for(int channel = 1; channel < buffer.getNumChannels(); ++channel)
//	{
//		std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
//	}
	
}

void WavetableSynth::doFilter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	filter.setCutoffFrequency(tree.getRawParameterValue("CUTOFFSLIDER")->load());
	filter.setOpenFilter(tree.getRawParameterValue("OPENFILTERSTATE")->load() < 0.5f ? false : true);
	filter.setOpenFilterSpeed(tree.getRawParameterValue("OPENFILTERSPEEDSLIDER")->load());
	filter.setKeyTrack(tree.getRawParameterValue("KEYTRACKSTATE")->load() < 0.5f ? false : true);
	filter.setResonance(tree.getRawParameterValue("RESONANCESLIDER")->load());
	filter.setFilterType(static_cast<FilterType>(static_cast<int>(tree.getRawParameterValue("FILTERTYPECOMBOBOX")->load())));
	filter.processBlock(buffer, midiMessages);
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn())
	{
		const int oscillatorId = midiEvent.getNoteNumber();
		const float frequency1 = midiNoteNumberTofrequency(oscillatorId, oscSemitoneAmount);
		const float frequency2 = midiNoteNumberTofrequency(oscillatorId, secondOscSemitoneAmount);
		oscillators[oscillatorId]->setFrequency(frequency1);
		secondaryOscillators[oscillatorId]->setFrequency(frequency2);
		envelopes[oscillatorId].trigger = 1;
		secondaryEnvelopes[oscillatorId].trigger = 1;
		
		const float unisonVoiceFreqNext = midiNoteNumberTofrequency(oscillatorId, oscSemitoneAmount, unisonDetuneAmount);
		const float detuneAmount = unisonVoiceFreqNext - frequency1;
		
		// TODO: this is a temp fix to clear the unison oscialltors every time a new midi note is pressed
		// It stops artifacts from last midi note sharing the same unison oscillators
		// We should instead have one unison group per potentital midi voice
		buildUnisonOscillators(true);
		for (int i = 0; i < unisonNumVoices; ++i)
		{
			const float detuneMultiplier = i - std::floor(unisonNumVoices / 2);
			const float unisonVoiceFrequency = frequency1 + (detuneMultiplier * detuneAmount);
			unisonOscillators[i]->setFrequency(unisonVoiceFrequency);
		}
	}
	else if (midiEvent.isNoteOff())
	{
		const int oscillatorId = midiEvent.getNoteNumber();
		envelopes[oscillatorId].trigger = 0;
		secondaryEnvelopes[oscillatorId].trigger = 0;
	}
	else if (midiEvent.isAllNotesOff())
	{
		for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
		{
			oscillators[oscillatorId]->stop();
			envelopes[oscillatorId].trigger = 0;
		}
	}
}

void WavetableSynth::setOscillators()
{
	const WavetableType newType = static_cast<WavetableType>(static_cast<int>(tree.getRawParameterValue("TYPECOMBOBOX")->load()));
	if (oscType != newType)
	{
		oscType = newType;
		switch(oscType)
		{
			case SineWave:
					setOscillator<SineWaveOscillator>();
				break;
			case SquareWave:
					setOscillator<SquareWaveOscillator>();
				break;
			case SawWave:
					setOscillator<SawWaveOscillator>();
				break;
			default:
				break;
		}
		
		constexpr bool forceResetOscs = true;
		buildUnisonOscillators(forceResetOscs);
	}
	
	const WavetableType newSecondaryType = static_cast<WavetableType>(static_cast<int>(tree.getRawParameterValue("SECONDOSCTYPECOMBOBOX")->load()));
	secondOscAmount = tree.getRawParameterValue("SECONDOSCAMOUNT")->load();
	if (secondOscType != newSecondaryType)
	{
		secondOscType = newSecondaryType;
		switch(secondOscType)
		{
			case SineWave:
					setSecondaryOscillator<SineWaveOscillator>();
				break;
			case SquareWave:
					setSecondaryOscillator<SquareWaveOscillator>();
				break;
			case SawWave:
					setSecondaryOscillator<SawWaveOscillator>();
				break;
			default:
				break;
		}
	}
	
	const float newOscSemitoneAmount = tree.getRawParameterValue("OSCSEMITONEAMOUNT")->load();
	const float newSecondOscSemitoneAmount = tree.getRawParameterValue("SECONDOSCSEMITONEAMOUNT")->load();
	
	bool newSemitoneAmount = false;
	if (newOscSemitoneAmount != oscSemitoneAmount)
	{
		oscSemitoneAmount = newOscSemitoneAmount;
		for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
		{
			oscillators[oscillatorId]->setFrequency(midiNoteNumberTofrequency(oscillatorId, oscSemitoneAmount));
		}
		newSemitoneAmount = true;
	}
	if (newSecondOscSemitoneAmount != secondOscSemitoneAmount)
	{
		secondOscSemitoneAmount = newSecondOscSemitoneAmount;
		for (int oscillatorId = 0; oscillatorId < oscillators.size(); ++oscillatorId)
		{
			secondaryOscillators[oscillatorId]->setFrequency(midiNoteNumberTofrequency(oscillatorId, secondOscSemitoneAmount));
		}
		newSemitoneAmount = true;
	}

	if (newSemitoneAmount)
	{
		filter.setSemitoneAmount(std::min(oscSemitoneAmount, secondOscSemitoneAmount));
	}
}

maxiEnv& WavetableSynth::getEnvelope(int oscillatorId)
{
	maxiEnv& env = envelopes[oscillatorId];
	
	env.setAttack(tree.getRawParameterValue("ATTACKSLIDER")->load());
	env.setDecay(tree.getRawParameterValue("DECAYSLIDER")->load());
	env.setSustain(tree.getRawParameterValue("SUSTAINSLIDER")->load());
	env.setRelease(tree.getRawParameterValue("RELEASESLIDER")->load());
	
	return env;
}

maxiEnv& WavetableSynth::getEnvelope2(int oscillatorId)
{
	maxiEnv& env2 = secondaryEnvelopes[oscillatorId];
	
	env2.setAttack(tree.getRawParameterValue("ATTACKSLIDER")->load());
	env2.setDecay(tree.getRawParameterValue("DECAYSLIDER")->load());
	env2.setSustain(tree.getRawParameterValue("SUSTAINSLIDER")->load());
	env2.setRelease(tree.getRawParameterValue("RELEASESLIDER")->load());
	
	return env2;
}

void WavetableSynth::buildUnisonOscillators(bool forceResetOscs)
{
	unisonNumVoices = tree.getRawParameterValue(UNISON_NUM_VOICES)->load();
	unisonDetuneAmount = tree.getRawParameterValue(UNISON_DETUNE_AMOUNT)->load();
	unisonStereoAmount = tree.getRawParameterValue(UNISON_STEREO_AMOUNT)->load();
	
	const int oldNumOscs = (int)unisonOscillators.size();
	if (forceResetOscs || unisonNumVoices != oldNumOscs)
	{
		unisonOscillators.clear();
		unisonOscillators.reserve(unisonNumVoices);
		for (int i = 0; i < unisonNumVoices; ++i)
		{
			switch(oscType)
			{
				case SineWave:
						unisonOscillators.emplace_back(std::make_shared<SineWaveOscillator>());
					break;
				case SquareWave:
						unisonOscillators.emplace_back(std::make_shared<SquareWaveOscillator>());
					break;
				case SawWave:
						unisonOscillators.emplace_back(std::make_shared<SawWaveOscillator>());
					break;
				default:
					break;
			}
			unisonOscillators[i]->init(sampleRate);
		}
	}
}

