/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"


//==============================================================================
/**
*/
class FractalSynthesisAudioProcessor : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    FractalSynthesisAudioProcessor();
    ~FractalSynthesisAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;


    void parameterChanged(const juce::String& parameterID, float newValue) override;

    static const int NUM_PARTIALS = 4;


    juce::AudioProcessorValueTreeState apvts;

  
    
    juce::OwnedArray<juce::AudioVisualiserComponent> waveVisualisers;
 
private:


    //Synth variables


    //number of partials to generate for additive synthesis, will correspond to number of synths



    //Define the number of polyphonies (max number of MIDI notes that can be played at the same time)
    //Should be constant during the execution
    static const int voicesNumber = 10;

    std::vector<juce::Synthesiser*> synths;

    juce::Synthesiser* synth;


    std::vector<std::complex<double>> fractalPoints = { 0, 0, 0, 0 }; //to store the fractal points

    std::vector<double> lfoRates = { 3, 3, 3, 3 };
    std::vector<double> freqDetunes = { 1, 2, 3, 4 };

    juce::AudioProcessorValueTreeState::ParameterLayout createParams();



    static std::complex<double> mandelbrot(std::complex<double> z, std::complex<double> c);

    static std::complex<double> burningShip(std::complex<double> z, std::complex<double> c);
    
    static std::complex<double> tricorn(std::complex<double> z, std::complex<double> c);

    std::complex<double>(*currentFractal)(std::complex<double> z, std::complex<double> c) = mandelbrot;

    void generateFractalSuccession(std::complex<double> c);

    void generateLFORates(std::vector<std::complex<double>> fractalSuccession);


    void generateFreqDetunes(std::vector<std::complex<double>> fractalSuccession);

    void updateADSR(int partialIndex, SynthVoice* voice);



    bool updatedFractal = true;
   


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalSynthesisAudioProcessor)
};
