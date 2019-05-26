#define PIN_SMARTPORT_RX 8  // only pins 8,9,10,11
#define PIN_SMARTPORT_TX 11

#define REG_VOLT 0
#define REG_RIPPLE_VOLT 1
#define REG_CURR 2
#define REG_THR 3
#define REG_RPM 5
#define REG_TEMP 6
#define REG_BEC_VOLT 7
#define REG_BEC_CURR 8
#define REG_TEMP_NTC 9
#define REG_TEMP_LINEAR 10

#define SCALE_VOLT 20
#define SCALE_RIPPLE_VOLT 4
#define SCALE_BEC 4
#define SCALE_CURR 50
#define SCALE_THR 1
#define SCALE_RPM 20416.66
#define SCALE_TEMP 30

#define REFRESH_RPM 200
#define REFRESH_VOLT 1000
#define REFRESH_CURR 1000
#define REFRESH_TEMP 1000

#define CASTLESERIAL_TIMEOUT 2
#define castleSerial Serial

#include "Smartport.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

struct Telemetry {
  float *rpmP = NULL;
  float *voltageP = NULL;
  float *rippleVoltageP = NULL;
  float *currentP = NULL;
  float *becCurrentP = NULL;
  float *becVoltageP = NULL;
  float *temperatureP = NULL;
  float *temperatureNtcP = NULL;
};

boolean castleRead(uint8_t reg, uint16_t &value);
void setup();
void loop();

Telemetry telemetry;
SoftwareSerial smartportSerial(PIN_SMARTPORT_RX, PIN_SMARTPORT_TX, true);
Smartport smartport(smartportSerial);
