/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "InputPlane.h"

//==============================================================================
/**
*/
class FractalSynthesisAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ComboBox::Listener
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

    void comboBoxChanged(juce::ComboBox* combo) override;
    
    juce::Image mandelbrotImage;
    juce::Image burningShipImage;
    juce::Image tricornImage;

    //Buttons, combobox, sliders...
    juce::ComboBox fractalFunctionComboBox;

    juce::Slider initialPointXSlider;
    juce::Slider initialPointYSlider;
    
    juce::Slider attackSlider;
    juce::Slider sustainSlider;
    juce::Slider decaySlider;
    juce::Slider releaseSlider;
    
    juce::Slider numPartialsSlider;

    InputPlane inputPlaneComponent;
    
    juce::ImageComponent mImageComponent;

    //Attachments (must be declared after the GUI elements to avoid crashes when closing the plugin)
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fractalComboBoxAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderInitialPointXAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderInitialPointYAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> numPartialsAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalSynthesisAudioProcessorEditor)
};
