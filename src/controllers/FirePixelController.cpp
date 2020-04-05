#include "controllers/FirePixelController.h"

FirePixelController::FirePixelController(std::unique_ptr<FirePixel>firePixel) {
  m_firePixel = std::move(firePixel);
}

// ***************************************
// ** FIREPIXEL COMMANDS                **
// ***************************************
void FirePixelController::initialize() {
  Serial.println("FirePixelController::initialize");
  uint8_t startBytes[5] = {124,0,165,165,94};
  m_firePixel->writeBytes(startBytes, 5);
  delay(100);
  uint8_t initializeBytes[5] = {1, 0, 0, 0, 254};
  m_firePixel->writeBytes(initializeBytes, 5);
  delay(100);
}

void FirePixelController::ignite() {
  Serial.println("FirePixelController::ignite");
  uint8_t unlockBytes[5] = {3, m_firePixel->getAddress(), 0, 0, 0};
  unlockBytes[4] = calculate_checksum(unlockBytes, 4);
  m_firePixel->writeBytes(unlockBytes, 5);
  // unlock AfterBurner
  uint8_t afterBurnerUnlockBytes[5] = {128, m_firePixel->getAddress(), 0, 0, 0};
  afterBurnerUnlockBytes[4] = calculate_checksum(afterBurnerUnlockBytes, 4);
  m_firePixel->writeBytes(afterBurnerUnlockBytes, 5);
  delay(100);

}

void FirePixelController::off() {
  Serial.println("FirePixelController::off");
  uint8_t lockBytes[5] = {2, m_firePixel->getAddress(), 0, 0, 0};
  lockBytes[4] = calculate_checksum(lockBytes, 4);
  m_firePixel->writeBytes(lockBytes, 5);
}

void FirePixelController::setFlameValue(uint8_t value) {
  uint8_t flameValueBytes[5] = {0, m_firePixel->getAddress(), value, 0, 0};
  flameValueBytes[4] = calculate_checksum(flameValueBytes, 4);
  m_firePixel->writeBytes(flameValueBytes, 5);
}

void FirePixelController::setAfterBurnerValue(uint8_t value) {
  // set AfterBurner valve value
  uint8_t afterBurnerValueBytes[5] = {128, m_firePixel->getAddress(), value, 1, 0};
  afterBurnerValueBytes[4] = calculate_checksum(afterBurnerValueBytes, 4);
  m_firePixel->writeBytes(afterBurnerValueBytes, 5);
  delay(100);
}


unsigned char FirePixelController::rot(unsigned char value) {
  return ((value % 2) * 128) + (value >> 1);
}

unsigned char FirePixelController::calculate_checksum(const unsigned char *data, uint8_t size) {
    if (size <= 0) {
        return 0;
    }
    char checksum = data[size - 1];
    for (int i = (int)size - 2; i >= 0; i--) {
        checksum = rot(checksum) ^ data[i];
    }
    checksum = checksum ^ 255;
    return checksum;
}
