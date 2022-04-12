/*
  ==============================================================================

    SynthVoice.cpp
    Created: 10 Apr 2022 8:11:10pm
    Author:  Ricky

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();

}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
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


    //AudioBlock it's just an alias for the output buffer (so by modifying
    //the AudioBlock we are actually modifying the given buffer)
    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };

    osc.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);


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

    osc.setFrequency(440);
    gain.setGainLinear(0.01f);

    isPrepared = true;
}
