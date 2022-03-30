#include "_main.h"

#define KEY_X         64      // 320/5
#define KEY_Y         80      // 240/3
#define KEY_RADIUS    5


// *****  Funciones de Touch  *****
// Tecla suelta
void sueltaKey() {
  TSPoint p;
  unsigned long timeMem = millis();
  do {
    p = ts.getPointCal(); if (p.z) timeMem = millis();
  } while (p.z || millis()-timeMem<100);
}

// Tecla número
byte teclaNum(int x, int y) {
  byte n = 1;
  if (x > KEY_X)   n += 1;
  if (x > KEY_X*2) n += 1;
  if (x > KEY_X*3) n += 1;
  if (x > KEY_X*4) n += 1;
  if (y > KEY_Y)   n += 5;
  if (y > KEY_Y*2) n += 5;
  return n;
}

// *****  Pantalla Calibración  *****
// Versión + Cal1 + Cal2
TSPoint cal(byte calibra) {
  TSPoint p;
  static unsigned long timeAct = 0;
  if (calibra == 0) {
    tft.setCursor(295,230); tft.print(VERS);
    timeAct = millis();
    do { p = ts.getPointCal(); } while (!p.z && millis()-timeAct<1000);
  } else if (calibra == 1) {
    tft.fillScreen(BACKCOLOR);
    tft.fillRect(0, 0, 4, 4, FRONTCOLOR);
    sueltaKey();
    do { p = ts.getPointCal(); } while (!p.z || p.x > 200);
  } else {
    tft.fillScreen(BACKCOLOR);
    tft.fillRect(319-4, 239-4, 4, 4, FRONTCOLOR);
    sueltaKey();
    do { p = ts.getPointCal(); } while (!p.z || p.x < 800);
  }
  return p;
}

// *****  Pantalla Tablero  *****
// Dibuja TABLERO
void tablero(void) {
  for (byte n=1; n<=15; n++) {
    tecla_off(n);
  }
}

// Tecla OFF
void tecla_off(byte n) {
  keyPress = 0;
  tecla(n, BACKCOLOR);
}

// Tecla ON
void tecla_on(byte n) {
  if (!keyPress) {
    keyPress = n;
    tecla(n, ONCOLOR);
  }
}

// TECLA (on, off)
void tecla(byte n, uint16_t myBackColor) {
  int x, y;
  n -= 1; x = n%5; y = n/5; x = x*KEY_X; y = y*KEY_Y;
  tft.fillRoundRect(x, y, KEY_X, KEY_Y, KEY_RADIUS, myBackColor);
  tft.drawRoundRect(x, y, KEY_X, KEY_Y, KEY_RADIUS, FRONTCOLOR);
  dibuja(n+1, x, y, myBackColor);
}

// Dibuja interior TECLA
void dibuja(byte n, int x, int y, uint16_t myBackColor) {
  String ico = tab.tec[n-1][statTec[n-1]].icon;
  String txt = tab.tec[n-1][statTec[n-1]].text;
  String col = tab.tec[n-1][statTec[n-1]].color;
  uint16_t myFrontColor = fColor(col);
  if (ico != "")
    spiffs_icon(tab.tec[n-1][statTec[n-1]].icon, x+8, y+8, myFrontColor, myBackColor);
  if (txt != "") {
    x += (32 - ((int)((float)txt.length()/2.0*6.0))); y += 63;
    tft.setTextColor(myFrontColor, myBackColor);
    tft.setCursor(x,y); tft.print(txt);
  }
}

// *****  Send Keyboard  *****
void sendKey(byte n) {
  String function = tab.tec[n-1][statTec[n-1]].function;
  if (function != "") {
    if (function.indexOf("_pant") != -1) {
      tab = spiffs_lectura(tab.tec[n-1][statTec[n-1]].function);
      sueltaKey();
      trabajo = 1;
    } else bleKeyboad(function);
    function = tab.tec[n-1][1].function;
    if (function != "") {
      statTec[n-1] = !statTec[n-1];
      sueltaKey();
      tecla_off(n);
    }
  }
}

void bleKeyboad(String function) {
  bool ctrl=false, shift=false, alt=false;
  if (function.indexOf("Ctrl+") != -1) { function.replace("Ctrl+", ""); ctrl = true; }
  if (function.indexOf("Shift+") != -1) { function.replace("Shift+", ""); shift = true; }
  if (function.indexOf("Alt+") != -1) { function.replace("Alt+", ""); alt = true; }

  if (ctrl) bleKeyboard.press(KEY_LEFT_CTRL);
  if (shift) bleKeyboard.press(KEY_LEFT_SHIFT);
  if (alt) bleKeyboard.press(KEY_LEFT_ALT);

  // F1...F12
  if (function.indexOf("F") != -1) {
    // if (ctrl) Serial.print("Ctrl+"); if (shift) Serial.print("Shift+"); if (alt) Serial.print("Alt+"); Serial.println(function);
    function.replace("F", "");
    int n = function.toInt() - 1;
    bleKeyboard.press(KEY_F1 + n);
  } else {
    char buf[20];
    function.toCharArray(buf, function.length());
    bleKeyboard.print(buf);
    // Serial.println(buf);
  }
  
  if (ctrl || shift || alt) {
    delay(100);
    bleKeyboard.releaseAll();
  }
}


// *****  colors  *****
// Default color definitions
String colorStr[] =   { "black",    "navy",     "darkgreen",      "darkcyan",   "maroon",
                        "purple",   "olive",    "lightgrey",      "darkgrey",   "blue",
                        "green",    "cyan",     "red",            "magenta",    "yellow",
                        "white",    "orange",   "greenyellow",    "pink",       "brown",
                        "gold",     "silver",   "skyblue",        "violet"
                      };
uint16_t colorInt[] = { TFT_BLACK,  TFT_NAVY,   TFT_DARKGREEN,    TFT_DARKCYAN, TFT_MAROON,
                        TFT_PURPLE, TFT_OLIVE,  TFT_LIGHTGREY,    TFT_DARKGREY, TFT_BLUE,
                        TFT_GREEN,  TFT_CYAN,   TFT_RED,          TFT_MAGENTA,  TFT_YELLOW,
                        TFT_WHITE,  TFT_ORANGE, TFT_GREENYELLOW,  TFT_PINK,     TFT_BROWN,
                        TFT_GOLD,   TFT_SILVER, TFT_SKYBLUE,      TFT_VIOLET
                      };
                        
uint16_t fColor(String myColor) {
  uint16_t toReturn = DEFAULTCOLOR;
  if (myColor) {
    int i;
    int limit = sizeof(colorInt)/2;
    for (i=0; i<limit; i++) {
      if (colorStr[i] == myColor) {
        toReturn = colorInt[i];
        break;
      }
    }
  }
  return toReturn;
}
