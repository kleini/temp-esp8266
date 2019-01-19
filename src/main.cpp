#define FW_NAME "temp-esp8266"
#define FW_VERSION "0.9.0"

#define OTA_PASS "lala"
#define OTA_PORT 8266

#include <Homie.h>
#include "ota.hpp"

// GPIO0
const int PIN_BUTTON = 0;

void onHomieEvent(const HomieEvent & event) {
  switch (event.type) {
    case HomieEventType::WIFI_DISCONNECTED:
      WiFi.disconnect();
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setResetTrigger(PIN_BUTTON, LOW, 5000);
  // LED_BUILTIN is connected to GPIO1, which is TXD, too. So either use LED or serial console.
  Homie.disableLedFeedback();

  Homie.onEvent(onHomieEvent);
  otaSetup(OTA_PORT, OTA_PASS);
  Homie.setup();
}

void loop() {
  Homie.loop();
  otaLoop();
}
