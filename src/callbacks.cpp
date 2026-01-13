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
#include "modules/espnow_module.h"

// Send ACK response to sender
static void sendAck(const uint8_t* mac) {
  const char* ack = "ACK";
  espnowSend(mac, (const uint8_t*)ack, strlen(ack));
  Serial.println("[ESP-NOW] Sent ACK");
}

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

  if (command == "PING") {
    // Respond with ACK for connection monitoring
    sendAck(mac);
  } else if (command == "PLAY" || command == "START") {
    audioPlayerPlay();
    sendAck(mac);
  } else if (command == "STOP") {
    audioPlayerStop();
    sendAck(mac);
  } else if (command == "RESTART") {
    audioPlayerRestart();
    sendAck(mac);
  } else if (command == "VOL_UP" || command == "VOLUP" || command == "V+") {
    audioPlayerVolumeUp();
    sendAck(mac);
  } else if (command == "VOL_DOWN" || command == "VOLDOWN" || command == "V-") {
    audioPlayerVolumeDown();
    sendAck(mac);
  } else if (command == "RESET") {
    sendAck(mac);
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
