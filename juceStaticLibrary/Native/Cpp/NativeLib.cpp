//
//  NativeLib.cpp
//  juceStaticLibrary
//
//  Created by Charles Culbert on 10/25/24.
//

#include "NativeLib.hpp"

// NativeLib.cpp
void NativeLib::createPlayer() {
    if (audioPlayer == nullptr) {
        audioPlayer = new AudioPlayer();
    }
}

void NativeLib::deletePlayer() {
    if (audioPlayer != nullptr) {
        delete audioPlayer;
        audioPlayer = nullptr;
    }
}

void NativeLib::nativeNoteOn(int midiNote, float velocity) {
    if (audioPlayer != nullptr) {
        audioPlayer->noteOn(midiNote, velocity);
    }
}

void NativeLib::nativeNoteOff(int midiNote) {
    if (audioPlayer != nullptr) {
        audioPlayer->noteOff(midiNote);
    }
}

void NativeLib::nativeSetVolume(float volume) {
    if (audioPlayer != nullptr) {
        audioPlayer->setVolume(volume);
    }
}
