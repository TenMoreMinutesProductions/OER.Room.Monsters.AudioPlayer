#include "AudioPlayer.h"
#include "config.h"
#include <DYPlayerArduino.h>

// Use Hardware Serial2 for DY-HV20T
static DY::Player player(&Serial2);
static uint8_t currentVolume = DYPLAYER_VOLUME;

void audioPlayerInit() {
  Serial.flush();  // Ensure serial buffer is clear before initializing Serial2
  Serial2.begin(9600, SERIAL_8N1, DYPLAYER_RX_PIN, DYPLAYER_TX_PIN);
  delay(500);  // Give module time to initialize after power-on

  player.setVolume(currentVolume);
  Serial.print("[Audio] Initialized DY-HV20T, volume: ");
  Serial.println(currentVolume);
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
    currentVolume++;
    player.setVolume(currentVolume);
    Serial.print("[Audio] Volume up: ");
    Serial.println(currentVolume);
  } else {
    Serial.println("[Audio] Volume already at max (30)");
  }
}

void audioPlayerVolumeDown() {
  if (currentVolume > 0) {
    currentVolume--;
    player.setVolume(currentVolume);
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
  Serial.print("[Audio] Volume set to: ");
  Serial.println(currentVolume);
}

uint8_t audioPlayerGetVolume() {
  return currentVolume;
}
