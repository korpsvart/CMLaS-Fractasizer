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

    SynthVoice(int numPartials);
    

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

    void setFreqDetunes(const std::vector<double>& freqDetunes);

    void setLFORates(const std::vector<double>& lfoRates);

    void setLFODepths(const std::vector<double>& lfoDepths);

    void updateADSR(int i, const float attack, const float decay, const float sustain, const float release);


    enum Pippo
    {
        oscIndex,
        gainIndex,
        panIndex
    };


    void setWaveType(const int partialIndex, const int choice);

    void applyLFO(int i);
    
    //had to make this buffer public to be able to access in the processor. 
    juce::OwnedArray<juce::AudioBuffer<float>> synthBuffers; //Local buffers to temporarily store synth output (one for each partial)

private:

    enum waveTypes
    {
        SINE,
        SAW,
        SQUARE
    };

    int numPartials;

    std::vector< juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>, juce::dsp::Panner<float>>> processorChains;

    std::vector<juce::ADSR> adsr;
    std::vector<juce::ADSR::Parameters> adsrParams;
    //juce::OwnedArray<juce::AudioBuffer<float>> synthBuffers; //Local buffers to temporarily store synth output (one for each partial)



    //Oscillator variables
    //juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };

    std::vector < juce::dsp::Oscillator<float>> lfos;


    std::vector<float> lfoRates;
    std::vector<float> lfoDepths;



    //float lfoRate = 3.0f; //Hz, how fast the lfo oscillates
    //float lfoDepth = 0.5f; //Depends on what you want to control
    
    static constexpr size_t lfoUpdateRate = 100;
    
    size_t lfoUpdateCounter = lfoUpdateRate;
    
    //juce::dsp::Gain<float> gain;

    std::vector<float> fixedGains;

    //juce::dsp::Panner<float> panner;


    float harmonicN = 1; //Identifies the order of this oscillator harmonic (this will be multiplied by the given frequency)


    std::vector<float> detuneFactors; //default is harmonic

    bool isPrepared = false;

    

};
