#include <EEPROM.h>
void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  for (int i = 0 ; i < 2048 ; i++) {
    EEPROM.write(i, 0);
    Serial.print("CLEAR add 0x");
    Serial.println(i,HEX);
  }
  Serial.println("ok...");
}
void loop() {}
