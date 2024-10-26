//
//  AudioPlayer.hpp
//  juceStaticLibrary
//
//  Created by Charles Culbert on 10/26/24.
//



// AudioPlayer.hpp
#ifndef AudioPlayer_hpp
#define AudioPlayer_hpp

#include "Juce/AudioSamplePlayerListener.h"
#include "JUCE/AudioSamplePlayer.h"
#include <JuceHeader.h>

// Forward declarations of our classes
class SineWaveSound;
class SineWaveVoice;

class AudioPlayer : public juceStaticLibraryNamespace::AudioSamplePlayerListener {
public:
    AudioPlayer();
    virtual ~AudioPlayer();

    void prepareToPlay(int samplesPerBlock, double sampleRate) override;
    void releaseResources() override;
    float* getNextAudioSamples(int numberOfFrames, int numberOfOutputChannels) override;

    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);
    void setVolume(float volume);
    
    void setup() {
        debugPrint("Setting up audio device");
        audioSamplePlayer.setListener(this);
        audioSamplePlayer.setupPlayer();
    }
    
    void stop() {
        debugPrint("Stopping audio device");
        audioSamplePlayer.closePlayer();
    }

private:
    std::unique_ptr<juce::Synthesiser> synth;
    double sampleRate = 44100.0;
    float masterVolume = 1.0f;
    juceStaticLibraryNamespace::AudioSamplePlayer audioSamplePlayer;
    bool isInitialized = false;
    
    void debugPrint(const char* msg);
};

// Define the synth classes
class SineWaveSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class SineWaveVoice : public juce::SynthesiserVoice {
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override {
        return dynamic_cast<SineWaveSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    }

    void stopNote(float, bool allowTailOff) override {
        if (allowTailOff) {
            if (tailOff == 0.0) tailOff = 1.0;
        } else {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        if (angleDelta != 0.0) {
            if (tailOff > 0.0) {
                while (--numSamples >= 0) {
                    auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                    tailOff *= 0.99;

                    if (tailOff <= 0.005) {
                        clearCurrentNote();
                        angleDelta = 0.0;
                        break;
                    }
                }
            } else {
                while (--numSamples >= 0) {
                    auto currentSample = (float)(std::sin(currentAngle) * level);
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};

#endif /* AudioPlayer_hpp */
