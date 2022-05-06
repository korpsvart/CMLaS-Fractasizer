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




    for (size_t i = 0; i < 4; i++)
    {

        attackSliders.add(new juce::Slider());
        decaySliders.add(new juce::Slider());
        sustainSliders.add(new juce::Slider());
        releaseSliders.add(new juce::Slider());
        waveTypeComboBoxes.add(new juce::ComboBox());

        attackLabels.add(new juce::Label());
        decayLabels.add(new juce::Label());
        sustainLabels.add(new juce::Label());
        releaseLabels.add(new juce::Label());
        waveTypeLabels.add(new juce::Label());

        setSliderStyle(attackSliders[i]);
        setSliderStyle(decaySliders[i]);
        setSliderStyle(sustainSliders[i]);
        setSliderStyle(releaseSliders[i]);


        attackLabels[i]->setText("Attack", juce::dontSendNotification);
        attackLabels[i]->setJustificationType(juce::Justification::centred);
        attackLabels[i]->attachToComponent(attackSliders[i], false);

        decayLabels[i]->setText("Decay", juce::dontSendNotification);
        decayLabels[i]->setJustificationType(juce::Justification::centred);
        decayLabels[i]->attachToComponent(decaySliders[i], false);


        sustainLabels[i]->setText("Sustain", juce::dontSendNotification);
        sustainLabels[i]->setJustificationType(juce::Justification::centred);
        sustainLabels[i]->attachToComponent(sustainSliders[i], false);


        releaseLabels[i]->setText("Release", juce::dontSendNotification);
        releaseLabels[i]->setJustificationType(juce::Justification::centred);
        releaseLabels[i]->attachToComponent(releaseSliders[i], false);


        attackAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", *attackSliders[i]);

        sustainAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", *sustainSliders[i]);

        decayAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", *decaySliders[i]);

        releaseAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", *releaseSliders[i]);


    }





    fractalComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FRACTAL_FUNCTION", fractalFunctionComboBox);


    sliderInitialPointXAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_X", initialPointXSlider);


    sliderInitialPointYAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INITIAL_POINT_Y", initialPointYSlider);
    

    mandelbrotImage = juce::ImageCache::getFromMemory(BinaryData::Mandelbrot2_png, BinaryData::Mandelbrot2_pngSize);
    burningShipImage = juce::ImageCache::getFromMemory(BinaryData::BurningShip2_png, BinaryData::BurningShip2_pngSize);
    tricornImage = juce::ImageCache::getFromMemory(BinaryData::Tricorn2_png, BinaryData::Tricorn2_pngSize);
    

    currentImage = mandelbrotImage;

    fractalFunctionComboBox.addListener(this);
   
    addAndMakeVisible(fractalFunctionComboBox);

    addAndMakeVisible(inputPlaneComponent);

    addAndMakeVisible(initialPointXSlider);

    addAndMakeVisible(initialPointYSlider);


    for (size_t i = 0; i < 4; i++)
    {
        addAndMakeVisible(attackSliders[i]);

        addAndMakeVisible(sustainSliders[i]);

        addAndMakeVisible(decaySliders[i]);

        addAndMakeVisible(releaseSliders[i]);

        addAndMakeVisible(waveTypeComboBoxes[i]);
    }


    setSize(900, 900);

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



    g.setColour(juce::Colours::floralwhite);
    g.setOpacity(0.8);
    g.drawRoundedRectangle(osc1Area.toFloat().reduced(15), 5.0f, 4.0f);
    g.drawText("OSC 1", osc1Area.toFloat().removeFromTop(10), juce::Justification::left);

    g.drawRoundedRectangle(osc2Area.toFloat().reduced(15), 5.0f, 4.0f);
    g.drawText("OSC 2", osc2Area.toFloat().removeFromTop(10), juce::Justification::left);

    g.drawRoundedRectangle(osc3Area.toFloat().reduced(15), 5.0f, 4.0f);
    g.drawText("OSC 3", osc3Area.toFloat().removeFromTop(10), juce::Justification::left);

    g.drawRoundedRectangle(osc4Area.toFloat().reduced(15), 5.0f, 4.0f);
    g.drawText("OSC 4", osc4Area.toFloat().removeFromTop(10), juce::Justification::left);



}

void FractalSynthesisAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds().reduced(20);

    auto oscArea = bounds.removeFromTop(bounds.getHeight() / 1.5);
    auto upperOscArea = oscArea.removeFromTop(oscArea.getHeight() / 2);
    auto lowerOscArea = oscArea;
    osc1Area = upperOscArea.removeFromLeft(upperOscArea.getWidth() / 2);
    osc2Area = upperOscArea;
    osc3Area = lowerOscArea.removeFromLeft(lowerOscArea.getWidth() / 2);
    osc4Area = lowerOscArea;
    fractalArea = bounds;


    buildOscSubArea(0, osc1Area);
    buildOscSubArea(1, osc2Area);
    buildOscSubArea(2, osc3Area);
    buildOscSubArea(3, osc4Area);

    fractalFunctionComboBox.setBounds(fractalArea.removeFromLeft(fractalArea.getWidth()/3).reduced(5));
    inputPlaneComponent.setBounds(fractalArea.removeFromLeft(fractalArea.getWidth()/2).reduced(5));
   
    initialPointXSlider.setBounds(fractalArea.removeFromTop(fractalArea.getHeight()/2).reduced(5));
    initialPointYSlider.setBounds(fractalArea.reduced(5));
    
 

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


void FractalSynthesisAudioProcessorEditor::setSliderStyle(juce::Slider* slider)
{

    slider->setSliderStyle(juce::Slider::Rotary);
    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    slider->setPopupDisplayEnabled(true, true, this);
    slider->setTextValueSuffix(" seconds");

}

void FractalSynthesisAudioProcessorEditor::buildOscSubArea(int index, juce::Rectangle<int> bounds)
{

    auto tempBounds = juce::Rectangle<int>(bounds); //create a copy so we don't modify the variable (needed by the paint method to draw the rectangle)
    tempBounds = tempBounds.reduced(40);
    auto oscADSRArea = tempBounds.removeFromLeft(tempBounds.getWidth() / 2);
    auto oscWaveTypeArea = tempBounds.removeFromTop(tempBounds.getHeight() / 2);
    auto oscWaveVisualizerArea = tempBounds;
    auto oscADArea = oscADSRArea.removeFromTop(oscADSRArea.getHeight() / 2);
    auto oscSRArea = oscADSRArea;

    attackSliders[index]->setBounds(oscADArea.removeFromLeft(oscADArea.getWidth() / 2));
    decaySliders[index]->setBounds(oscADArea);
    sustainSliders[index]->setBounds(oscSRArea.removeFromLeft(oscSRArea.getWidth() / 2));
    releaseSliders[index]->setBounds(oscSRArea);

    waveTypeComboBoxes[index]->setBounds(oscWaveTypeArea);
}
