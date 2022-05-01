/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


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


    juce::AudioProcessorValueTreeState apvts;



private:


    //Synth variables


    //number of partials to generate for additive synthesis, will correspond to number of synths
    //Can be changed during the execution (through the GUI)
    int partialsNumber = 4;


    //Define the number of polyphonies (max number of MIDI notes that can be played at the same time)
    //Should be constant during the execution
    const int voicesNumber = 4;

    std::vector<juce::Synthesiser*> synths;

    juce::AudioProcessorValueTreeState::ParameterLayout createParams();


    static std::complex<double> mandelbrot(std::complex<double> z, std::complex<double> c);

    static std::complex<double> burningShip(std::complex<double> z, std::complex<double> c);

    std::complex<double>(*currentFractal)(std::complex<double> z, std::complex<double> c);

    void generateFractalSuccession(std::complex<double>(*fractalFunction)(std::complex<double> z, std::complex<double> c), std::complex<double> c);

    bool updatedFractal = false;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalSynthesisAudioProcessor)
};
