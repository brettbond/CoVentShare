#include "controllers/FirePixelController.h"

class Breather {
public:
  Breather();
  void start();
  void setValveAperture(uint8_t value);

protected:
  FirePixelController m_firePixelController;
};
