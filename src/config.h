#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
//                    PROJECT IDENTITY
// ============================================================
// PROJECT_NAME: Set to your repo folder name (e.g., "OER.Room.UpsideDown.Radio")
// DEVICE_IDENTIFIER: Used for MQTT client/topics, mDNS hostname, and OTA
//                    Naming convention: oer-{room}-{prop} (e.g., oer-ud-radio)
#define PROJECT_NAME "OER.Monsters.AudioPlayer"
#define DEVICE_IDENTIFIER "oer-monsters-audioplayer"

// ============================================================
//                  MODULE ENABLE/DISABLE
// ============================================================
#define USE_WIFI 0      // WiFi connectivity
#define USE_MQTT 0      // MQTT pub/sub (requires WiFi)
#define USE_MDNS 0      // mDNS discovery (requires WiFi)
#define USE_OTA 0       // Over-the-air updates (requires WiFi)
#define USE_ESPNOW 1    // ESP-NOW communication
#define USE_HEARTBEAT 1 // Heartbeat LED status indicator

// Dependency checks
#if USE_MQTT && !USE_WIFI
#error "MQTT requires WiFi to be enabled (USE_WIFI 1)"
#endif
#if USE_MDNS && !USE_WIFI
#error "mDNS requires WiFi to be enabled (USE_WIFI 1)"
#endif
#if USE_OTA && !USE_WIFI
#error "OTA requires WiFi to be enabled (USE_WIFI 1)"
#endif

// ============================================================
//                   WIFI CONFIGURATION
// ============================================================
#if USE_WIFI
#define WIFI_SSID "[QUARANTINED] Virus Detected"
#define WIFI_PASS "Vince1138"
#endif

// ============================================================
//                   MQTT CONFIGURATION
// ============================================================
#if USE_MQTT
#define MQTT_BROKER "192.168.1.218"
#define MQTT_PORT 1883
#define MQTT_TOPIC_PREFIX "SP/"
#endif

// ============================================================
//                    OTA CONFIGURATION
// ============================================================
#if USE_OTA
#define OTA_PASSWORD "" // Leave empty for no password
#endif

// ============================================================
//                  ESP-NOW CONFIGURATION
// ============================================================
#if USE_ESPNOW
#define ESPNOW_HOST 0 // 1 = Host (broadcasts), 0 = Client (listens)
// Host MAC address - set this to your host device's MAC
// Only used when ESPNOW_HOST is 0 (client mode)
#define ESPNOW_HOST_MAC {0xB8, 0xF8, 0x62, 0x62, 0xFF, 0x04}
#endif

// ============================================================
//                HEARTBEAT CONFIGURATION
// ============================================================
#if USE_HEARTBEAT
#define HEARTBEAT_PIN 48 // GPIO48 = onboard RGB LED on ESP32-S3-DevKitC-1
#endif

// ============================================================
//                     PIN DEFINITIONS
// ============================================================
// ESP32-S3 Pin Notes (YD-ESP32-S3 / DevKitC-1):
// - GPIO 0, 3, 45, 46: Strapping pins (affect boot mode) - avoid for outputs
// - GPIO 19, 20: Reserved for native USB - do not use
// - GPIO 26-32: Not available on N8R2 (used for PSRAM/Flash)
// - GPIO 33-34: Do not exist on ESP32-S3 (chip architecture difference from ESP32)
// - GPIO 35-37: Reserved for PSRAM on N8R2/N8R8 variants; available on N8 (no PSRAM)
// - GPIO 38-42: Available for general use (38-39 fully available; 40-42 may overlap JTAG)
// - GPIO 43-44: Reserved for UART0 (TX/RX)
// - GPIO 48: Onboard RGB LED (directly usable for NeoPixel output)
// - ADC1: GPIO 1-10 (safe to use with WiFi)
// - ADC2: GPIO 11-20 (conflicts with WiFi - avoid for ADC when WiFi active)
// - No DAC pins on S3 - use I2S or external DAC for audio
//
// Safe general-purpose pins: 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 38, 39, 40, 41, 42, 47
#define INPUT_PIN 4
#define OUTPUT_PIN 5

// Reset button pin (active LOW with internal pullup)
#define RESET_PIN 6

// DY-HV20T Audio Player (UART)
#define DYPLAYER_RX_PIN 16  // ESP32 RX <- DY-HV20T TX (IO0)
#define DYPLAYER_TX_PIN 17  // ESP32 TX -> DY-HV20T RX (IO1)

// ============================================================
//                   DY-HV20T AUDIO SETTINGS
// ============================================================
#define DYPLAYER_VOLUME 18     // Default volume level (0-30), saved to flash
#define DYPLAYER_VOLUME_STEP 6 // Volume change per button press
#define DYPLAYER_TRACK 1       // Track number to loop (00001.mp3)

/*
 * DY-HV20T WIRING:
 * ================
 * DY-HV20T Pin   ESP32-S3 Pin    Notes
 * -----------    ------------    -----
 * VIN            5V              Power supply (6-35VDC)
 * GND            GND             Common ground
 * IO0 (TX)       GPIO16 (RX2)    Serial data from player
 * IO1 (RX)       GPIO17 (TX2)    Serial data to player
 * SPK+           Speaker +       4-8 ohm speaker (up to 20W)
 * SPK-           Speaker -       4-8 ohm speaker
 *
 * CON1           GND             \
 * CON2           GND              } One-line serial mode
 * CON3           3.3V            /
 *
 * Note: ESP32 is 3.3V logic, DY-HV20T is 5V tolerant on IO pins.
 *       No level shifter needed for ESP32 connection.
 *
 * ESP-NOW COMMANDS (from controller ESP32):
 * =========================================
 * "PLAY"     - Start/resume playback
 * "STOP"     - Stop playback
 * "RESTART"  - Restart track from beginning
 * "VOL_UP"   - Increase volume
 * "VOL_DOWN" - Decrease volume
 */

#endif
