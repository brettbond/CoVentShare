#pragma once
#include <stdint.h>

class FirePixelModel {
public:


protected:
  bool m_isLocked;
  uint8_t m_errorCode;
  uint8_t m_ignited;
  uint8_t m_initialized;
};
