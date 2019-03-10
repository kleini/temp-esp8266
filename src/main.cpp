#define FW_NAME "temp-esp8266"
#define FW_VERSION "0.9.2"

const int PIN_SDA = 0;
const int PIN_SCL = 2;
const int I2C_BME280_ADDRESS = 0x77;

#include <Homie.h>
#include "BME280Node.hpp"
#include "Timer.h"

BME280Node bme280Node("bme280", I2C_BME280_ADDRESS, 60, Adafruit_BME280::SAMPLING_X16, Adafruit_BME280::SAMPLING_X16, Adafruit_BME280::SAMPLING_X16);
Timer t;

void prepareSleep() {
  Homie.prepareToSleep();
}

void onHomieEvent(const HomieEvent & event) {
  switch (event.type) {
    case HomieEventType::MQTT_READY:
      t.after(100, prepareSleep);
      break;
    case HomieEventType::READY_TO_SLEEP:
      Homie.doDeepSleep(60000000UL);
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  WiFi.forceSleepWake();

  Wire.begin(PIN_SDA, PIN_SCL);

  Homie_setFirmware(FW_NAME, FW_VERSION);
  // GPIO0 is used for I²C SDA
  Homie.disableResetTrigger();
  // LED_BUILTIN is connected to GPIO1, which is TXD, too. So either use LED or serial console.
  Homie.disableLedFeedback();

  Homie.onEvent(onHomieEvent);

  Homie.setup();
}

void loop() {
  Homie.loop();
  t.update();
}
