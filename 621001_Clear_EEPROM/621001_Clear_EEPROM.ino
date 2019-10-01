#include <EEPROM.h>
#define sizeBytes 2048
#define endByte 2048
#define startByte 0
unsigned long ok = 0;
unsigned long nok = 0;
unsigned long tok = 0;
void setup()
{
  Serial.begin(115200);
  EEPROM.begin(sizeBytes);
  delay(100);
  Serial.print("> Write a char(255) / hex(FF) from byte ");
  Serial.print(startByte);
  Serial.print(" to ");
  Serial.print(endByte - 1);
  Serial.print("into the EEPROM with a defined size of ");
  Serial.print(sizeBytes);
  Serial.println("testing EEPROM for written bytes");
  Serial.println("");
  for (int i = startByte; i < endByte; ++i)
  {
    if (EEPROM.read(i) == 255) {
      ++ok;
    } else {
      ++nok;
    }
  }
  Serial.printf("empty bytes: %6d\r\n", ok);
  Serial.printf("not empty bytes: %6d\r\n", nok);
  Serial.println("Start clearing EEPROM... - Please wait!!!");
  delay(1000);
  for (int i = startByte; i < endByte; ++i) {
    EEPROM.write(i, -1);
    Serial.print("Clear add 0x ");
    Serial.println(i,HEX);
  }
  EEPROM.commit();
  delay(1000);
  Serial.println("testing EEPROM for clearing");
  String test;
  for (int i = startByte; i < endByte; ++i)
  { Serial.print("Data add 0x ");
    Serial.print(i,HEX);
    Serial.print(" : ");
    Serial.println(EEPROM.read(i)); 
    if (EEPROM.read(i) == 255) {
      ++tok;
    }
  }
  if (tok = endByte - startByte) {
    Serial.println("EEPROM killed correctly");
  } else{
    Serial.println("EEPROM not killed - ERROR !!!");
  }
  
    Serial.println("OK,,, remove your ESP32.");
  }

void loop()
{
}
