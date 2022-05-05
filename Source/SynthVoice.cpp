/*
  ==============================================================================

    SynthVoice.cpp
    Created: 10 Apr 2022 8:11:10pm
    Author:  Ricky

  ==============================================================================
*/

#include "SynthVoice.h"
#include "PluginProcessor.h"


SynthVoice::SynthVoice(int numPartials)
{

    this->numPartials = numPartials;

    for (size_t i = 0; i < numPartials; i++)
    {
        processorChains.push_back(juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>, juce::dsp::Panner<float>>{});
    }
    for (size_t i = 0; i < numPartials; i++)
    {
        lfos.push_back(juce::dsp::Oscillator<float>{ [](float x) { return std::sin(x); }, 128 });
    }

    for (int i = 0; i < numPartials; ++i)
    {

        processorChains[i].get<oscIndex>().initialise([](float x) { return std::sin(x); });
        processorChains[i].get<gainIndex>().setGainLinear(0.5f); //just a default value

        lfos[i].setFrequency(3); //just a default value
    }


    for (int i = 0; i < numPartials; ++i)
    {

        detuneFactors.push_back(i + 1);
    }

    for (size_t i = 0; i < numPartials; i++)
    {
        lfoRates.push_back(3.0f);
    }

    for (size_t i = 0; i < numPartials; i++)
    {
        lfoDepths.push_back(0.5f);
    }



}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    //osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber)*harmonicN);


    auto freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);


    for (int i = 0; i < numPartials; ++i)
    {
        processorChains[i].get<oscIndex>().setFrequency(freq * detuneFactors[i]);
    }

    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

    jassert(isPrepared);


    if (!isVoiceActive())
        return;


    //Set the size for the local temp buffer (only re-allocate if needed)
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    //clear the local temp buffer
    synthBuffer.clear();




    //AudioBlock it's just an alias for the given buffer needed by DSP classes (so by modifying
    //the AudioBlock we are actually modifying the given buffer)
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

    //osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    //panner.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    for (size_t pos = 0; pos < (size_t) numSamples;)
            {
                auto max = juce::jmin ((size_t) numSamples - pos, lfoUpdateCounter);
                auto block = audioBlock.getSubBlock (pos, max);
     
                juce::dsp::ProcessContextReplacing<float> context (block);

                //gain.process (context);

                for (int i = 0; i < numPartials; ++i)
                {
                    processorChains[i].process(context);
                }
                
                pos += max;
                lfoUpdateCounter -= max;

                if (lfoUpdateCounter == 0)
                {
                    lfoUpdateCounter = lfoUpdateRate;
                    for (int i = 0; i < numPartials; ++i)
                    {
                        applyLFO(i);
                    }
                }
            }


        
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    //Add the local temp buffer to the final audio output buffer

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

    }

    if (!adsr.isActive())
        clearCurrentNote();


}

void SynthVoice::applyLFO(int i)
{
    auto lfoOut = lfos[i].processSample(0.0f);
    auto depth = fixedGain * lfoDepths[i];
    auto gainVariation = juce::jmap(lfoOut, -1.0f, 1.0f, fixedGain - depth, fixedGain + depth);
    processorChains[i].get<gainIndex>().setGainLinear(gainVariation);
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannelsNumber)
{

    //Prepare oscillator (passing ProcessSpec)

    adsr.setSampleRate(sampleRate);

    //gain.setGainLinear(0.3f);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannelsNumber;



    for (int i = 0; i < numPartials; ++i)
    {
        processorChains[i].prepare(spec);
    }


    //osc.prepare(spec);
    for (int i = 0; i < numPartials; i++)
    {
        lfos[i].
            prepare({ spec.sampleRate / lfoUpdateRate, spec.maximumBlockSize, spec.numChannels });
    }
    //gain.prepare(spec);
    //panner.prepare(spec);


    isPrepared = true;
}


void SynthVoice::setHarmonicN(float harmonicN)
{

    this->harmonicN = harmonicN;
}

void SynthVoice::setGain(float gainValue)
{

    //gain.setGainLinear(gainValue);
    //processorChain.get<gainIndex>().setGainLinear(gainValue);
}

void SynthVoice::setPan(float panValue)
{
    //panner.setPan(panValue);

    for (int i = 0; i < numPartials; ++i)
    {
        processorChains[i].get<panIndex>().setPan(panValue);
    }
}

void SynthVoice::setFreqDetunes(const std::vector<double>& freqDetunes)
{

    for (int i = 0; i < numPartials; ++i)
    {
        detuneFactors[i] = freqDetunes[i];
    }

}

void SynthVoice::setLFORates(const std::vector<double>& lfoRates)
{
    for (size_t i = 0; i < numPartials; i++)
    {
        this->lfoRates[i] = lfoRates[i];
        lfos[i].setFrequency(lfoRates[i]);
    }

}

void SynthVoice::setLFODepths(const std::vector<double>& lfoDepths)
{
    for (size_t i = 0; i < numPartials; i++)
    {
        this->lfoDepths[i] = lfoDepths[i];
    }
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    adsr.setParameters(adsrParams);
}

void SynthVoice::setWaveType(const int choice)
{

    /*

    switch (choice)
    {
    case 0: //Sin wave
        osc.initialise([](float x) { return std::sin(x); });
        break;
    case 1: //Saw wave
        osc.initialise([](float x) { return x / juce::MathConstants<float>::pi; });
        break;
    case 2: //Square wave
        osc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    }

    */
}


