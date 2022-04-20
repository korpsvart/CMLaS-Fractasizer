/*
  ==============================================================================

    InputPlane.h
    Created: 20 Apr 2022 10:57:42pm
    Author:  Ricky

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class InputPlane : public juce::Component
{
public:
    InputPlane(juce::Slider& sliderX, juce::Slider& sliderY);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;

private:

    juce::Slider& sliderX;
    juce::Slider& sliderY;

    float currentX = 0.5f;
    float currentY = 0.5f;



};
