/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAudioProcessor::WavetableSynthAudioProcessor()
	:
#ifndef JucePlugin_PreferredChannelConfigurations
	  AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ) ,
#endif
	tree(*this, nullptr, "Parameters", createParameters())
	, synth(tree)
{
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
	synth.prepareToPlay(sampleRate, samplesPerBlock);
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
	
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = 0; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	synth.processBlock(buffer, midiMessages);
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
	juce::ValueTree state = tree.copyState();
	std::unique_ptr<juce::XmlElement> xml (state.createXml());
	copyXmlToBinary (*xml, destData);
}

void WavetableSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName (tree.state.getType()))
		{
			tree.replaceState(juce::ValueTree::fromXml (*xmlState));
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavetableSynthAudioProcessor();
}

void WavetableSynthAudioProcessor::reset()
{
	synth.reset();
}

juce::AudioProcessorValueTreeState::ParameterLayout WavetableSynthAudioProcessor::createParameters()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
	
	// wavetable oscillator type
	params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("TYPECOMBOBOX", 1), "TypeComboBox", juce::StringArray("Sine Wave", "Square Wave", "Saw Wave"), 1));
	
	// envelope
	params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ATTACKSLIDER", 1), "AttackSlider", juce::NormalisableRange<float>(0.1f, 5000.0f), 500.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DECAYSLIDER", 1), "DecaySlider", juce::NormalisableRange<float>(1.0f, 2000.0f), 500.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SUSTAINSLIDER", 1), "SustainSlider", juce::NormalisableRange<float>(0.0f, 1.0f), 0.8f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("RELEASESLIDER", 1), "ReleaseSlider", juce::NormalisableRange<float>(.1f, 5000.0f), 200.0f));
	
	// filter
	params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("CUTOFFSLIDER", 1), "CutoffSlider", juce::NormalisableRange<float>(20.0f, 20000.0f, 0, 0.2f), 200.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("RESONANCESLIDER", 1), "Resonancelider", juce::NormalisableRange<float>(0.5f, 5.0f), 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("FILTERTYPECOMBOBOX", 1), "FilterTypeComboBox", juce::StringArray("Low Pass", "High Pass", "Band Pass", "Test"), 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("OPENFILTERSPEEDSLIDER", 1), "OpenFilterSpeedSlider", juce::NormalisableRange<float>(1.0f, 10.0f), 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("OPENFILTERSTATE", 1), "OpenFilterSpeedButton", false));
	params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("KEYTRACKSTATE", 1), "KeyTackButton", false));
	
	return { params.begin(), params.end() };
}
