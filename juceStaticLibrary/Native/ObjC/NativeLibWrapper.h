//
//  NativeLibWrapper.h
//  juceStaticLibrary
//
//  Created by Charles Culbert on 10/25/24.
//

#import <Foundation/Foundation.h>

@interface NativeLibWrapper : NSObject

- (void) createPlayer;

- (void) deletePlayer;

- (void) nativeNoteOn:(int)midiNote velocity:(float)velocity;

- (void) nativeNoteOff: (int) midiNote;

- (void) nativeSetVolume: (float) volume;


@end
