/*
 * castle telemetry v0.1
 *
 * License https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * Arduino sketch to send to Frsky SmartPort Castle Serial Link telemetry
 *
 * Wiring
 * ------
 *
 * - SmartPort Vcc to Arduino RAW
 * - SmartPort Gnd to Arduino Gnd
 * - Smartport Signal to Arduino PIN_SMARTPORT_RX (7)
 * - Smartport Signal to R1 (4.7k)
 * - R3 (4.7k) to Arduino PIN_SMARTPORT_TX (12)
 * - Castle Serial Link Rx to Arduino Tx
 * - Castle Serial Link Tx to Arduino Rx
 *
 */

#define PIN_SMARTPORT_RX 7
#define PIN_SMARTPORT_TX 12

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
  float rpm = 0;
  float volt = 0;
  float curr = 0;
  float rippleVoltage = 0;
  float becVolt = 0;
  float becCurr = 0;
  float temperature = 0;
  float temperatureNtc = 0;
};

boolean castleRead(uint8_t reg, uint16_t &value);
void setup();
void loop();

Telemetry telemetry;
SoftwareSerial smartportSerial(PIN_SMARTPORT_RX, PIN_SMARTPORT_TX, true);
Smartport smartport(smartportSerial);
