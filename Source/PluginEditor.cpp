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

    setSize(600, 400);


    fractalFunctionComboBox.addItem("Mandelbrot Set", 1);
    fractalFunctionComboBox.addItem("Burning Ship Set", 2);
    



    fractalComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FRACTAL_FUNCTION", fractalFunctionComboBox);


    sliderInitialPointXAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_X", initialPointXSlider);


    sliderInitialPointYAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_Y", initialPointYSlider);
    
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    
    numPartialsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "NUM_PARTIALS", numPartialsSlider);

    mandelbrotImage = juce::ImageCache::getFromMemory(BinaryData::Mandelbrot_png, BinaryData::Mandelbrot_pngSize);
    burningShipImage = juce::ImageCache::getFromMemory(BinaryData::burningShip_png, BinaryData::burningShip_pngSize);
    
    std::cout << audioProcessor.apvts.getParameter("FRACTAL_FUNCTION")->getParameterIndex();
   
    // still have to figure out how to change the image when the combo box changes, for now we show the mandelbrot image
    if (!(mandelbrotImage.isNull() || burningShipImage.isNull())) {
        mImageComponent.setImage(mandelbrotImage, juce::RectanglePlacement::stretchToFit);
    }
    else
        jassert(!(mandelbrotImage.isNull() || burningShipImage.isNull()));

    fractalFunctionComboBox.addListener(this);
    
    
    addAndMakeVisible(mImageComponent);
    
    addAndMakeVisible(fractalFunctionComboBox);

    addAndMakeVisible(inputPlaneComponent);

    addAndMakeVisible(initialPointXSlider);

    addAndMakeVisible(initialPointYSlider);
    
    addAndMakeVisible(attackSlider);
    
    addAndMakeVisible(sustainSlider);
    
    addAndMakeVisible(decaySlider);
    
    addAndMakeVisible(releaseSlider);
    
    addAndMakeVisible(numPartialsSlider);



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


    auto bounds = getLocalBounds().reduced(20);

    auto upperArea = bounds.removeFromTop(bounds.getHeight()/2);
    fractalFunctionComboBox.setBounds(upperArea.removeFromLeft(upperArea.getWidth()/3).reduced(5));
    inputPlaneComponent.setBounds(upperArea.removeFromLeft(upperArea.getWidth()/2).reduced(5));
   
    initialPointXSlider.setBounds(upperArea.removeFromTop(upperArea.getHeight()/2).reduced(5));
    initialPointYSlider.setBounds(upperArea.reduced(5));
    
    auto adsrAreaUpper = bounds.removeFromRight(bounds.getWidth()/2);
    auto adsrAreaLower = adsrAreaUpper.removeFromBottom(adsrAreaUpper.getHeight()/2);
    
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    //attackSlider.setRotaryParameters(this juce::Slider::RotaryHorizontalVerticalDrag));
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    attackSlider.setPopupDisplayEnabled(true, true, this);
    attackSlider.setTextValueSuffix("Attack");


    
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    //sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sustainSlider.getY(),sustainSlider.getX());
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sustainSlider.setPopupDisplayEnabled(true, true, this);
    sustainSlider.setTextValueSuffix("Sustain");
    
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    decaySlider.setPopupDisplayEnabled(true, true, this);
    decaySlider.setTextValueSuffix("Decay");
    
    //decaySlider.setTitle("Decay");
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    releaseSlider.setPopupDisplayEnabled(true, true, this);
    releaseSlider.setTextValueSuffix("Release");
    
    numPartialsSlider.setSliderStyle(juce::Slider::Rotary);
    
    releaseSlider.setBounds(adsrAreaLower.removeFromRight(adsrAreaLower.getWidth()/2).reduced(5));
    decaySlider.setBounds(adsrAreaLower.reduced(5));
    sustainSlider.setBounds(adsrAreaUpper.removeFromRight(adsrAreaUpper.getWidth()/2).reduced(5));
    attackSlider.setBounds(adsrAreaUpper.reduced(5));
    
    numPartialsSlider.setBounds(bounds.removeFromRight(bounds.getWidth()/2).reduced(5));
    
    mImageComponent.setBounds(bounds.reduced(5));

}

void FractalSynthesisAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combo){
    
        if (combo->getSelectedId()==1)
            mImageComponent.setImage(mandelbrotImage, juce::RectanglePlacement::stretchToFit);
        else
            mImageComponent.setImage(burningShipImage, juce::RectanglePlacement::stretchToFit);
    
}
