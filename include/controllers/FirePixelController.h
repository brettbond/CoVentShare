#pragma once

#include "devices/FirePixel.h"
#include <memory>

class FirePixelController {
public:
  FirePixelController(std::unique_ptr<FirePixel>firePixel);

  void initialize();
  void ignite();
  void off();
  void setFlameValue(uint8_t value);
  void setAfterBurnerValue(uint8_t value);

protected:
  unsigned char rot(unsigned char value);
  unsigned char calculate_checksum(const unsigned char *data, uint8_t size);


private:
  std::unique_ptr<FirePixel> m_firePixel;
};
