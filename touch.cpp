#include "touch.h"

//#define TFT_RD      25
#define NUMSAMPLES  2


TSPoint::TSPoint(void) {
    x = y = 0;
}

TSPoint::TSPoint(int16_t x0, int16_t y0, int16_t z0) {
  x = x0;
  y = y0;
  z = z0;
}

//
// Touch Screen
//
TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rxplate) {
  _yp = yp;
  _xm = xm;
  _ym = ym;
  _xp = xp;
  _xa = _xm;
  _ya = _yp;
  _rxplate = rxplate;
  analogReadResolution(ADC_RESOLUTION);
}

void TouchScreen::calibrado(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax) {
  _xmin = xmin;
  _xmax = xmax;
  _ymin = ymin;
  _ymax = ymax;
}

TSPoint TouchScreen::getPointCal(void) {
  uint16_t x, y, z;
  getTouchRaw(x,y,z);
  return TSPoint(x,y,z);
}

TSPoint TouchScreen::getPoint(uint16_t x_max, uint16_t y_max) {
  uint16_t x, y, z;
  int16_t x2, y2;
  getTouchRaw(x,y,z);
  x2 = map(x, _xmin, _xmax, 0, x_max); if (x2<0) x=0; else if (x2>x_max) x=x_max; else x=x2;
  y2 = map(y, _ymin, _ymax, 0, y_max); if (y2<0) y=0; else if (y2>y_max) y=y_max; else y=y2;
  return TSPoint(x,y,z);
}

/***************************************************************************************
** Function name:           getTouchRaw
** Description:             read raw touch position.  Always returns true.
***************************************************************************************/
void TouchScreen::getTouchRaw(uint16_t &x, uint16_t &y, uint16_t &z) {
  //digitalWrite(TFT_RD, HIGH);
  pinMode(_yp, INPUT); pinMode(_ym, INPUT); pinMode(_xp, OUTPUT); pinMode(_xm, OUTPUT);
  digitalWrite(_xp, HIGH); digitalWrite(_xm, LOW);
  x = _myAnalog(_ya);
//############################################################
  pinMode(_xp, INPUT); pinMode(_xm, INPUT); pinMode(_yp, OUTPUT); pinMode(_ym, OUTPUT);
  digitalWrite(_yp, HIGH); digitalWrite(_ym, LOW);
  y = _myAnalog(_xa);
//############################################################
  // Set X+ to ground // Set Y- to VCC // Hi-Z X- and Y+
  pinMode(_xp, OUTPUT); pinMode(_yp, INPUT);
  digitalWrite(_ym, HIGH); digitalWrite(_xp, LOW);
  int z1 = _myAnalog(_xa);
  int z2 = _myAnalog(_ya);
  if (z1 == 0 && z2 == ADC_MAX) z = 0;
  else {
    if (_rxplate != 0) {
      float rtouch;
      rtouch = z2;
      rtouch /= z1;
      rtouch -= 1;
      rtouch *= x;
      rtouch *= _rxplate;
      rtouch /= ADC_MAX+1;
      z = rtouch;
    } else
      z = (ADC_MAX-(z2-z1));
    if (z == 65535) z = 0;
  }
//############################################################
  // Restore pins LCD
  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);
//############################################################
  // Cambio de coordenadas (Rotation=1)
  uint16_t tx = x; uint16_t ty = y;
  y = ADC_MAX-tx; x = ty;
}

uint16_t TouchScreen::_myAnalog(byte canal) {
  uint16_t samples[NUMSAMPLES];
  bool valid;
  do {
    valid = true;
    delay(1); samples[0] = analogRead(canal);
    delay(1); samples[1] = analogRead(canal);
    if (samples[0] - samples[1] < -NOISE_LEVEL || samples[0] - samples[1] > NOISE_LEVEL) valid = false;
  } while (valid == false);
  samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
  return samples[1];
}
