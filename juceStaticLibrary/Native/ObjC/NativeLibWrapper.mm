//
//  NativeLibWrapper.mm
//  juceStaticLibrary
//
//  Created by Charles Culbert on 10/25/24.
//

#import <Foundation/Foundation.h>
#import "NativeLibWrapper.h"
#import "../Cpp/NativeLib.hpp"

@interface NativeLibWrapper() {
    NativeLib nativeLib;
}
@end

@implementation NativeLibWrapper


- (void)createPlayer {
    nativeLib.createPlayer();
}

- (void)deletePlayer {
    nativeLib.deletePlayer();
}

- (void)nativeNoteOn:(int)midiNote velocity:(float)velocity {
    nativeLib.nativeNoteOn(midiNote, velocity);
}

- (void)nativeNoteOff:(int)midiNote {
    nativeLib.nativeNoteOff(midiNote);
}

- (void)nativeSetVolume:(float)volume {
    nativeLib.nativeSetVolume(volume);
}


@end
