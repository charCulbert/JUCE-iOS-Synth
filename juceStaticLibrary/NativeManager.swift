//
//  NativeManager.swift
//  juceStaticLibrary
//
//  Created by Charles Culbert on 10/25/24.
//

// NativeManager.swift
class NativeManager {
    private let nativeLibWrapper = NativeLibWrapper()
    
    func setup() {
        nativeLibWrapper.createPlayer()
    }
    
    func dispose() {
        nativeLibWrapper.deletePlayer()
    }
    
    // New methods for MIDI note control
    func noteOn(midiNote: Int32, velocity: Float) {
        nativeLibWrapper.nativeNote(on: midiNote, velocity: velocity)
    }
    
    func noteOff(midiNote: Int32) {
        nativeLibWrapper.nativeNoteOff(midiNote)
    }
    
    func setVolume(volume: Float) {
        nativeLibWrapper.nativeSetVolume(volume)
    }
}
