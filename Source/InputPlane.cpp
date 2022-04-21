/*
  ==============================================================================

    InputPlane.cpp
    Created: 20 Apr 2022 10:58:06pm
    Author:  Ricky

  ==============================================================================
*/

#include "InputPlane.h"
#include <JuceHeader.h>

InputPlane::InputPlane(juce::Slider& sliderX, juce::Slider& sliderY) : sliderX(sliderX), sliderY(sliderY)
{

    sliderX.addListener(this);
    sliderY.addListener(this);

}

void InputPlane::paint(juce::Graphics& g)
{


    auto area = getLocalBounds();

    g.fillAll(juce::Colour::fromRGB(0, 0, 0));

    g.setColour(juce::Colours::aliceblue);
    g.drawEllipse(sliderX.getValue() * area.getWidth(), (1 - sliderY.getValue()) * area.getHeight(), 5, 5, 1);


}

void InputPlane::resized()
{
}

void InputPlane::mouseDown(const juce::MouseEvent& event)
{

    auto area = getLocalBounds();


    float x = (float)event.getMouseDownX() / area.getWidth();

    float y = 1 - ((float)event.getMouseDownY() / area.getHeight());

    sliderX.setValue(x);
    sliderY.setValue(y);

    repaint();

}

void InputPlane::sliderValueChanged(juce::Slider* slider)
{
    repaint();
}

