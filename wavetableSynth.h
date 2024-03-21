/*
  ==============================================================================

    wavetableSynth.h
    Created: 25 Apr 2023 2:47:50pm
    Author:  S

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "WavetableOscillator.h"
#include "effect.h"



class WavetableSynth
{
public:
    
    void prepareToPlay(double sampleRate); //sets the sample rate for processing
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&); //member function.  is called from within PluginProcessor’s processBlock().
    void switchOSC1(int index);
    void switchOSC2(int index);
    std::vector<WavetableOscillator> oSC1;
    std::vector<WavetableOscillator> oSC2;
    
    juce::dsp::Gain<float> gain_OSC1;
    juce::dsp::Gain<float> gain_OSC2;
    void setGain(float cgain);
    
private:
   
    static std::vector<float> generateSinWaveTable(); 
    static std::vector<float> generateSawWaveTable();
    static std::vector<float> generatePulWaveTable();
    static std::vector<float> generateTriWaveTable();
    
    static float midiNoteNumberToFrequency(int midiNoteNumber); //MIDI note number to frequency in Hz
    void initializeOscillators(); //depends on OSCILLATORS_COUNT
    void handleMidiEvent(const juce::MidiMessage& midiEvent); //a MIDI message to synth’s parameters change.
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample); //generates samples in the [beginSample, endSample) range (Standard Template Library-style range).
    
    const int OSCILLATORS_COUNT = 16; // number of voice
    double sampleRate;
    int samplesPerBlock;
    std::vector<WavetableOscillator> tempOscillators_1; //contains all oscillators created by initializeOscillators(). To these oscillators, particular notes are assigned.
    std::vector<WavetableOscillator> tempOscillators_2;
    
    float velocity = 0;
    static const int WAVETABLE_LENGTH = 1024; // tableSize, e.g., 256, 512, 1024, 4096;
    std::vector<int> currentNotes;
    std::array<juce::ADSR, 16>  ADSR ; //why private? (2nd. arg - 16 depends on oscl_count)
    int currentSample = 0;
};
