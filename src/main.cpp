#define FW_NAME "temp-esp8266"
#define FW_VERSION "0.9.1"

const int PIN_SDA = 0;
const int PIN_SCL = 2;
const int I2C_BME280_ADDRESS = 0x76;

#define OTA_PASS "lala"
#define OTA_PORT 8266

#include <Homie.h>
#include "ota.hpp"
#include "BME280Node.hpp"

BME280Node bme280Node("bme280", I2C_BME280_ADDRESS);

void onHomieEvent(const HomieEvent & event) {
  switch (event.type) {
    case HomieEventType::WIFI_DISCONNECTED:
      WiFi.disconnect();
      break;
    case HomieEventType::MQTT_READY:
      Homie.getLogger() << "MQTT connected, preparing for deep sleep..." << endl;
      Homie.prepareToSleep();
      break;
    case HomieEventType::READY_TO_SLEEP:
      Homie.getLogger() << "Ready to sleep for " << ESP.deepSleepMax() << endl;
      // microseconds
      Homie.doDeepSleep(1000000UL);
      break;
    default:
      break;
  }
}

void setupHandler() {
  // advertise units
  bme280Node.setupHandler();
}

void loopHandler() {
  // We want to OTA handler to run only in the loop when WiFi is connected
  otaLoop();
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  Wire.begin(PIN_SDA, PIN_SCL);

  Homie_setFirmware(FW_NAME, FW_VERSION);
  // GPIO0 is used for I²C SDA
  Homie.disableResetTrigger();
  // LED_BUILTIN is connected to GPIO1, which is TXD, too. So either use LED or serial console.
  Homie.disableLedFeedback();

  Homie.onEvent(onHomieEvent);
  otaSetup(OTA_PORT, OTA_PASS);

  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}
