#ifndef GT911_LITE_H
#define GT911_LITE_H

#include "Arduino.h"
#include <Wire.h>

#define GT911_ADDR1 (uint8_t)0x5D
#define GT911_ADDR2 (uint8_t)0x14

#define GT911_POINT_INFO (uint16_t)0x814E
#define GT911_POINT_1    (uint16_t)0x814F
#define GT911_POINT_2    (uint16_t)0x8157
#define GT911_POINT_3    (uint16_t)0x815F
#define GT911_POINT_4    (uint16_t)0x8167
#define GT911_POINT_5    (uint16_t)0x816F
#define GT911_POINTS_REG {GT911_POINT_1, GT911_POINT_2, GT911_POINT_3, GT911_POINT_4, GT911_POINT_5}

class TP_Point {
  public:
    TP_Point(void);
    TP_Point(uint8_t id, uint16_t x, uint16_t y, uint16_t size);

    bool operator==(TP_Point);
    bool operator!=(TP_Point);

    uint8_t id;
    uint16_t x;
    uint16_t y;
    uint16_t size;
};

class GT911_Lite {
  public:
    GT911_Lite();
    void begin(TwoWire *wire);
    bool read(void);
    void setResolution(uint16_t width, uint16_t height);  // NEW METHOD

    uint8_t isLargeDetect;
    uint8_t touches = 0;
    bool isTouched = false;
    bool down = false;
    TP_Point points[5];

    uint16_t x;
    uint16_t y;
    uint16_t size;

    uint16_t last_x;
    uint16_t last_y;
    uint16_t last_size;
    bool last_down;

    // MOVED TO PUBLIC (from private)
    void writeByteData(uint16_t reg, uint8_t val);
    uint8_t readByteData(uint16_t reg);
    void readBlockData(uint16_t reg, uint8_t *buf, uint8_t size);

  private:
    TP_Point readPoint(uint8_t *data);
    uint8_t addr;
    TwoWire *_wire;
};

#endif // GT911_LITE_H