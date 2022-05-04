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



    fractalFunctionComboBox.addItem("Mandelbrot Set", 1);
    fractalFunctionComboBox.addItem("Burning Ship Set", 2);
    fractalFunctionComboBox.addItem("Tricorn", 3);



    setSliderStyle(attackSlider);
    setSliderStyle(decaySlider);
    setSliderStyle(sustainSlider);
    setSliderStyle(releaseSlider);


    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.attachToComponent(&attackSlider, false);

    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    decayLabel.attachToComponent(&decaySlider, false);


    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centred);
    sustainLabel.attachToComponent(&sustainSlider, false);


    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.attachToComponent(&releaseSlider, false);



    fractalComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FRACTAL_FUNCTION", fractalFunctionComboBox);


    sliderInitialPointXAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_X", initialPointXSlider);


    sliderInitialPointYAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_Y", initialPointYSlider);
    
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    
    mandelbrotImage = juce::ImageCache::getFromMemory(BinaryData::Mandelbrot2_png, BinaryData::Mandelbrot2_pngSize);
    burningShipImage = juce::ImageCache::getFromMemory(BinaryData::BurningShip2_png, BinaryData::BurningShip2_pngSize);
    tricornImage = juce::ImageCache::getFromMemory(BinaryData::Tricorn2_png, BinaryData::Tricorn2_pngSize);
    

    currentImage = mandelbrotImage;

    fractalFunctionComboBox.addListener(this);
   
    addAndMakeVisible(fractalFunctionComboBox);

    addAndMakeVisible(inputPlaneComponent);

    addAndMakeVisible(initialPointXSlider);

    addAndMakeVisible(initialPointYSlider);
    
    addAndMakeVisible(attackSlider);
    
    addAndMakeVisible(sustainSlider);
    
    addAndMakeVisible(decaySlider);
    
    addAndMakeVisible(releaseSlider);


    setSize(700, 500);

}

FractalSynthesisAudioProcessorEditor::~FractalSynthesisAudioProcessorEditor()
{
}

//==============================================================================
void FractalSynthesisAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setOpacity(0.4);
    g.drawImageWithin(currentImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(),
        juce::RectanglePlacement::fillDestination);


}

void FractalSynthesisAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..


    auto bounds = getLocalBounds().reduced(20);

    auto upperArea = bounds.removeFromTop(bounds.getHeight()/2);
    fractalFunctionComboBox.setBounds(upperArea.removeFromLeft(upperArea.getWidth()/3).reduced(5));
    inputPlaneComponent.setBounds(upperArea.removeFromLeft(upperArea.getWidth()/2).reduced(5));
   
    initialPointXSlider.setBounds(upperArea.removeFromTop(upperArea.getHeight()/2).reduced(5));
    initialPointYSlider.setBounds(upperArea.reduced(5));
    
    auto adsrAreaUpper = bounds.reduced(7);
    auto adsrAreaLower = adsrAreaUpper.removeFromBottom(adsrAreaUpper.getHeight()/2);
    
    
    releaseSlider.setBounds(adsrAreaLower.removeFromRight(adsrAreaLower.getWidth()/2).reduced(7));
    decaySlider.setBounds(adsrAreaLower.reduced(7));
    sustainSlider.setBounds(adsrAreaUpper.removeFromRight(adsrAreaUpper.getWidth()/2).reduced(7));
    attackSlider.setBounds(adsrAreaUpper.reduced(7));

}

void FractalSynthesisAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combo){
    
    /*
        if (combo->getSelectedId()==1)
            mImageComponent.setImage(mandelbrotImage, juce::RectanglePlacement::stretchToFit);
        if (combo->getSelectedId()==2)
            mImageComponent.setImage(burningShipImage, juce::RectanglePlacement::stretchToFit);
        else
            mImageComponent.setImage(tricornImage, juce::RectanglePlacement::stretchToFit);
    */


    if (combo->getSelectedId() == 1)
        currentImage = mandelbrotImage;
    else if (combo->getSelectedId() == 2)
        currentImage = burningShipImage;
    else if(combo->getSelectedId() == 3)
        currentImage = tricornImage;


    repaint();

}


void FractalSynthesisAudioProcessorEditor::setSliderStyle(juce::Slider& slider)
{

    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    slider.setPopupDisplayEnabled(true, true, this);
}