/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
FractalSynthesisAudioProcessor::FractalSynthesisAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", FractalSynthesisAudioProcessor::createParams())
#endif
{

    //Create synth voices

    //voicesNumber voices for each synth (allows voicesNumber MIDI notes to be played at the same time)
    //First synth will play always the fundamental
    //Second synth will play the 2nd harmonic and so on


    //Create synths

    for (size_t order = 0; order < partialsNumber; order++)
    {
        juce::Synthesiser* synth = new juce::Synthesiser(); //create synth
        synth->addSound(new SynthSound()); //add SynthSound (only one)
        //Add 4 voices at same harmonic order (order+1)
        for (size_t voice = 0; voice < voicesNumber; voice++)
        {
            synth->addVoice(new SynthVoice(order+1));
        }
        synths.push_back(synth); //Add current synth
    }

    //TODO: refactoring to correctly free memory


    //Add this as listener to the choice param
    apvts.addParameterListener("FRACTAL_FUNCTION", this);


}

FractalSynthesisAudioProcessor::~FractalSynthesisAudioProcessor()
{

}

//==============================================================================
const juce::String FractalSynthesisAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FractalSynthesisAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FractalSynthesisAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FractalSynthesisAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FractalSynthesisAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FractalSynthesisAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FractalSynthesisAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FractalSynthesisAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FractalSynthesisAudioProcessor::getProgramName (int index)
{
    return {};
}

void FractalSynthesisAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FractalSynthesisAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..



    for (juce::Synthesiser* synth: synths)
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
            {
                voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
            }

        }
        synth->setCurrentPlaybackSampleRate(sampleRate);
    }



}

void FractalSynthesisAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FractalSynthesisAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FractalSynthesisAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());




    for (juce::Synthesiser* synth : synths)
    {
        for (int i = 0; i < synth->getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(synth->getVoice(i)))
            {
                // Osc controls
                // ADSR
                // LFO ecc...
            }
        }
        synth->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }


}

//==============================================================================
bool FractalSynthesisAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FractalSynthesisAudioProcessor::createEditor()
{
    return new FractalSynthesisAudioProcessorEditor (*this);
}

//==============================================================================
void FractalSynthesisAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FractalSynthesisAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void FractalSynthesisAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    //Here we update the values of the synths based on the selected fractal function



    if (strcmp(parameterID.toRawUTF8(), "FRACTAL_FUNCTION"))
    {
        int fractalOption = newValue;

        switch (fractalOption)
        {
        case 0:

            break;
        case 1:

            break;
        default:
            break;
        }


    }
}

juce::AudioProcessorValueTreeState::ParameterLayout FractalSynthesisAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterChoice>("FRACTAL_FUNCTION", "Fractal Function",
        juce::StringArray("Mandelbrot Set", "Burning Ship Set"), 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("INITIAL_POINT_X", "Initial Point X", 0.1, 1, 0.5));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("INITIAL_POINT_Y", "Initial Point Y", 0.1, 1, 0.5));

    return { params.begin(), params.end() };
}

std::complex<double> FractalSynthesisAudioProcessor::mandelbrot(std::complex<double> z, std::complex<double> c)
{
    return std::pow(z, 2) + c;
}

std::complex<double> FractalSynthesisAudioProcessor::burningShip(std::complex<double> z, std::complex<double> c)
{
    std::complex<double> z1(std::abs(z.real()), std::abs(z.imag()));
    return std::pow(z1, 2) + c;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FractalSynthesisAudioProcessor();
}
