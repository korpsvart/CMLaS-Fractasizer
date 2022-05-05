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

    SynthVoice(float harmonicN);
    

    bool canPlaySound(juce::SynthesiserSound*) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newPitchWheelValue) override;

    void controllerMoved(int controllerNumber, int newControllerValue) override;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannelsNumber);

    void setHarmonicN(float harmonicN);

    void setGain(float gainValue);

    void setPan(float panValue);

    void updateADSR(const float attack, const float decay, const float sustain, const float release);

    void setLFOParams(const float lfoRate, const float lfoDepth);

    void setWaveType(const int choice);


private:

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::AudioBuffer<float> synthBuffer; //Local buffer to temporarily store synth output



    //Oscillator variables
    juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };

    juce::dsp::Oscillator<float> lfoOsc{ [](float x) { return std::sin(x); }, 128};

    float lfoRate = 3.0f; //Hz, how fast the lfo oscillates
    float lfoDepth = 0.5f; //Depends on what you want to control
    
    static constexpr size_t lfoUpdateRate = 100;
    
    size_t lfoUpdateCounter = lfoUpdateRate;
    
    juce::dsp::Gain<float> gain;

    float fixedGain = 0.5f;

    juce::dsp::Panner<float> panner;


    float harmonicN = 1; //Identifies the order of this oscillator harmonic (this will be multiplied by the given frequency)

    bool isPrepared = false;

};
