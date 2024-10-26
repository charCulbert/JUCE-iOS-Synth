import SwiftUI

// ContentView.swift
struct ContentView: View {
    let nativeManager = NativeManager()
    @State private var volume: Double = 0.8
    
    // Define some musical intervals
    let intervals = [
        ("Fifth (D-A)", [62, 69]),
        ("Fourth (D-G)", [62, 67]),
        ("Major Third (D-F#)", [62, 66]),
        ("Minor Third (D-F)", [62, 65]),
        ("Octave (D-D)", [62, 74])
    ]
    
    // Individual note definitions
    let notes = [
        ("C", 60), ("C#", 61), ("D", 62), ("D#", 63),
        ("E", 64), ("F", 65), ("F#", 66), ("G", 67),
        ("G#", 68), ("A", 69), ("A#", 70), ("B", 71)
    ]
    
    var body: some View {
        
        // Performance monitor overlay
                   VStack {
                       Spacer()
                       HStack {
                           Spacer()
                           PerformanceView()
                               .padding()
                       }
                   }
        
        
        Button("Test Sound") {
            print("Setting up audio...")
            nativeManager.setup()
            
            // Set volume to max
            nativeManager.setVolume(volume: 1.0)
            
            // Test sequence
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
                print("Playing middle C...")
                nativeManager.noteOn(midiNote: 60, velocity: 1.0)
                
                DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
                    print("Playing E...")
                    nativeManager.noteOn(midiNote: 64, velocity: 1.0)
                    
                    DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
                        print("Playing G...")
                        nativeManager.noteOn(midiNote: 67, velocity: 1.0)
                        
                        // Stop all notes after 1 second
                        DispatchQueue.main.asyncAfter(deadline: .now() + 1.0) {
                            print("Stopping all notes...")
                            nativeManager.noteOff(midiNote: 60)
                            nativeManager.noteOff(midiNote: 64)
                            nativeManager.noteOff(midiNote: 67)
                        }
                    }
                }
            }
        }
        
        
        
        VStack(spacing: 20) {
            Button("Setup") {
                nativeManager.setup()
            }
            .padding()
            
            // Volume slider - fixed syntax
                   VStack {
                       Text("Volume: \(Int(volume * 100))%")
                       Slider(value: $volume, in: 0...1) { _ in
                           nativeManager.setVolume(volume: Float(volume))
                       }
                   }
            .padding()
            
            // Intervals section
            GroupBox("Intervals") {
                VStack(spacing: 10) {
                    ForEach(intervals, id: \.0) { interval in
                        Button(interval.0) {
                            // Play both notes of the interval
                            for note in interval.1 {
                                nativeManager.noteOn(midiNote: Int32(note), velocity: 0.8)
                            }
                            
                            // Auto note-off after 500ms
                            DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
                                for note in interval.1 {
                                    nativeManager.noteOff(midiNote: Int32(note))
                                }
                            }
                        }
                        .buttonStyle(.bordered)
                    }
                }
                .padding(.vertical)
            }
            .padding(.horizontal)
            
            // Individual notes section
            GroupBox("Single Notes") {
                LazyVGrid(columns: [
                    GridItem(.adaptive(minimum: 50))
                ], spacing: 10) {
                    ForEach(notes, id: \.0) { note in
                        Button(note.0) {
                            nativeManager.noteOn(midiNote: Int32(note.1), velocity: 0.8)
                            
                            // Auto note-off after 500ms
                            DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
                                nativeManager.noteOff(midiNote: Int32(note.1))
                            }
                        }
                        .buttonStyle(.bordered)
                    }
                }
                .padding()
            }
            .padding(.horizontal)
            
            Spacer()
        }
        .padding()
    }
}
