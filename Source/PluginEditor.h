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


    /*

    //Osc 0
    juce::Slider attackSlider0;
    juce::Slider sustainSlider0;
    juce::Slider decaySlider0;
    juce::Slider releaseSlider0;
    juce::ComboBox waveTypeComboBox0;

    juce::Label attackLabel0;
    juce::Label decayLabel0;
    juce::Label sustainLabel0;
    juce::Label releaseLabel0;
    juce::Label waveTypeLabel0;
    

    //Osc 1
    juce::Slider attackSlider1;
    juce::Slider sustainSlider1;
    juce::Slider decaySlider1;
    juce::Slider releaseSlider1;
    juce::ComboBox waveTypeComboBox1;

    juce::Label attackLabel1;
    juce::Label decayLabel1;
    juce::Label sustainLabel1;
    juce::Label releaseLabel1;
    juce::Label waveTypeLabel1;

    */




    InputPlane inputPlaneComponent;

    //Attachments (must be declared after the GUI elements to avoid crashes when closing the plugin)
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fractalComboBoxAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderInitialPointXAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderInitialPointYAttachment;


    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> attackAttachments;

    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> decayAttachments;

    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> sustainAttachments;

    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> releaseAttachments;
    
    /*
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalSynthesisAudioProcessorEditor)
};
