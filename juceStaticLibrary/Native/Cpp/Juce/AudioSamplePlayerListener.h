/*
  ==============================================================================

    AudioSamplePlayerListener.h
    Created: 25 Oct 2024 11:02:32pm
    Author:  Charles Culbert

  ==============================================================================
*/

#pragma once

namespace juceStaticLibraryNamespace
{
class AudioSamplePlayerListener
{
    protected:
    AudioSamplePlayerListener() = default;
    ~AudioSamplePlayerListener() = default;
    public:
    virtual void prepareToPlay(int samplesPerBlock, double sampleRate) = 0;
    virtual void releaseResources() = 0;
    virtual float* getNextAudioSamples(int numberOfFrames, int NumberOfOutputChannels) = 0;
};
}
