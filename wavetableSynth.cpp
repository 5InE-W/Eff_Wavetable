/*
  ==============================================================================

    wavetableSynth.cpp
    Created: 25 Apr 2023 2:47:50pm
    Author:  S

  ==============================================================================
*/

#include "wavetableSynth.h"

// generate wave forms
std::vector<float> WavetableSynth::generateSinWaveTable()
{
    
    const auto PI = std::atanf(1.f) * 4;
    std::vector<float> sineWaveTable = std::vector<float>(WAVETABLE_LENGTH);
    
    for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
    {
        sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i)/static_cast<float>(WAVETABLE_LENGTH));
    }
    return sineWaveTable;
}

std::vector<float> WavetableSynth::generateSawWaveTable()
{
    
    std::vector<float> sawWaveTable(WAVETABLE_LENGTH);
    
    for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
    {
        sawWaveTable[i] = static_cast<float>(i)/static_cast<float>(WAVETABLE_LENGTH);
                        
    }
    return sawWaveTable;
}

std::vector<float> WavetableSynth::generatePulWaveTable()
{
    
    std::vector<float> pulWaveTable(WAVETABLE_LENGTH);
    
    int halfsize = (WAVETABLE_LENGTH/2) - 1;
    for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
    {
        if (i < halfsize)
        {
            pulWaveTable[i] = 1.0f;
        } else
        {
            pulWaveTable[i] = -1.0f;
        }
                        
    }
    return pulWaveTable;
}

std::vector<float> WavetableSynth::generateTriWaveTable()
{
     
    std::vector<float> triWaveTable(WAVETABLE_LENGTH);
    float increment = 1/ static_cast<float>(WAVETABLE_LENGTH); // steigung
    int halfsize = (WAVETABLE_LENGTH/ 2) - 1;
    float temp = 0;
    for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
    {
        if (i < halfsize)
        {
            temp += increment;
            triWaveTable[i] = temp;
        } else
        {
            temp -= increment;
            triWaveTable[i] = temp;
        }
                        
    }
    return triWaveTable;
}

// initialize the Oscillators (for 16 voices)
void WavetableSynth::initializeOscillators()
{
    
    currentNotes.clear();
    

    for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
    {
        currentNotes.push_back(-1); // list of notes, to be able to trigger note on/off.
    }
    
    
}

// output between OSC 1 and 2.
void WavetableSynth::setGain(float cgain)
{
    
    gain_OSC2.setGainLinear(cgain);
    gain_OSC1.setGainLinear(1.f - cgain);
    
    
}

// switch waveforms within OSC1.
void WavetableSynth::switchOSC1(int index)
{
    if(index == 0)
    {
        const auto waveTable = generateSinWaveTable();
        tempOscillators_1.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_1.emplace_back(waveTable,this->sampleRate);
        }
        oSC1.swap(tempOscillators_1);
    }
    else if(index == 1)
    {
        const auto waveTable = generateSawWaveTable();
        tempOscillators_1.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_1.emplace_back(waveTable,this->sampleRate);
        }
        oSC1.swap(tempOscillators_1);
    }
    else if(index == 2)
    {
        const auto waveTable = generatePulWaveTable();
        tempOscillators_1.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_1.emplace_back(waveTable,this->sampleRate);
        }
        oSC1.swap(tempOscillators_1);
    }
    else if(index == 3)
    {
        const auto waveTable = generateTriWaveTable();
        tempOscillators_1.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_1.emplace_back(waveTable,this->sampleRate);
        }
        oSC1.swap(tempOscillators_1);
    }
}


// switch waveforms within OSC2.
void WavetableSynth::switchOSC2(int index)
{
    if(index == 0)
    {
        const auto waveTable = generateSinWaveTable();
        tempOscillators_2.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_2.emplace_back(waveTable,this->sampleRate);
        }
        oSC2.swap(tempOscillators_2);
    }
    else if(index == 1)
    {
        const auto waveTable = generateSawWaveTable();
        tempOscillators_2.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_2.emplace_back(waveTable,this->sampleRate);
        }
        oSC2.swap(tempOscillators_2);
    }
    else if(index == 2)
    {
        const auto waveTable = generatePulWaveTable();
        tempOscillators_2.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_2.emplace_back(waveTable,this->sampleRate);
        }
        oSC2.swap(tempOscillators_2);
    }
    else if(index == 3)
    {
        const auto waveTable = generateTriWaveTable();
        tempOscillators_2.clear();
        for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
        {
            tempOscillators_2.emplace_back(waveTable,this->sampleRate);
        }
        oSC2.swap(tempOscillators_2);
    }
}

//configure our synthesizer after plugin start or after a major settings change
void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = 1;
    gain_OSC1.prepare(spec);
    gain_OSC2.prepare(spec);
    initializeOscillators();
}



void WavetableSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //AudioBuffer object contains audio samples of the current block
    //MidiBuffer object contains MIDI events that happened during that block
    
    currentSample = 0;
    
    for(const auto midiMessage : midiMessages)
    {
        const auto midiEvent = midiMessage.getMessage();
        const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
        
        render(buffer, currentSample, midiEventSample);
        handleMidiEvent(midiEvent);
        
        currentSample = midiEventSample;
    
    }
    
    this->render(buffer, currentSample, buffer.getNumSamples());
}


void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    float* firstChannel = buffer.getWritePointer(0);
    
    for (auto& osc : oSC1)
    {
        if (osc.isPlaying())
        {
            for (auto sample = startSample; sample < endSample; ++sample)
            {
                const float temp = gain_OSC1.processSample(osc.getSample() * osc.adsr.getNextSample() * velocity);

                firstChannel[sample] += temp;
              
            }
        }
                                          
    }
    for (auto& osc : oSC2)
    {
        if (osc.isPlaying())
        {
            for (auto sample = startSample; sample < endSample; ++sample)
            {
                const float temp = gain_OSC2.processSample((osc.getSample() * osc.adsr.getNextSample() * velocity));
                firstChannel[sample] += temp;
            }
        }
    }
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    if (midiEvent.isNoteOn())
    {
        for(int i = 0; i < OSCILLATORS_COUNT; i++)
        {
            if(!oSC1[i].isPlaying() && !oSC2[i].isPlaying())
            {
                currentNotes[i] = midiEvent.getNoteNumber();
                velocity = midiEvent.getVelocity() / 127.f;
                const auto frequency = midiNoteNumberToFrequency(midiEvent.getNoteNumber());
                oSC1[i].setFrequency(frequency);
                oSC2[i].setFrequency(frequency);
                oSC1[i].adsr.noteOn();
                oSC2[i].adsr.noteOn();
                break;
            }
        }
    }
    else if (midiEvent.isNoteOff())
    {
        
        for(int i = 0; i < OSCILLATORS_COUNT; i++)
        {
            const auto midiNote = midiEvent.getNoteNumber();
            if(currentNotes[i] == midiNote)
            {
                oSC1[i].adsr.noteOff();
                oSC2[i].adsr.noteOff();
                currentNotes[i] = -1;
               
                break;
               
            }
        }
    }
   
}
    

float WavetableSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
    constexpr auto A4_FREQUENCY = 440.f;
    constexpr auto A4_NOTE_NUMBER = 69.f;
    constexpr auto SEMITONES_IN_AN_OCTAVE = 12.f;
    return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER)/SEMITONES_IN_AN_OCTAVE);
}
