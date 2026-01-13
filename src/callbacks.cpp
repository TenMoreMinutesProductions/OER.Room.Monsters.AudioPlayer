#include "callbacks.h"
#include "config.h"
#include "setup.h"
#include "AudioPlayer.h"

// ============================================================
//                   CALLBACK FUNCTIONS
// ============================================================

#if USE_MQTT
// Called when MQTT message is received on subscribed topics
void onMqttMessage(String topic, String payload) {
  propLog("[MQTT] Received: " + payload);

  // Handle reset command (case-insensitive)
  if (payload.equalsIgnoreCase("reset")) {
    propLog("[MQTT] Reset command received");
    propRequestReset();
    return;
  }
}
#endif

#if USE_ESPNOW
// Called when ESP-NOW message is received
void onEspNowReceive(const uint8_t* mac, const uint8_t* data, int len) {
  // Convert received data to string
  char cmd[32];
  int copyLen = (len < 31) ? len : 31;
  memcpy(cmd, data, copyLen);
  cmd[copyLen] = '\0';

  Serial.print("[ESP-NOW] Received: ");
  Serial.print(cmd);
  Serial.print(" from ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // Handle audio player commands (case-insensitive)
  String command = String(cmd);
  command.trim();
  command.toUpperCase();

  if (command == "PLAY" || command == "START") {
    audioPlayerPlay();
  } else if (command == "STOP") {
    audioPlayerStop();
  } else if (command == "RESTART") {
    audioPlayerRestart();
  } else if (command == "VOL_UP" || command == "VOLUP" || command == "V+") {
    audioPlayerVolumeUp();
  } else if (command == "VOL_DOWN" || command == "VOLDOWN" || command == "V-") {
    audioPlayerVolumeDown();
  } else if (command == "RESET") {
    propRequestReset();
  } else {
    Serial.print("[ESP-NOW] Unknown command: ");
    Serial.println(cmd);
  }
}

// Called when ESP-NOW send completes
void onEspNowSend(const uint8_t* mac, bool success) {
  Serial.print("[ESP-NOW] Send ");
  Serial.println(success ? "OK" : "FAILED");
}
#endif

// ============================================================
//                    RESET HANDLER
// ============================================================
// Called when reset is triggered via button (held 1s) or ESP-NOW command.

void onPropReset() {
  propLog("[Reset] Resetting audio player...");

  // Stop current playback
  audioPlayerStop();

  // Reset volume to default
  audioPlayerSetVolume(DYPLAYER_VOLUME);

  // Restart the audio loop
  audioPlayerStartLoop();

  propLog("[Reset] Complete - audio restarted");
}
