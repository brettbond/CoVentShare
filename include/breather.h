#include "controllers/FirePixelController.h"

class Breather {
public:

  enum BreathState { BREATH_NOT_STARTED,
         INHALE,
         EXHALE };
  const uint8_t MAX_FLOW_RATE = 20;

  Breather(uint8_t breathsPerMinute);
  void start();
  uint8_t breathe();

protected:
  FirePixelController m_firePixelController;
  uint8_t m_breathsPerMinute;

  /** Starting time of current breath */
  int64_t m_breathStartTime;

  /** Ending time of last breath */
  int64_t m_lastBreathEndTime;

  BreathState m_breathState;

  uint8_t m_currentFlowRate;

  void setValveAperture(uint8_t value);

private:
    uint32_t timeSinceBreathStarted();
    uint32_t totalInhaleTimeMillis();
    uint32_t totalExhaleTimeMillis();
    uint8_t percentOfInhale();
    uint8_t percentOfExhale();
    uint8_t calcFlowRate();
};
