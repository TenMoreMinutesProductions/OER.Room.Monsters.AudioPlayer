#include "AudioPlayer.h"
#include "config.h"
#include <DYPlayerArduino.h>
#include <Preferences.h>

// Use Hardware Serial2 for DY-HV20T
static DY::Player player(&Serial2);
static Preferences prefs;
static uint8_t currentVolume = DYPLAYER_VOLUME;

// Save volume to flash
static void saveVolume() {
  prefs.putUChar("volume", currentVolume);
}

void audioPlayerInit() {
  Serial.flush();  // Ensure serial buffer is clear before initializing Serial2
  Serial2.begin(9600, SERIAL_8N1, DYPLAYER_RX_PIN, DYPLAYER_TX_PIN);
  delay(500);  // Give module time to initialize after power-on

  // Load saved volume from flash (or use default if not set)
  prefs.begin("audio", false);
  currentVolume = prefs.getUChar("volume", DYPLAYER_VOLUME);
  if (currentVolume > 30) currentVolume = DYPLAYER_VOLUME;  // Sanity check

  player.setVolume(currentVolume);
  Serial.print("[Audio] Initialized DY-HV20T, volume: ");
  Serial.print(currentVolume);
  Serial.println(" (loaded from flash)");
}

void audioPlayerStartLoop() {
  player.setCycleMode(DY::PlayMode::RepeatOne);  // Loop single track
  player.playSpecified(DYPLAYER_TRACK);
  delay(50);
  Serial.print("[Audio] Starting loop on track: ");
  Serial.println(DYPLAYER_TRACK);
}

void audioPlayerPlay() {
  player.play();
  Serial.println("[Audio] Play/Resume");
}

void audioPlayerStop() {
  player.stop();
  Serial.println("[Audio] Stopped");
}

void audioPlayerRestart() {
  player.playSpecified(DYPLAYER_TRACK);
  delay(50);
  Serial.println("[Audio] Restarted from beginning");
}

void audioPlayerVolumeUp() {
  if (currentVolume < 30) {
    currentVolume += DYPLAYER_VOLUME_STEP;
    if (currentVolume > 30) currentVolume = 30;
    player.setVolume(currentVolume);
    saveVolume();
    Serial.print("[Audio] Volume up: ");
    Serial.println(currentVolume);
  } else {
    Serial.println("[Audio] Volume already at max (30)");
  }
}

void audioPlayerVolumeDown() {
  if (currentVolume > 0) {
    if (currentVolume >= DYPLAYER_VOLUME_STEP) {
      currentVolume -= DYPLAYER_VOLUME_STEP;
    } else {
      currentVolume = 0;
    }
    player.setVolume(currentVolume);
    saveVolume();
    Serial.print("[Audio] Volume down: ");
    Serial.println(currentVolume);
  } else {
    Serial.println("[Audio] Volume already at min (0)");
  }
}

void audioPlayerSetVolume(uint8_t volume) {
  if (volume > 30) volume = 30;
  currentVolume = volume;
  player.setVolume(currentVolume);
  saveVolume();
  Serial.print("[Audio] Volume set to: ");
  Serial.println(currentVolume);
}

uint8_t audioPlayerGetVolume() {
  return currentVolume;
}
