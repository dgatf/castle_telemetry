#include "castle_telemetry.h"

boolean castleRead(uint8_t reg, uint16_t &value) {
  uint8_t val = -(B10000000 + reg);
  uint8_t buf[3];
  castleSerial.write((uint8_t)B10000000);
  castleSerial.write(reg);
  castleSerial.write((uint8_t)0);
  castleSerial.write((uint8_t)0);
  castleSerial.write(val);
  if (castleSerial.readBytes(buf, 3) == 3) {
    value = buf[0] << 8 | buf[1];
  }
  // crc
  if (buf[0] + buf[1] + buf[2] == 0)
    return true;
  return false;
}

void setup() {

  smartportSerial.begin(57600);

  castleSerial.begin(9600);
  castleSerial.setTimeout(CASTLESERIAL_TIMEOUT);
  castleSerial.write((uint8_t)0);
  castleSerial.write((uint8_t)0);
  castleSerial.write((uint8_t)0);
  castleSerial.write((uint8_t)0);
  castleSerial.write((uint8_t)0);

  telemetry.voltageP = smartport.addElement(VFAS_FIRST_ID, REFRESH_VOLT);
  telemetry.rpmP = smartport.addElement(RPM_FIRST_ID, REFRESH_RPM);
  telemetry.temperatureP = smartport.addElement(T1_FIRST_ID, REFRESH_TEMP);
  telemetry.temperatureNtcP = smartport.addElement(T2_FIRST_ID, REFRESH_TEMP);

  telemetry.currentP = smartport.addElement(CURR_FIRST_ID, REFRESH_CURR);
  telemetry.rippleVoltageP =
      smartport.addElement(VFAS_FIRST_ID + 2, REFRESH_VOLT);
  telemetry.becCurrentP = smartport.addElement(CURR_FIRST_ID + 1, REFRESH_CURR);
  telemetry.becVoltageP = smartport.addElement(VFAS_FIRST_ID + 1, REFRESH_VOLT);
}

void loop() {
  uint16_t value = 0;
  if (castleRead(REG_VOLT, value))
    *telemetry.voltageP =
        smartport.formatData(VFAS_FIRST_ID, (float)value / 2042 * SCALE_VOLT);
  smartport.processTelemetry();
  if (castleRead(REG_RPM, value))
    *telemetry.rpmP =
        smartport.formatData(RPM_FIRST_ID, (float)value / 2042 * SCALE_RPM);
  smartport.processTelemetry();
  if (castleRead(REG_TEMP, value))
    *telemetry.temperatureP =
        smartport.formatData(T1_FIRST_ID, (float)value / 2042 * SCALE_TEMP);
  smartport.processTelemetry();
  if (castleRead(REG_TEMP_NTC, value))
    *telemetry.temperatureNtcP =
        smartport.formatData(T2_FIRST_ID, (float)value / 2042 * SCALE_TEMP);
  smartport.processTelemetry();

  if (castleRead(REG_CURR, value))
    *telemetry.currentP =
        smartport.formatData(CURR_FIRST_ID, (float)value / 2042 * SCALE_CURR);
  smartport.processTelemetry();
  if (castleRead(REG_RIPPLE_VOLT, value))
    *telemetry.rippleVoltageP = smartport.formatData(
        VFAS_FIRST_ID + 2, (float)value / 2042 * SCALE_RIPPLE_VOLT);
  smartport.processTelemetry();
  if (castleRead(REG_BEC_CURR, value))
    *telemetry.becCurrentP = smartport.formatData(
        CURR_FIRST_ID + 1, (float)value / 2042 * SCALE_BEC);
  smartport.processTelemetry();
  if (castleRead(REG_BEC_VOLT, value))
    *telemetry.becVoltageP = smartport.formatData(
        VFAS_FIRST_ID + 1, (float)value / 2042 * SCALE_BEC);
  smartport.processTelemetry();
}
