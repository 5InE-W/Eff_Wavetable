/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 9 May 2023 10:18:29am
    Author:  S

  ==============================================================================
*/

#include "WavetableOscillator.h"
#include <cmath>

WavetableOscillator::WavetableOscillator(std::vector<float> waveTable, double sampleRate)
    :waveTable{std::move(waveTable)},
        sampleRate{sampleRate}  
{
    adsr.setSampleRate(sampleRate);
    params.attack = .1;
    params.decay = .9;
    params.sustain = .5;
    params.release = 1.0;
    adsr.setParameters(params);
    adsr.reset();
    
    
}

// ADSR envelope
void WavetableOscillator::setAttack(float attack)
{
    params.attack = attack;
    adsr.setParameters(params);
}

void WavetableOscillator::setDecay(float decay)
{
    params.decay = decay;
    adsr.setParameters(params);
}
void WavetableOscillator::setSustain(float sustain)
{
    params.sustain= sustain;
    adsr.setParameters(params);
}

void WavetableOscillator::setRelease(float release)
{
    params.release = release;
    adsr.setParameters(params);
}


// oscillators

void WavetableOscillator::setFrequency(float frequency)
{
    this->stop();
    indexIncrement = frequency * static_cast<float>(waveTable.size())/ static_cast<float>(sampleRate);
    
}

float WavetableOscillator::getSample()
{
    index = std::fmod(index, static_cast<float>(waveTable.size()));
    const auto sample = interpolateLinearly();
    index += indexIncrement;
    return sample;
}

float WavetableOscillator::interpolateLinearly() const
{
    const auto truncatedIndex = static_cast<int>(index); // index to integer transform. e.g. 100.7 -> 100
    const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(waveTable.size()); // define the next index within the periode
    
    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex); // 100.7 - 100 = 0.7 (the rest)
    const auto truncatedIndexWeight = 1.f - nextIndexWeight; // 0.3
    
    return truncatedIndexWeight * waveTable[truncatedIndex] + nextIndexWeight * waveTable[nextIndex]; // 0.3 * wt[100] + 0.7 * wt[101]
}

void WavetableOscillator::stop()
{
    index = 0.f;
    indexIncrement = 0.f;
}

bool WavetableOscillator::isPlaying() const
{
    if(indexIncrement != 0.f && adsr.isActive())
    {
    return true;
    }
    else
    {
    return false;
    }
}
