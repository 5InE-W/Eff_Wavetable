/*
  ==============================================================================

    effect.h
    Created: 16 Mar 2024 7:19:43pm
    Author:  S

  ==============================================================================
*/

#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Distortion
{
public:
    Distortion();
    ~Distortion();
    void prepare(juce::dsp::ProcessSpec spec);
    void reset();
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void updateParameters();
    void setDist(float cdist); //input
    void setLevel(float clevel); //output

    
private:
   
    static constexpr size_t numWaveShapers = 1;
    juce::dsp::WaveShaper<float> mWaveShapers; 

    juce::dsp::Gain<float> mInputVolume, mOutputVolume;
    
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);

    int currentSample = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distortion)
};
