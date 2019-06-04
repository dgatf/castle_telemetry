#include "Smartport.h"

Smartport::Smartport(Stream &serial) : _serial(serial) {
  pinMode(LED_SMARTPORT, OUTPUT);
}

void Smartport::sendByte(uint8_t c, uint16_t *crcp) {
  uint16_t crc = *crcp;

  if (crcp != NULL) {
    crc += c;
    crc += crc >> 8;
    crc &= 0x00FF;
    *crcp = crc;
  }

  if (c == 0x7D || c == 0x7E) {
    _serial.write(0x7D);
    c ^= 0x20;
  }

  _serial.write(c);
}

void Smartport::sendData(uint16_t dataId, uint32_t val, uint8_t sensorId) {
  while (_serial.available()) {
    _serial.read();
  }
  while (1) {
    while (!_serial.available()) {
    }
    if (_serial.read() == 0x7E) {
      while (!_serial.available()) {
      }
      if (_serial.read() == sensorId) {
        sendData(dataId, val);
        return;
      }
    }
  }
}

void Smartport::sendData(uint16_t dataId, uint32_t val) {
  digitalWrite(LED_SMARTPORT, HIGH);
  uint16_t crc = 0;
  uint8_t *u8p;
  // type
  sendByte(0x10, &crc);
  // dataId
  u8p = (uint8_t *)&dataId;
  sendByte(u8p[0], &crc);
  sendByte(u8p[1], &crc);
  // val
  u8p = (uint8_t *)&val;
  sendByte(u8p[0], &crc);
  sendByte(u8p[1], &crc);
  sendByte(u8p[2], &crc);
  sendByte(u8p[3], &crc);
  // crc
  sendByte(0xFF - (uint8_t)crc, NULL);
  digitalWrite(LED_SMARTPORT, LOW);
}

void Smartport::sendVoid() {
  uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
  _serial.write(buf, 8);
}

void Smartport::sendVoid(uint8_t sensorId) {
  while (_serial.available()) {
    _serial.read();
  }
  while (1) {
    while (!_serial.available()) {
    }
    if (_serial.read() == 0x7E) {
      while (!_serial.available()) {
      }
      if (_serial.read() == sensorId) {
        sendVoid();
        return;
      }
    }
  }
}

uint32_t Smartport::formatData(uint16_t dataId, float value) {

  if ((dataId >= GPS_SPEED_FIRST_ID && dataId <= GPS_SPEED_LAST_ID) ||
      (dataId >= RBOX_BATT1_FIRST_ID && dataId <= RBOX_BATT2_FIRST_ID))
    return value * 1000;

  if ((dataId >= ALT_FIRST_ID && dataId <= VARIO_LAST_ID) ||
      (dataId >= VFAS_FIRST_ID && dataId <= VFAS_LAST_ID) ||
      (dataId >= ACCX_FIRST_ID && dataId <= GPS_ALT_LAST_ID) ||
      (dataId >= GPS_COURS_FIRST_ID && dataId <= GPS_COURS_LAST_ID) ||
      (dataId >= A3_FIRST_ID && dataId <= A4_LAST_ID))
    return value * 100;

  if ((dataId >= CURR_FIRST_ID && dataId <= CURR_LAST_ID) ||
      (dataId >= AIR_SPEED_FIRST_ID && dataId <= AIR_SPEED_LAST_ID) ||
      dataId == A1_ID || dataId == A2_ID || dataId == RXBT_ID)
    return value * 10;

  return value;
}

uint32_t Smartport::formatEscPower(float volt, float curr) {
  return (uint32_t)(curr * 100) << 16 | (uint16_t)(volt * 100);
}

uint32_t Smartport::formatEscRpmCons(float rpm, float cons) {
  return (uint32_t)cons << 16 | (uint16_t)(rpm / 100);
}

uint32_t Smartport::formatCell(uint8_t cellId, float val) {
  val *= 500;
  return (uint8_t)val << 8 | cellId;
}
