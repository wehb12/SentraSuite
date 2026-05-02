/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "maximilian.h"

//==============================================================================
WavetableSynthAudioProcessor::WavetableSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
:
		AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
//		parameters(*this,
//				   nullptr,
//				   juce::Identifier("LowpassAndHighpassPlugin"),
//				   {std::make_unique<juce::AudioParameterFloat>("cutoff_frequency",
//																"Cutoff Frequency",
//																juce::NormalisableRange{20.f,
//																	20000.f,
//																	0.1f,
//																	0.2f,
//																	false},
//																500.f),
//					std::make_unique<juce::AudioParameterBool>("highpass",
//															   "Highpass",
//															   false)})
{
//	cutoffFrequencyParameter = parameters.getRawParameterValue("cutoff_frequency");
//	highpassParameter = parameters.getRawParameterValue("highpass");
}

WavetableSynthAudioProcessor::~WavetableSynthAudioProcessor()
{
}

//==============================================================================
const juce::String WavetableSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WavetableSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WavetableSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WavetableSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WavetableSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WavetableSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WavetableSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void WavetableSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WavetableSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	
	currentSampleRate = sampleRate;
	updateAngleDelta();
	createWavetable();
	
	synth.prepareToPlay(sampleRate, samplesPerBlock);
	filter.setSamplingRate(static_cast<float>(sampleRate));
}

void WavetableSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WavetableSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//float cutoffFrequency = 0.0f;
//bool noteOn = false;
void WavetableSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	maxiSettings::setup(currentSampleRate, totalNumOutputChannels, buffer.getNumSamples());
	
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = 0; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

//	synth.setCutoffFrequency(cutoffFrequency);
	synth.processBlock(buffer, midiMessages);
	
	// retrieve and set the parameter values
//	const auto cutoffFrequency = 150.0f;//cutoffFrequencyParameter->load();
	// in C++, std::atomic<T> to T conversion is equivalent to a load
//	const auto highpass = false; //*highpassParameter < 0.5f ? false : true;
//	filter.setCutoffFrequency(cutoffFrequency);
//	filter.setHighpass(highpass);

	// perform the filtering
//	filter.processBlock(buffer, midiMessages);
	
//	if (noteOn)
//	{
//		cutoffFrequency = std::min(cutoffFrequency + 4.0f, 200.0f);
//	}
//	
//	for (const juce::MidiMessageMetadata midiMessage : midiMessages)
//	{
//		const juce::MidiMessage midiEvent = midiMessage.getMessage();
//		
//		if (midiEvent.isNoteOn())
//		{
//			noteOn = true;
//		}
//		else if (midiEvent.isNoteOff() || midiEvent.isAllNotesOff())
//		{
//			cutoffFrequency = 0.0f;
//			noteOn = false;
//		}
//	}
}

//==============================================================================
bool WavetableSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WavetableSynthAudioProcessor::createEditor()
{
    return new WavetableSynthAudioProcessorEditor (*this/*, parameters*/);
}

//==============================================================================
void WavetableSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WavetableSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavetableSynthAudioProcessor();
}

void WavetableSynthAudioProcessor::updateAngleDelta()
{
	auto cyclesPerSample = currentFrequency / currentSampleRate; // [2]
	angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi; // [3]
}

void WavetableSynthAudioProcessor::createWavetable()
{
//	sineTable.setSize (1, (int) tableSize);
//	float* samples = sineTable.getWritePointer (0);
//	double angleDelta = juce::MathConstants<double>::twoPi / (double) (tableSize - 1);
//	double currentAngle = 0.0;
//	for (unsigned int i = 0; i < tableSize; ++i)
//	{
//		double sample = std::sin (currentAngle);
//		samples[i] = (float) sample;
//		currentAngle += angleDelta;
//	}
}

void WavetableSynthAudioProcessor::reset()
{
	synth.reset();
}
