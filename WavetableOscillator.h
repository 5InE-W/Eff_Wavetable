/*
  ==============================================================================

    WavetableOscillator.h
    Created: 9 May 2023 10:18:29am
    Author:  S

  ==============================================================================
*/

#pragma once
#include <vector>
#include "JuceHeader.h"
class WavetableOscillator
{
public:
    WavetableOscillator(std::vector<float> waveTable, double sampleRate); // constructor takes the wavetable and the samplingRate and stores them in member variables waveTable[11] and sampleRate [12] respectively using an initializer list.
    WavetableOscillator(const WavetableOscillator&) = delete; //[2] prevent from accidenally copying an oscillator.
    WavetableOscillator& operator=(const WavetableOscillator&) = delete; //[2]
    WavetableOscillator(WavetableOscillator&&) = default; //[3]
    WavetableOscillator& operator=(WavetableOscillator&&) = default; //[3]
    
    void setFrequency(float frequency); //[5]
    float getSample(); //[4] getSample() should return 1 sample of the oscillator and advance the index member variable [9] by indexIncrement member variable [10] which is frequency-dependent. This is the core of the wavetable synthesis algorithm;
    
    void stop(); //[6]
    bool isPlaying() const; //[7]
    juce::ADSR adsr;
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);
    
private:
    float interpolateLinearly() const; //[8]
    float index = 0.f; //[9]
    float indexIncrement = 0.f; //[10]
    std::vector<float> waveTable; //[11]
    double sampleRate; //[12]
    juce::ADSR::Parameters params;
    
};
