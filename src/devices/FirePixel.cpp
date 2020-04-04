#include "devices/FirePixel.h"
#define LOG_FLAME_COMMANDS


FirePixel::FirePixel(HardwareSerial serial, uint8_t address) :
m_serial(serial),
m_address(address) {

}


void FirePixel::writeBytes(uint8_t *data, uint8_t length) {
  #ifdef LOG_FLAME_COMMANDS
  Serial.print("-> [");
  for(int i=0; i<length; i++) {
    Serial.print(data[i], DEC);
    if(i<length-1) {
      Serial.print(", ");
    }
  }
  Serial.println("]");
  #endif
  m_serial.write(data, length);
}
