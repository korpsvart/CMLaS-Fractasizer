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

    void setSliderStyle(juce::Slider* slider);

    void buildOscSubArea(int index, juce::Rectangle<int> bounds);
    
    juce::Image mandelbrotImage;
    juce::Image burningShipImage;
    juce::Image tricornImage;

    juce::Image currentImage;

    //Buttons, combobox, sliders...
    juce::ComboBox fractalFunctionComboBox;

    juce::Slider initialPointXSlider;
    juce::Slider initialPointYSlider;


    juce::OwnedArray<juce::Slider> attackSliders;

    juce::OwnedArray<juce::Slider> sustainSliders;

    juce::OwnedArray<juce::Slider> releaseSliders;

    juce::OwnedArray<juce::Slider> decaySliders;


    juce::OwnedArray<juce::Label> attackLabels;

    juce::OwnedArray<juce::Label> sustainLabels;

    juce::OwnedArray<juce::Label> releaseLabels;

    juce::OwnedArray<juce::Label> decayLabels;

    juce::OwnedArray<juce::Label> waveTypeLabels;

    juce::OwnedArray<juce::ComboBox> waveTypeComboBoxes;

    //juce::OwnedArray<juce::ComboBox> waveVisualizerBoxes;


    InputPlane inputPlaneComponent;

    //Attachments (must be declared after the GUI elements to avoid crashes when closing the plugin)
   
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fractalComboBoxAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderInitialPointXAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderInitialPointYAttachment;


    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> attackAttachments;

    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> decayAttachments;

    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> sustainAttachments;

    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> releaseAttachments;

    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>, 4> waveTypeAttachments;
    
    /*
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    */

    juce::Rectangle<int> osc1Area;
    juce::Rectangle<int> osc2Area;
    juce::Rectangle<int> osc3Area;
    juce::Rectangle<int> osc4Area;
    juce::Rectangle<int> fractalArea;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalSynthesisAudioProcessorEditor)
};


