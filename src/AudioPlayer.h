#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <Arduino.h>

// Initialize the DY-HV20T audio player
void audioPlayerInit();

// Start looping the configured track
void audioPlayerStartLoop();

// Play/resume playback
void audioPlayerPlay();

// Stop playback
void audioPlayerStop();

// Restart track from beginning
void audioPlayerRestart();

// Volume controls
void audioPlayerVolumeUp();
void audioPlayerVolumeDown();
void audioPlayerSetVolume(uint8_t volume);

// Get current volume (0-30)
uint8_t audioPlayerGetVolume();

#endif
