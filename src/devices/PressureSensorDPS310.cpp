#include "devices/PressureSensorDPS310.h"

PressureSensorDPS310::PressureSensorDPS310(uint8_t sdaPin, uint8_t sclPin) :
pressureWire(TwoWire(0)),
dps(),
m_sdaPin(sdaPin),
m_sclPin(sclPin) {}

bool PressureSensorDPS310::begin() {
  Serial.println("DPS310");
  pressureWire.begin(m_sdaPin, m_sclPin, 100000);
  if (! dps.begin_I2C(0x77, &pressureWire)) {             // Can pass in I2C address here
    Serial.println("Failed to find DPS");
    return false;
  }
  Serial.println("DPS OK!");

  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
  return true;
}

bool PressureSensorDPS310::isAvailable() {
  return dps.temperatureAvailable() && dps.pressureAvailable();
}

void PressureSensorDPS310::getEvents() {
  dps.getEvents(&m_temperatureEvent, &m_pressureEvent);
}

float PressureSensorDPS310::getAltitudeMeters() {
  getEvents();
  return (((pow((sea_press/(m_pressureEvent.pressure)), 1/5.257) - 1.0)*(m_temperatureEvent.temperature +273.15)))/0.0065;
}
