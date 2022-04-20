/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "InputPlane.h"

//==============================================================================
FractalSynthesisAudioProcessorEditor::FractalSynthesisAudioProcessorEditor (FractalSynthesisAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), inputPlaneComponent(initialPointXSlider, initialPointYSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.



    setSize(400, 300);


    fractalFunctionComboBox.addItem("Mandelbrot Set", 1);
    fractalFunctionComboBox.addItem("Burning Ship Set", 2);




    fractalComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FRACTAL_FUNCTION", fractalFunctionComboBox);


    sliderInitialPointXAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_X", initialPointXSlider);


    sliderInitialPointYAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_Y", initialPointYSlider);

    addAndMakeVisible(fractalFunctionComboBox);

    addAndMakeVisible(inputPlaneComponent);

    addAndMakeVisible(initialPointXSlider);

    addAndMakeVisible(initialPointYSlider);



}

FractalSynthesisAudioProcessorEditor::~FractalSynthesisAudioProcessorEditor()
{
}

//==============================================================================
void FractalSynthesisAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void FractalSynthesisAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..


    auto bounds = getLocalBounds().reduced(50);

    fractalFunctionComboBox.setBounds(bounds.removeFromLeft(bounds.getWidth()/2));
    inputPlaneComponent.setBounds(bounds.removeFromTop(bounds.getHeight() / 2));


    initialPointXSlider.setBounds(bounds.removeFromTop(bounds.getHeight() / 2));
    initialPointYSlider.setBounds(bounds);




}

