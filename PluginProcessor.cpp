/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynth_PluginAudioProcessor::WavetableSynth_PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    
    synth = std::make_unique<WavetableSynth>();
    dist = std::make_unique<Distortion>();
    
    osc1 = treeState.getRawParameterValue("OSC1");
    osc2 = treeState.getRawParameterValue("OSC2");
    treeState.addParameterListener("attack_osc1", this);
    treeState.addParameterListener("decay_osc1", this);
    treeState.addParameterListener("sustian_osc1", this);
    treeState.addParameterListener("release_osc1", this);
    treeState.addParameterListener("attack_osc2", this);
    treeState.addParameterListener("decay_osc2", this);
    treeState.addParameterListener("sustian_osc2", this);
    treeState.addParameterListener("release_osc2", this);
    treeState.addParameterListener("OSC1",this);
    treeState.addParameterListener("OSC2",this);
    treeState.addParameterListener("MIX", this);
    treeState.addParameterListener("DIST",this); // Arg (paramID)
    treeState.addParameterListener("LEVEL",this);
}

WavetableSynth_PluginAudioProcessor::~WavetableSynth_PluginAudioProcessor()
{
    treeState.removeParameterListener("attack_osc1", this);
    treeState.removeParameterListener("decay_osc1", this);
    treeState.removeParameterListener("sustian_osc1", this);
    treeState.removeParameterListener("release_osc1", this);
    treeState.removeParameterListener("OSC1",this);
    treeState.removeParameterListener("attack_osc2", this);
    treeState.removeParameterListener("decay_osc2", this);
    treeState.removeParameterListener("sustian_osc2", this);
    treeState.removeParameterListener("release_osc2", this);
    treeState.removeParameterListener("OSC2",this);
    treeState.removeParameterListener("MIX", this);
    treeState.removeParameterListener("DIST",this);
    treeState.removeParameterListener("LEVEL",this);
}

juce::AudioProcessorValueTreeState::ParameterLayout
WavetableSynth_PluginAudioProcessor::createParameterLayout()
{
    // create parameters
    // you could also use a array with strings and add them in a for loop
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> cparams; //stores value in params vector
    auto pOscType1 = std::make_unique<juce::AudioParameterChoice>("OSC1","OSC TYPE 1",juce::StringArray{"Sine", "Saw", "Pulse", "Tri"},1);
    auto pOscType2 = std::make_unique<juce::AudioParameterChoice>("OSC2","OSC TYPE 2",juce::StringArray{"Sine", "Saw", "Pulse", "Tri"},2);
    auto pOsc1attack = std::make_unique<juce::AudioParameterFloat>("attack_osc1","attack_osc1",0.1,1.0,0.1);
    auto pOsc1decay= std::make_unique<juce::AudioParameterFloat>("decay_osc1","decay_osc1",0.1,1.0,0.5);
    auto pOsc1sustain = std::make_unique<juce::AudioParameterFloat>("sustain_osc1","sustain_osc1",0.1,1.0,0.5);
    auto pOsc1release= std::make_unique<juce::AudioParameterFloat>("release_osc1","release_osc1",0.1,1.0,0.5);
    auto pOsc2attack = std::make_unique<juce::AudioParameterFloat>("attack_osc2","attack_osc2",0.1,1.0,0.5);
    auto pOsc2decay= std::make_unique<juce::AudioParameterFloat>("decay_osc2","decay_osc2",0.1,1.0,0.5);
    auto pOsc2sustain = std::make_unique<juce::AudioParameterFloat>("sustain_osc2","sustain_osc2",0.1,1.0,0.5);
    auto pOsc2release= std::make_unique<juce::AudioParameterFloat>("release_osc2","release_osc2",0.1,1.0,0.5);
    auto pMix= std::make_unique<juce::AudioParameterFloat>("MIX","MIX",0,1.0,0.5);
    
    //distortion
    auto pdist = std::make_unique<juce::AudioParameterFloat>("DIST","DIST",0.f,60.f,1.f);
    auto plevel = std::make_unique<juce::AudioParameterFloat>("LEVEL","LEVEL",-40.f,1.f,-20.f);
   
    
    cparams.push_back(std::move(pOscType1)); 
    cparams.push_back(std::move(pOscType2));
    cparams.push_back(std::move(pOsc1attack));
    cparams.push_back(std::move(pOsc1decay));
    cparams.push_back(std::move(pOsc1sustain));
    cparams.push_back(std::move(pOsc1release));
    cparams.push_back(std::move(pOsc2attack));
    cparams.push_back(std::move(pOsc2decay));
    cparams.push_back(std::move(pOsc2sustain));
    cparams.push_back(std::move(pOsc2release));
    cparams.push_back(std::move(pMix));
    cparams.push_back(std::move(pdist));
    cparams.push_back(std::move(plevel));
   
    
    return {cparams.begin(),cparams.end()};
}





//==============================================================================
const juce::String WavetableSynth_PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WavetableSynth_PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynth_PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynth_PluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WavetableSynth_PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WavetableSynth_PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WavetableSynth_PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WavetableSynth_PluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WavetableSynth_PluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void WavetableSynth_PluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WavetableSynth_PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth->prepareToPlay(sampleRate);
    synth->switchOSC1(*osc1);
    synth->switchOSC2(*osc2);
    
    //prepare distortion
    // initialize the value for synth
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    dist->prepare(spec);
  
}

void WavetableSynth_PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void WavetableSynth_PluginAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if(parameterID == "attack_osc1")
    {
        for(auto& osc : synth->oSC1)
            osc.setAttack(newValue);
    }
    if(parameterID == "decay_osc1")
    {
        for(auto& osc : synth->oSC1)
            osc.setDecay(newValue);
    }
    
    if(parameterID == "sustain_osc1")
    {
        for(auto& osc : synth->oSC1)
            osc.setSustain(newValue);
    }
    
    if(parameterID == "release_osc1")
    {
        for(auto& osc : synth->oSC1)
            osc.setRelease(newValue);
    }
    
    if(parameterID == "OSC1")
    {
        synth->switchOSC1(newValue);
    }
    
    if(parameterID == "attack_osc2")
    {
        for(auto& osc : synth->oSC2)
            osc.setAttack(newValue);
    }
    if(parameterID == "decay_osc2")
    {
        for(auto& osc : synth->oSC2)
            osc.setDecay(newValue);
    }
        
    if(parameterID == "sustain_osc2")
    {
        for(auto& osc : synth->oSC2)
            osc.setSustain(newValue);
    }
        
    if(parameterID == "release_osc2")
    {
        for(auto& osc : synth->oSC2)
            osc.setRelease(newValue);
    }
        
    if(parameterID == "OSC2")
    {
        synth->switchOSC2(newValue);
    }
    
    if(parameterID == "MIX")
    {
        synth->setGain(newValue);
    }
    
     if(parameterID == "DIST")
    {
        dist->setDist(newValue);
    }
    
    if(parameterID == "LEVEL")
   {
       dist->setLevel(newValue);
   }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WavetableSynth_PluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WavetableSynth_PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    for (auto i = 0; i < buffer.getNumChannels(); ++i)
           buffer.clear(i, 0, buffer.getNumSamples()
        );
    synth->processBlock(buffer, midiMessages);
    dist->processBlock(buffer, midiMessages);

}

//==============================================================================
bool WavetableSynth_PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WavetableSynth_PluginAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this); //new WavetableSynth_PluginAudioProcessorEditor (*this);
}

//==============================================================================
void WavetableSynth_PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
                         treeState.state.writeToStream(mos);
}

void WavetableSynth_PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
                              if(tree.isValid() )
                              {
                                  treeState.replaceState(tree);
                              }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavetableSynth_PluginAudioProcessor();
}
