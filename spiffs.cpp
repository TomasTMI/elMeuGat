#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <PNGdec.h>
//#include <Streaming.h>  //     Serial << i << " " << c << " " << t << " " << f << endl;
#include "_main.h"


bool spiffs_begin(void) {
  bool spiffs = SPIFFS.begin(true);
  if (!spiffs) Serial.println("An Error has occurred while mounting SPIFFS");
  exploraFiles();
  return spiffs;
}

Tab_nn spiffs_lectura(char* pantalla) {
  Tab_nn tab;
  File datos = SPIFFS.open(pantalla, "r");
  DynamicJsonDocument doc(4096);
  if (deserializeJson(doc, datos)) Serial.println(F("Failed to read file, using default configuration"));
  else {
    char tec[4];
    for (byte i=0; i<15; i++) {
      sprintf(tec, "t%02d", i+1);
      if (doc.containsKey(tec)) {
        strcpy(tab.tec[i][0].icon, doc[tec]["i"]);
        strcpy(tab.tec[i][0].color, doc[tec]["c"]);
        strcpy(tab.tec[i][0].text, doc[tec]["t"]);
        strcpy(tab.tec[i][0].function, doc[tec]["f"]);
      } else {
        strcpy(tab.tec[i][0].icon, "");
        strcpy(tab.tec[i][0].color, "");
        strcpy(tab.tec[i][0].text, "");
        strcpy(tab.tec[i][0].function, "");
      }
      sprintf(tec, "t%02d_", i+1);
      if (doc.containsKey(tec)) {
        strcpy(tab.tec[i][1].icon, doc[tec]["i"]);
        strcpy(tab.tec[i][1].color, doc[tec]["c"]);
        strcpy(tab.tec[i][1].text, doc[tec]["t"]);
        strcpy(tab.tec[i][1].function, doc[tec]["f"]);
      } else {
        strcpy(tab.tec[i][1].icon, "");
        strcpy(tab.tec[i][1].color, "");
        strcpy(tab.tec[i][1].text, "");
        strcpy(tab.tec[i][1].function, "");
      }
    }
  }
  return tab;
}

void exploraFiles(void) {
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  Serial.println("FILES: ");
  while(file){
    Serial.print(" - "); Serial.print(file.name()); Serial.print(" ... "); Serial.print(file.size()); Serial.println(" B");
    file = root.openNextFile();
  }
  Serial.print(" * "); Serial.print(SPIFFS.usedBytes()/1000); Serial.print(" kB / "); Serial.print(SPIFFS.totalBytes()/1000); Serial.println(" kB");
}


// *****  PNG  *****
PNG png;
int16_t xpos, ypos;
uint16_t frontColor, backColor;
File myfile;

void * myOpen(const char *filename, int32_t *size) {
  //Serial.printf("Attempting to open %s\n", filename);
  myfile = SPIFFS.open(filename, "r");
  *size = myfile.size();
  return &myfile;
}
void myClose(void *handle) {
  if (myfile) myfile.close();
}
int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length) {
  if (!myfile) return 0;
  return myfile.read(buffer, length);
}
int32_t mySeek(PNGFILE *handle, int32_t position) {
  if (!myfile) return 0;
  return myfile.seek(position);
}

void myDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[48], pixel;
  for (byte i=0; i<48; i++) {
    if (png.getPixelType() == 6) pixel = (pDraw->pPixels[i*4+3] << 8) | (pDraw->pPixels[i*4+2] & 0xff);
    else pixel = (pDraw->pPixels[i*2+1] << 8) | (pDraw->pPixels[i*2] & 0xff);
    if (pixel) lineBuffer[i] = frontColor; else lineBuffer[i] = backColor;
  }
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
/*  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);*/
}

uint16_t endian(uint16_t color) {
  byte h = color >> 8;
  byte l = color & 0x00FF;
  uint16_t out = l << 8 | h;
  return out;
}

void spiffs_icon(char* myIcon, int x, int y, uint16_t myFrontColor, uint16_t myBackColor) {
  xpos = x; ypos = y;
  frontColor = endian(myFrontColor); backColor = endian(myBackColor);
  int16_t rc = png.open(myIcon, myOpen, myClose, myRead, mySeek, myDraw);
  if (rc == PNG_SUCCESS) {
    // Serial.printf("%s: (%d x %d), %d bpp, pixelType: %d\n", myIcon, png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    png.decode(NULL, 0);
    png.close();
  }
}
