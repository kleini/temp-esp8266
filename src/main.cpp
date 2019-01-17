#include <Homie.h>

// GPIO0
const int PIN_BUTTON = 0;

void onHomieEvent(const HomieEvent & event) {
  switch (event.type) {
    case HomieEventType::WIFI_DISCONNECTED:
      WiFi.disconnect();
      break;
  }
}

// LED_BUILTIN is connected to GPIO1, which is TXD, too. So either use LED or serial console.
void setup() {
    Serial.begin(115200);
    Serial << endl << endl;

    Homie_setFirmware("temp-esp8266", "0.9.0");
    Homie.setResetTrigger(PIN_BUTTON, LOW, 5000);

    Homie.onEvent(onHomieEvent);
    Homie.setup();
}

void loop() {
    Homie.loop();
}
