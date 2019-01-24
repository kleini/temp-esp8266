/*
 * BME280Node.cpp
 * Homie Node for BME280 sensors using Adafruit BME280 library.
 *
 * Version: 1.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 * Author: Markus Haack (http://github.com/mhaack)
 */

#include "BME280Node.hpp"

BME280Node::BME280Node(const char *name, const int i2cAddress, const int measurementInterval)
    : HomieNode(name, "BME280Sensor"), _i2cAddress(i2cAddress), _lastMeasurement(0) {
  _measurementInterval = (measurementInterval > MIN_INTERVAL) ? measurementInterval : MIN_INTERVAL;
}

void BME280Node::printCaption() {
  Homie.getLogger() << cCaption << endl;
}

void BME280Node::loop() {
  if (millis() - _lastMeasurement >= _measurementInterval * 1000UL || _lastMeasurement == 0) {
    _lastMeasurement = millis();

    if (_sensorFound) {
      bme.takeForcedMeasurement(); // has no effect in normal mode

      temperature = bme.readTemperature();
      humidity = bme.readHumidity();
      pressure = bme.readPressure() / 100;

      printCaption();

      Homie.getLogger() << cIndent << "Temperature: " << temperature << " °C" << endl;
      Homie.getLogger() << cIndent << "Humidity: " << humidity << " %" << endl;
      Homie.getLogger() << cIndent << "Pressure: " << pressure << " hPa" << endl;

      setProperty("status").send("ok");
      setProperty("temperature").send(String(temperature));
      setProperty("humidity").send(String(humidity));
      setProperty("pressure").send(String(pressure));
    } else {
      setProperty("status").send("error");
    }
  }
}

void BME280Node::setupHandler() {
  setProperty("temperature/unit").send("°C");
  setProperty("humidity/unit").send("%");
  setProperty("pressure/unit").send("hPa");
}

void BME280Node::setup() {
  advertise("status");
  advertise("temperature");
  advertise("temperature/unit");
  advertise("humidity");
  advertise("humidity/unit");
  advertise("pressure");
  advertise("pressure/unit");

  if (bme.begin(_i2cAddress)) {
    _sensorFound = true;
    Homie.getLogger() << cCaption << " found" << endl
                      << cIndent << "Reading interval: " << _measurementInterval << " s" << endl;
    // Parameters taken from the weather station monitoring example (advancedsettings.ino) in
    // the Adafruit BME280 library
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF);
  } else {
    _sensorFound = false;
    Homie.getLogger() << cCaption << " not found. Check wiring!" << endl;
  }
}
