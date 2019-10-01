#include "_WebConfig.h"
#define  BLYNK_PRINT Serial
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
bool setupStatus = false;
int reset = 0;

ESPConf espConf;

void setup() {
  Serial.begin(115200);
  setup_loop();
  if (!reset) {
    char auth[espConf.getBToken().length() + 1] ;
    espConf.getBToken().toCharArray(auth, espConf.getBToken().length() + 1);
    char ssid[espConf.getWiFiSSID().length() + 1] ;
    espConf.getWiFiSSID().toCharArray(ssid, espConf.getWiFiSSID().length()  + 1);
    char pass[espConf.getWiFiPasswd().length() + 1];
    espConf.getWiFiPasswd().toCharArray(pass, espConf.getWiFiPasswd().length() + 1);
    Blynk.begin(auth, ssid, pass);
  }
}

void loop() {
  if (reset) Config();
  else    Blynk.run();
}
