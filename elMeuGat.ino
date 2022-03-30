/*
  Simulación de keyboard ElGato
  SD: 128 MB
  Arduino UNO: 32k / 2k ... excedido
  ESP32:  1M3 / 327k    ... BT ***

  Notes:
    - User_Setup.h modificat senser
    - Touch_Setup.h comentat ESP32_WIFI_TOUCH
*/


#include "_main.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


// Variables
TFT_eSPI tft = TFT_eSPI();
TouchScreen ts(TOUCH_PIN_XP, TOUCH_PIN_YP, TOUCH_PIN_XM, TOUCH_PIN_YM, 300);
BleKeyboard bleKeyboard;
byte keyPress;
byte trabajo = 2;   // 2:Cal(1"+c1+c2) ... 1:Tablero ... 0:Keys
Tab_nn tab;
bool statTec[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
char msg[30]; //sprintf(msg, "X:%d  Y:%d  Z:%d", p.x, p.y, p.z); Serial.println(msg);
union {
  struct s_eeprom eeprom;
  byte eep[EEP_LONG];
  } mem;


// *****  MAIN  *****
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, LOW);
  while (!Serial) delay(1); Serial.println(F("LCD_keyboard!"));
  eeprom_begin(EEP_LONG);
  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(BACKCOLOR);
  spiffs_begin();
  tab = spiffs_lectura("/_pantMain.json");
  bleKeyboard.begin();
}

void loop() {
  loopCalibra();
  loopTeclado();
}

void loopCalibra(void) {
  TSPoint p;
  if (trabajo == 2) {
    p = cal(0);
    if (p.z) {
      p = cal(1);
      mem.eeprom.xMin = p.x;
      mem.eeprom.yMin = p.y;
      p = cal(2);
      mem.eeprom.xMax = p.x;
      mem.eeprom.yMax = p.y;
      eeprom_write(EEP_LONG, mem.eep);
    }
    eeprom_read(EEP_LONG, mem.eep); eeprom_serial(mem.eeprom);
    ts.calibrado(mem.eeprom.xMin, mem.eeprom.xMax, mem.eeprom.yMin, mem.eeprom.yMax);
    trabajo = 1;
  }
  if (trabajo == 1) {
    digitalWrite(LED_BUILTIN, LOW);
    tft.fillScreen(BACKCOLOR);
    tablero();
    sueltaKey();
    keyPress = 0;
    trabajo = 0;
  }
}

void loopTeclado(void) {
  static unsigned long timeAct = 0;
  TSPoint p = ts.getPoint(tft.width(), tft.height());
  if (p.z) timeAct = millis();
  if (p.z && !keyPress) {
    digitalWrite(LED_BUILTIN, HIGH);
    byte n = teclaNum(p.x, p.y);
    tecla_on(n);
    sendKey(n);
  }
  if (!p.z && keyPress && millis()-timeAct>100) {
    digitalWrite(LED_BUILTIN, LOW);
    tecla_off(keyPress);
  }
}
