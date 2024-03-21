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
:mWaveShapers{  std::tanh  }
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
    mWaveShapers.prepare(spec);
    
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

void Distortion::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    currentSample = 0;
    
    for(const auto midiMessage : midiMessages)
    {
        const auto midiEvent = midiMessage.getMessage();
        const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
        render(buffer, currentSample, midiEventSample);
        currentSample = midiEventSample;
    
    }
    
    render(buffer, currentSample, buffer.getNumSamples());
}

void Distortion::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    
    auto* firstChannel = buffer.getWritePointer(0);
    
    for (auto sample = startSample; sample < endSample; ++sample)
    {
        const float temp = mWaveShapers.processSample(firstChannel[sample] * mInputVolume.getGainLinear());

        firstChannel[sample] = mOutputVolume.getGainLinear() * temp * 0.7f;
        
    }
      
    for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
    }
}

