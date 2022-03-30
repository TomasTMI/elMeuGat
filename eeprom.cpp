/**/


#include <EEPROM.h>
#include "_main.h"


/********************************/
void eeprom_begin(int num) {
  EEPROM.begin(num);
}


void eeprom_read(int num, byte eep[]) {
  for (int i=0; i<num; i++) eep[i] = byte(EEPROM.read(i));
}


void eeprom_write(int num, byte eep[]) {
  for (int i=0; i<num; i++) EEPROM.write(i, eep[i]);
  EEPROM.commit();
}

void eeprom_serial(struct s_eeprom eeprom) {
  Serial.printf("*EEPROM: min: %d, %d ... max: %d, %d\n", eeprom.xMin, eeprom.yMin, eeprom.xMax, eeprom.yMax);
}
