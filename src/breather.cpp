#include "breather.h"

Breather::Breather() :
m_firePixelController(std::unique_ptr<FirePixel>(new FirePixel(Serial2, 1))) {

}

void Breather::start() {
  m_firePixelController.initialize();
  m_firePixelController.ignite();
}

void Breather::setValveAperture(uint8_t value) {
  m_firePixelController.setFlameValue(value);
}
