#include "loop.h"
#include "config.h"
#include "setup.h"
#include "AudioPlayer.h"
#include "esp_task_wdt.h"

// OTA status check (task runs on Core 0)
#if USE_OTA
  #include "modules/ota_module.h"
#endif

// Forward declaration of reset handler (implemented in callbacks.cpp)
extern void onPropReset();

// Flag to track if audio has started
static bool audioStarted = false;

void loopMain() {
  // ============================================================
  // Core 0 Tasks (run automatically via FreeRTOS):
  //   - WiFi reconnection
  //   - OTA update handling
  //   - ESP-NOW (callbacks are event-driven)
  //   - Reset button monitoring
  // ============================================================

  // Feed the watchdog - proves loop is running
  esp_task_wdt_reset();

  // ============================================================
  // Core 1 Tasks (run here in main loop):
  // ============================================================

  #if USE_OTA
    // Skip game logic during OTA update
    if (otaIsUpdating()) {
      return;
    }
  #endif

  // Check for reset request (from button or MQTT)
  if (propResetRequested()) {
    propClearResetRequest();
    onPropReset();
  }

  // ============================================================
  // Audio Player - Start looping on first run
  // ============================================================
  if (!audioStarted) {
    audioPlayerStartLoop();
    audioStarted = true;
  }
}
