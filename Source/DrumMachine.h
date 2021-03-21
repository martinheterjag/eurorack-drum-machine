/*
  ==============================================================================

    DrumMachine.h
    Created: 29 Feb 2020 5:38:47pm
    Author:  me

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

void playKick(int gpio, int level, uint32_t tick, void *userdata);
void playSnare(int gpio, int level, uint32_t tick, void *userdata);
void playClap(int gpio, int level, uint32_t tick, void *userdata);
void playClosedHat(int gpio, int level, uint32_t tick, void *userdata);
void playOpenHat(int gpio, int level, uint32_t tick, void *userdata);
void playCymbal(int gpio, int level, uint32_t tick, void *userdata);

class DrumMachine {
 public:
  DrumMachine();
  ~DrumMachine();

  AudioDeviceManager devmgr_;
  AudioIODevice* device_;

  ScopedPointer<AudioFormatReaderSource> kick_;
  ScopedPointer<AudioFormatReaderSource> snare_;
  ScopedPointer<AudioFormatReaderSource> clap_;
  ScopedPointer<AudioFormatReaderSource> closed_hat_;
  ScopedPointer<AudioFormatReaderSource> open_hat_;
  ScopedPointer<AudioFormatReaderSource> cymbal_;

  ScopedPointer<AudioSourcePlayer> kick_player_;
  ScopedPointer<AudioSourcePlayer> snare_player_;
  ScopedPointer<AudioSourcePlayer> clap_player_;
  ScopedPointer<AudioSourcePlayer> closed_hat_player_;
  ScopedPointer<AudioSourcePlayer> open_hat_player_;
  ScopedPointer<AudioSourcePlayer> cymbal_player_;

 private:
  AudioFormatManager fmgr_;
};
