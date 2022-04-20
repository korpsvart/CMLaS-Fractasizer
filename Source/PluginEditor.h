/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FractalSynthesisAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FractalSynthesisAudioProcessorEditor (FractalSynthesisAudioProcessor&);
    ~FractalSynthesisAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FractalSynthesisAudioProcessor& audioProcessor;

    
    //Buttons, combobox, sliders...
    juce::ComboBox fractalFunctionComboBox;

    //Attachments (must be declared after the GUI elements to avoid crashes when closing the plugin)
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fractalComboBoxAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalSynthesisAudioProcessorEditor)
};
