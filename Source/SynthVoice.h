/*
  ==============================================================================

    SynthVoice.h
    Created: 10 Apr 2022 8:10:44pm
    Author:  Ricky

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:

    bool canPlaySound(juce::SynthesiserSound*) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newPitchWheelValue) override;

    void controllerMoved(int controllerNumber, int newControllerValue) override;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannelsNumber);

private:

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::AudioBuffer<float> synthBuffer; //Local buffer to temporarily store synth output


    //Oscillator variables
    juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };

    juce::dsp::Gain<float> gain;

    bool isPrepared = false;

};