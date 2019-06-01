#include "castle_telemetry.h"

boolean castleRead(uint8_t reg, uint16_t &value) {
  uint8_t val = -(B10000000 + reg);

  while (castleSerial.available()) {
    castleSerial.read();
  }

  uint8_t buf[5] = {(uint8_t)B10000000, reg, 0, 0, val};
  castleSerial.write(buf, 5);

  if (castleSerial.readBytes(buf, 3) == 3) {

    // crc
    if (buf[0] + buf[1] + buf[2] - 0xFF == 1) {
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

  if (castleRead(REG_VOLT, value1)) {
    telemetry.volt =
        smartport.formatData(VFAS_FIRST_ID, (float)value1 / 2042 * SCALE_VOLT);
    smartport.sendData(VFAS_FIRST_ID, telemetry.volt, SENSOR_ID_10);
  }
  if (castleRead(REG_CURR, value1)) {
    telemetry.curr =
        smartport.formatData(CURR_FIRST_ID, (float)value1 / 2042 * SCALE_CURR);
    smartport.sendData(CURR_FIRST_ID, telemetry.curr, SENSOR_ID_10);
  }
  if (castleRead(REG_TEMP, value1)) {
    telemetry.temperature = smartport.formatData(
        ESC_TEMPERATURE_FIRST_ID, (float)value1 / 2042 * SCALE_TEMP);
    smartport.sendData(ESC_TEMPERATURE_FIRST_ID, telemetry.temperature,
                       SENSOR_ID_10);
  }
  if (castleRead(REG_TEMP_NTC, value1)) {
    telemetry.temperatureNtc = smartport.formatData(
        ESC_TEMPERATURE_FIRST_ID + 1, (float)value1 / 2042 * SCALE_TEMP);
    smartport.sendData(ESC_TEMPERATURE_FIRST_ID + 1, telemetry.temperatureNtc,
                       SENSOR_ID_10);
  }
  if (castleRead(REG_BEC_VOLT, value1)) {
    telemetry.becVolt = smartport.formatData(VFAS_FIRST_ID + 1,
                                             (float)value1 / 2042 * SCALE_BEC);
    smartport.sendData(VFAS_FIRST_ID + 1, telemetry.becVolt, SENSOR_ID_10);
  }
  if (castleRead(REG_BEC_CURR, value1)) {
    telemetry.becCurr = smartport.formatData(CURR_FIRST_ID + 1,
                                             (float)value1 / 2042 * SCALE_BEC);
    smartport.sendData(CURR_FIRST_ID + 1, telemetry.becCurr, SENSOR_ID_10);
  }
  if (castleRead(REG_RIPPLE_VOLT, value1)) {
    telemetry.rippleVoltage = smartport.formatData(
        VFAS_FIRST_ID + 2, (float)value1 / 2042 * SCALE_RIPPLE_VOLT);
    smartport.sendData(VFAS_FIRST_ID + 2, telemetry.rippleVoltage,
                       SENSOR_ID_10);
  }
}
