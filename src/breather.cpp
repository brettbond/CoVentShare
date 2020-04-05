#include "breather.h"

Breather::Breather(uint8_t breathsPerMinute) :
m_firePixelController(std::unique_ptr<FirePixel>(new FirePixel(Serial2, 1))),
m_breathsPerMinute(breathsPerMinute),
m_breathStartTime(0),
m_lastBreathEndTime(0),
m_breathState(BREATH_NOT_STARTED),
m_currentFlowRate(0) {

}

void Breather::start() {
  m_firePixelController.initialize();
  m_firePixelController.ignite();
}

uint8_t Breather::breathe() {
  switch(m_breathState) {
    case BREATH_NOT_STARTED:
      Serial.println("START INHALE");
      // time to breathe
      m_breathStartTime = millis();
      m_breathState = INHALE;
      m_currentFlowRate = calcFlowRate();
    break;

    case INHALE:
      m_currentFlowRate = calcFlowRate();
      if(percentOfInhale() >= 99) {
        Serial.println("START EXHALE");
        m_breathState = EXHALE;
      }
    break;

    case EXHALE:
      m_currentFlowRate = 0;
      if(percentOfExhale() >= 99) {
        Serial.println("BREATH COMPLETE");
        m_breathState = BREATH_NOT_STARTED;
      }
    break;
  }
  setValveAperture(map(m_currentFlowRate, 0, 40, 0, 200));
  return m_currentFlowRate;
}

void Breather::setValveAperture(uint8_t value) {
  m_firePixelController.setAfterBurnerValue(value);
}

uint8_t Breather::calcFlowRate() {
  if(m_breathState != INHALE) {
    return 0;
  }

  if(m_breathStartTime <= 0) {
    return 0;
  }

  uint8_t currentFlowRate = float(percentOfInhale()) / 100.0f * MAX_FLOW_RATE;
  return currentFlowRate;
}

uint32_t Breather::timeSinceBreathStarted() {
  return millis() - m_breathStartTime;
}

uint8_t Breather::percentOfInhale() {
  if(m_breathState != INHALE) {
    return 0;
  }
  return float(timeSinceBreathStarted()) / float(totalInhaleTimeMillis()) * 100.0f;
}

uint8_t Breather::percentOfExhale() {
  if(m_breathState != EXHALE) {
    return 0;
  }
  return (float(timeSinceBreathStarted() - totalInhaleTimeMillis()) / (float)totalExhaleTimeMillis()) * 100.0f;
}

uint32_t Breather::totalInhaleTimeMillis() {
  // assume 1:1 inhale:exhale ratio
  return 60000 / (m_breathsPerMinute * 2);
}

uint32_t Breather::totalExhaleTimeMillis() {
  return totalInhaleTimeMillis();
}
