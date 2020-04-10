#include "Adafruit_DPS310.h"
const float sea_press = 1017.25;


class PressureSensorDPS310 {
public:
  PressureSensorDPS310(uint8_t sdaPin, uint8_t sclPin);
  bool begin();
  bool isAvailable();
  void getEvents();
  float getAltitudeMeters();

protected:
  TwoWire pressureWire;
  Adafruit_DPS310 dps;

  uint8_t m_i2cAddress;
  uint8_t m_sdaPin;
  uint8_t m_sclPin;

  sensors_event_t m_temperatureEvent, m_pressureEvent;
};
