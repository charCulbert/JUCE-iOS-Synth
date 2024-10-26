

// AudioPlayer.cpp
//  juceStaticLibrary
//
//  Created by Charles Culbert on 10/26/24.
//

// AudioPlayer.cpp
#include "AudioPlayer.hpp"
#include <iostream>

void AudioPlayer::debugPrint(const char* msg) {
    std::cout << "AudioPlayer: " << msg << std::endl;
}

AudioPlayer::AudioPlayer() : synth(std::make_unique<juce::Synthesiser>()) {
    debugPrint("Constructor called");
    
    // Initialize synth
    for (int i = 0; i < 4; ++i) {
        synth->addVoice(new SineWaveVoice());
    }
    synth->addSound(new SineWaveSound());
    
    debugPrint("Synth initialized with voices and sound");
}

AudioPlayer::~AudioPlayer() {
    debugPrint("Destructor called");
}

void AudioPlayer::prepareToPlay(int samplesPerBlock, double newSampleRate) {
    debugPrint("prepareToPlay called");
    sampleRate = newSampleRate;
    synth->setCurrentPlaybackSampleRate(sampleRate);
}

void AudioPlayer::releaseResources() {
    debugPrint("releaseResources called");
}

float* AudioPlayer::getNextAudioSamples(int numberOfFrames, int numberOfOutputChannels) {
    auto* returnBuffer = new float[numberOfFrames * numberOfOutputChannels];
    
    // Create temporary JUCE buffer
    juce::AudioBuffer<float> tempBuffer(numberOfOutputChannels, numberOfFrames);
    tempBuffer.clear();
    
    // Let synth render into our buffer
    juce::MidiBuffer tempMidi;
    synth->renderNextBlock(tempBuffer, tempMidi, 0, numberOfFrames);
    
    // Debug print some values
    float maxValue = 0.0f;
    
    // Copy and apply volume
    for (int channel = 0; channel < numberOfOutputChannels; ++channel) {
        const float* channelData = tempBuffer.getReadPointer(channel);
        for (int frame = 0; frame < numberOfFrames; ++frame) {
            float value = channelData[frame] * masterVolume;
            returnBuffer[frame * numberOfOutputChannels + channel] = value;
            maxValue = std::max(maxValue, std::abs(value));
        }
    }
    
    // Debug print if we're actually generating signal
    if (maxValue > 0.0f) {
        debugPrint("Generating audio signal");
    }
    
    return returnBuffer;
}

void AudioPlayer::noteOn(int midiNote, float velocity) {
    debugPrint("noteOn called");
    if (!isInitialized) {
        setup();
        isInitialized = true;
    }
    synth->noteOn(1, midiNote, velocity);
}

void AudioPlayer::noteOff(int midiNote) {
    debugPrint("noteOff called");
    synth->noteOff(1, midiNote, 1.0f, true);
}

void AudioPlayer::setVolume(float volume) {
    debugPrint("setVolume called");
    masterVolume = volume;
}
