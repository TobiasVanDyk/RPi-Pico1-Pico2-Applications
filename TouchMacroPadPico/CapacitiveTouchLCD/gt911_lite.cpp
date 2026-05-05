#include "Arduino.h"
#include "gt911_lite.h"

#include <Wire.h>

GT911_Lite::GT911_Lite() {
}

void GT911_Lite::begin(TwoWire *wire) {
  _wire = wire;

  _wire->beginTransmission(GT911_ADDR1);
  if (_wire->endTransmission() == 0) {
    addr = GT911_ADDR1;
    Serial.println("detected ADDR1");
  } else {
    _wire->beginTransmission(GT911_ADDR2);
    if (_wire->endTransmission() == 0) {
      addr = GT911_ADDR2;
      Serial.println("detected ADDR2");
    } else {
      Serial.println("not detected");
      return;
    }
  }
}

void GT911_Lite::setResolution(uint16_t width, uint16_t height) {
  // Set GT911 firmware registers to match LCD dimensions
  writeByteData(0x8048, lowByte(width));    // X_OUTPUT_MAX_LOW
  writeByteData(0x8049, highByte(width));   // X_OUTPUT_MAX_HIGH
  writeByteData(0x804A, lowByte(height));   // Y_OUTPUT_MAX_LOW
  writeByteData(0x804B, highByte(height));  // Y_OUTPUT_MAX_HIGH
  
  // Refresh config in GT911
  writeByteData(0x8100, 1);  // GT911_CONFIG_FRESH
  delay(100);
}

bool GT911_Lite::read() {
  uint8_t pointInfo = readByteData(GT911_POINT_INFO);
  uint8_t bufferStatus = (pointInfo >> 7) & 1;
  if (!bufferStatus) {
    return false;
  }

  writeByteData(GT911_POINT_INFO, 0);

  touches = pointInfo & 0x0F;
  isLargeDetect = (pointInfo >> 6) & 1;
  isTouched = (touches > 0);

  uint16_t pointRegs[] = GT911_POINTS_REG;
  for (uint8_t i = 0; i < touches && i < 5; i++) {
    uint8_t data[8];
    readBlockData(pointRegs[i], data, 8);
    points[i] = readPoint(data);
  }

  if (touches > 0) {
    down = true;
    x = points[0].x;
    y = points[0].y;
    size = points[0].size;
  } else {
    down = false;
    x = last_x;
    y = last_y;
    size = last_size;
  }

  bool changed = x != last_x || y != last_y || size != last_size || down != last_down;

  last_x = x;
  last_y = y;
  last_size = size;
  last_down = down;

  return changed;
}

TP_Point GT911_Lite::readPoint(uint8_t *data) {
  uint8_t id = data[0];
  uint16_t x = data[1] + (data[2] << 8);
  uint16_t y = data[3] + (data[4] << 8);
  uint16_t size = data[5] + (data[6] << 8);

  return TP_Point(id, x, y, size);
}

void GT911_Lite::writeByteData(uint16_t reg, uint8_t val) {
  _wire->beginTransmission(addr);
  _wire->write(highByte(reg));
  _wire->write(lowByte(reg));
  _wire->write(val);
  _wire->endTransmission();
}

uint8_t GT911_Lite::readByteData(uint16_t reg) {
  _wire->beginTransmission(addr);
  _wire->write(highByte(reg));
  _wire->write(lowByte(reg));
  _wire->endTransmission(false);  // repeated start

  if (_wire->requestFrom(addr, (uint8_t)1) != 1) {
    return 0;
  }
  return _wire->read();
}

void GT911_Lite::readBlockData(uint16_t reg, uint8_t *buf, uint8_t size) {
  _wire->beginTransmission(addr);
  _wire->write(highByte(reg));
  _wire->write(lowByte(reg));
  _wire->endTransmission(false);  // repeated start

  uint8_t received = _wire->requestFrom(addr, size);
  if (received != size) {
    return;
  }

  for (uint8_t i = 0; i < size && _wire->available(); i++) {
    buf[i] = _wire->read();
  }
}

// TP_Point implementation

TP_Point::TP_Point()
  : id(0), x(0), y(0), size(0) {
}

TP_Point::TP_Point(uint8_t _id, uint16_t _x, uint16_t _y, uint16_t _size)
  : id(_id), x(_x), y(_y), size(_size) {
}

bool TP_Point::operator==(TP_Point point) {
  return (point.x == x) && (point.y == y) && (point.size == size);
}

bool TP_Point::operator!=(TP_Point point) {
  return (point.x != x) || (point.y != y) || (point.size != size);
}