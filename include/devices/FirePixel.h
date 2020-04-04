#pragma once
#include <stdint.h>
#include <Arduino.h>
#include "models/FirePixelModel.h"

class FirePixel {
public:
  FirePixel(HardwareSerial serial, uint8_t address);
  // void write(EventQueue &queue);
  void writeBytes(uint8_t *data, uint8_t length);

  uint8_t getAddress() { return m_address; }

protected:
  FirePixelModel m_firePixelModel;

private:
  unsigned char calculate_checksum(const unsigned char *data, uint8_t size);
  unsigned char rot(unsigned char value);

  HardwareSerial m_serial;
  uint8_t        m_address;
};
