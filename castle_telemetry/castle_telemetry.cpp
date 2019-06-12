#include "castle_telemetry.h"

boolean castleRead(uint8_t reg, uint16_t &value) {
  while (castleSerial.available()) {
    castleSerial.read();
  }

  uint8_t crc = -(B10000000 + reg);
  uint8_t buf[5] = {B10000000, reg, 0, 0, crc};
  castleSerial.write(buf, 5);

  if (castleSerial.readBytes(buf, 3) == 3) {
    // crc
    if ((uint8_t)(buf[0] + buf[1] + buf[2]) == 0) {
      value = (uint16_t)buf[0] << 8 | buf[1];
      return true;
    }
  }
  return false;
}

void setup() {

  smartportSerial.begin(57600);

  castleSerial.begin(9600);
  castleSerial.setTimeout(CASTLESERIAL_TIMEOUT);
  uint8_t buf[5] = {0, 0, 0, 0, 0};
  castleSerial.write(buf, 5);
}

void loop() {
  uint16_t value1 = 0;
  uint16_t value2 = 0;

  if (castleRead(REG_VOLT, value1) && castleRead(REG_CURR, value2)) {
    telemetry.escPower = smartport.formatEscPower(
        (float)value1 / 2042 * SCALE_VOLT, (float)value2 / 2042 * SCALE_CURR);
  }
  smartport.sendData(ESC_POWER_FIRST_ID, telemetry.escPower, SENSOR_ID_10);
  if (castleRead(REG_RPM, value1)) {
    telemetry.rpm =
        smartport.formatEscRpmCons((float)value1 / 2042 * SCALE_RPM, 0);
  }
  smartport.sendData(ESC_RPM_CONS_FIRST_ID, telemetry.rpm, SENSOR_ID_10);
  if (castleRead(REG_RIPPLE_VOLT, value1)) {
    telemetry.rippleVoltage =
        smartport.formatEscPower((float)value1 / 2042 * SCALE_VOLT, 0);
  }
  smartport.sendData(ESC_POWER_FIRST_ID + 2, telemetry.rippleVoltage,
                     SENSOR_ID_10);
  if (castleRead(REG_BEC_VOLT, value1) && castleRead(REG_BEC_CURR, value2)) {
#ifdef OPENTX_BEC_FIX
    telemetry.escBecPower = smartport.formatBecPower(
        (float)value1 / 2042 * SCALE_BEC, (float)value2 / 2042 * SCALE_BEC);
#else
    telemetry.escBecPower = smartport.formatEscPower(
        (float)value1 / 2042 * SCALE_BEC, (float)value2 / 2042 * SCALE_BEC);
#endif
  }
#ifdef OPENTX_BEC_FIX
  smartport.sendData(SBEC_POWER_FIRST_ID, telemetry.escBecPower, SENSOR_ID_10);
#else
  smartport.sendData(ESC_POWER_FIRST_ID + 1, telemetry.escBecPower,
                     SENSOR_ID_10);
#endif
  if (castleRead(REG_TEMP, value1)) {
    telemetry.temperature = smartport.formatData(
        ESC_TEMPERATURE_FIRST_ID, (float)value1 / 2042 * SCALE_TEMP);
  }
  smartport.sendData(ESC_TEMPERATURE_FIRST_ID, telemetry.temperature,
                     SENSOR_ID_10);
  if (castleRead(REG_TEMP_NTC, value1)) {
    telemetry.temperatureNtc = smartport.formatData(
        ESC_TEMPERATURE_FIRST_ID + 1, (float)value1 / 2042 * SCALE_TEMP);
  }
  smartport.sendData(ESC_TEMPERATURE_FIRST_ID + 1, telemetry.temperatureNtc,
                     SENSOR_ID_10);
}
