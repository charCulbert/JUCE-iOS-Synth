//
//  NativeLib.hpp
//  juceStaticLibrary
//
//  Created by Charles Culbert on 10/25/24.
//


#ifndef NativeLib_hpp
#define NativeLib_hpp

#include <stdio.h>
#include <string>
#include "AudioPlayer.hpp"  // Add this since NativeLib uses AudioPlayer


// NativeLib.hpp
class NativeLib {
public:
    void createPlayer();
    void deletePlayer();
    void nativeNoteOn(int midiNote, float velocity);
    void nativeNoteOff(int midiNote);
    void nativeSetVolume(float volume);

private:
    AudioPlayer* audioPlayer = nullptr;
};
#endif /* NativeLib_hpp */
