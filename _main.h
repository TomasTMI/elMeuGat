#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BleKeyboard.h>
#include "touch.h"

#define VERS          "1.02"

#define TOUCH_PIN_YP  TFT_CS
#define TOUCH_PIN_XM  33
#define TOUCH_PIN_YM  TFT_D1
#define TOUCH_PIN_XP  TFT_D0

#define BACKCOLOR     TFT_BLACK
#define FRONTCOLOR    TFT_WHITE
#define ONCOLOR       TFT_MAGENTA
#define DEFAULTCOLOR  TFT_WHITE

// EEPROM
struct s_eeprom {
  uint16_t xMin;
  uint16_t xMax;
  uint16_t yMin;
  uint16_t yMax;
  };
#define EEP_LONG      sizeof(struct s_eeprom)

// Estructuras JSON
struct Tec_nn {
  char icon[21];
  char color[12];
  char text[11];
  char function[26];
};
struct Tab_nn {
  Tec_nn tec[15][2];
};


extern TFT_eSPI tft;
extern TouchScreen ts;
extern BleKeyboard bleKeyboard;
extern byte keyPress;
extern byte trabajo;
extern Tab_nn tab;
extern bool statTec[15];

void eeprom_begin(int num);
void eeprom_read(int num, byte eep[]);
void eeprom_write(int num, byte eep[]);
void eeprom_serial(struct s_eeprom eeprom);
bool spiffs_begin(void);
Tab_nn spiffs_lectura(char* pantalla);
void exploraFiles(void);
void spiffs_icon(char* myIcon, int x, int y, uint16_t myFrontColor, uint16_t myBackColor);

Tab_nn SD_begin(void);
void sueltaKey();
byte teclaNum(int x, int y);
TSPoint cal(byte calibra);
void tablero(void);
void tecla_off(byte n);
void tecla_on(byte n);
void tecla(byte n, uint16_t myBackColor);
void dibuja(byte n, int x, int y, uint16_t myBackColor);
void sendKey(byte n);
void bleKeyboad(String function);
uint16_t fColor(String myColor);
