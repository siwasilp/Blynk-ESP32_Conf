#ifndef ESP32
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include <EEPROM.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#define EEPROM_SIZE 2048

class ESPConf
{
  public:
    void begin();
    void begin(String Mode);
    void init();
    void setResetButton(uint8_t pin, bool on);
    void setAPSSID(String name);
    void setAPPasswd(String passwd);
    void setWiFiSSID(String name);
    void setWiFiPasswd(String passwd);
    void setBToken(String token);
    void handle();
    void wizard();
    void CreateAP();
    bool getCheckSetup();
    String getAPSSID();
    String getAPPasswd();
    String getWiFiSSID();
    String getWiFiPasswd();
    String getBToken();

  private:
    int apNameAddr;
    int apPasswdAddr;
    int wifiSSIDAddr;
    int wifiPasswdAddr;
    int bTokenAddr;
    uint8_t resetButton = 0;
    bool resetButtonOn = HIGH;
    String Mode = "blynk";
    void pushArgs(String arg, int addr);
    void run();
    void setMode(String Mode);
    void startWebServer();
    bool checkSetup();
    void WiFiConnect();
    static void resetSys();
};
