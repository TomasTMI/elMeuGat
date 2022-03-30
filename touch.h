#ifndef _ESP32_TOUCHSCREEN_H_
#define _ESP32_TOUCHSCREEN_H_
#include <Arduino.h>

#ifndef ESP32
    #error "Not an ESP32 board. Make sure the correct board is selected."
#endif

#define ADC_RESOLUTION 10 // Resolution for ESP32 ADC (default 12 bits) ... 10bits
#define NOISE_LEVEL 4  // Allow small amount of measurement noise
#define ADC_MAX ((1 << ADC_RESOLUTION)-1) // maximum value for ESP32 ADC (default 11db, 12 bits) ... 1023


class TSPoint {
public:
    TSPoint(void);
    TSPoint(int16_t x, int16_t y, int16_t z);
    int16_t x, y, z;
};

class TouchScreen {
public:
    TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx = 0);
    void calibrado(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax);
    void getTouchRaw(uint16_t &x, uint16_t &y, uint16_t &z);
    TSPoint getPoint(uint16_t x_max = 4095, uint16_t y_max = 4095);
    TSPoint getPointCal(void);

private:
    uint16_t _myAnalog(byte canal);
    uint8_t _yp, _ym, _xm, _xp, _ya, _xa;
    uint16_t _rxplate;
    uint16_t _xmin, _xmax, _ymin, _ymax;
};

#endif
