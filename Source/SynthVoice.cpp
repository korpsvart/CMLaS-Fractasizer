/*
  ==============================================================================

    SynthVoice.cpp
    Created: 10 Apr 2022 8:11:10pm
    Author:  Ricky

  ==============================================================================
*/

#include "SynthVoice.h"


SynthVoice::SynthVoice(float harmonicN)
{
    this->harmonicN = harmonicN;
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber)*harmonicN);
    adsr.noteOn();

}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

    jassert(isPrepared);


    if (!isVoiceActive())
        return;


    //Set the size for the local temp buffer (only re-allocate if needed)
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    //clear the local temp buffer
    synthBuffer.clear();




    //AudioBlock it's just an alias for the given buffer needed by DSP classes (so by modifying
    //the AudioBlock we are actually modifying the given buffer)
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

    osc.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    panner.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    //Add the local temp buffer to the final audio output buffer

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

    }

    if (!adsr.isActive())
        clearCurrentNote();


}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannelsNumber)
{

    //Prepare oscillator (passing ProcessSpec)

    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannelsNumber;

    osc.prepare(spec);
    gain.prepare(spec);
    panner.prepare(spec);


    isPrepared = true;
}


void SynthVoice::setHarmonicN(float harmonicN)
{

    this->harmonicN = harmonicN;
}

void SynthVoice::setGain(float gainValue)
{

    gain.setGainLinear(gainValue);
}

void SynthVoice::setPan(float panValue)
{
    panner.setPan(panValue);
}
