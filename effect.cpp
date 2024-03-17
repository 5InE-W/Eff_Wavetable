/*
  ==============================================================================

    effect.cpp
    Created: 16 Mar 2024 7:19:43pm
    Author:  S

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "effect.h"

//==============================================================================

Distortion::Distortion()
    :mWaveShapers{ { std::tanh } }
{
    
}



Distortion::~Distortion()
{
    // Empty destructor
}

void Distortion::prepare(juce::dsp::ProcessSpec spec)
{
   
    mInputVolume.prepare(spec);
    mOutputVolume.prepare(spec);
    
    reset();
}

void Distortion::setDist(float cdist)
{
    mInputVolume.setGainDecibels(cdist);
}

void Distortion::setLevel(float clevel)
{
    mOutputVolume.setGainDecibels(clevel);
}


void Distortion::reset()
{
}

void Distortion::processBlock(juce::AudioBuffer<float>& buffer)
{
    currentSample = 0;
    render(buffer, currentSample, buffer.getNumSamples());
}



void Distortion::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    
    auto* firstChannel = buffer.getWritePointer(0);
    
    for (auto sample = startSample; sample < endSample; ++sample)
    {
        const float temp = mWaveShapers[0].processSample(firstChannel[sample] * mInputVolume.getGainLinear());

        firstChannel[sample] = mOutputVolume.getGainLinear() * temp * 0.7f;
        
    }
      
    
}


