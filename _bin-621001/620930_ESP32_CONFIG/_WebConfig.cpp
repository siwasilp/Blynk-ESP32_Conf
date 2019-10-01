#include "_WebConfig.h"
#include "_config_pages.h"
DNSServer dnsServer;
WiFiServer server(80);
WebServer server1(8080);
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 10, 1);
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
void ESPConf::begin() {
  if (checkSetup())
    run();
  else
    wizard();
}
void ESPConf::begin(String mode) {
  setMode(mode);
  if (checkSetup())
    run();
  else
    wizard();
}
void ESPConf::init() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("failed to initialise EEPROM");
    delay(1000000);
  }
  WiFi.mode(WIFI_AP_STA);
  apNameAddr = 0;
  apPasswdAddr = 30;
  wifiSSIDAddr = 60;
  wifiPasswdAddr = 90;
  bTokenAddr = 120;
}
void ESPConf::setAPSSID(String name) {
  pushArgs(name, apNameAddr);
}
void ESPConf::setAPPasswd(String passwd) {
  pushArgs(passwd, apPasswdAddr);
}
void ESPConf::setWiFiSSID(String name) {
  pushArgs(name, wifiSSIDAddr);
}
void ESPConf::setWiFiPasswd(String passwd) {
  pushArgs(passwd, wifiPasswdAddr);
}
void ESPConf::setBToken(String token) {
  pushArgs(token, bTokenAddr);
}
void ESPConf::setResetButton(uint8_t pin, bool on) {
  resetButton = pin;
  resetButtonOn = on;
}

bool ESPConf::getCheckSetup() {
  return checkSetup();
}

String ESPConf::getAPSSID() {
  String ret = EEPROM.readString(apNameAddr);
  ret.trim();
  return ret;
}
String ESPConf::getAPPasswd() {
  String ret =  EEPROM.readString(apPasswdAddr);
  ret.trim();
  return ret;
}
String ESPConf::getWiFiSSID() {
  String ret =  EEPROM.readString(wifiSSIDAddr);
  ret.trim();
  return ret;
}
String ESPConf::getWiFiPasswd() {
  String ret =  EEPROM.readString(wifiPasswdAddr);
  ret.trim();
  return ret;
}
String ESPConf::getBToken() {
  String ret =  EEPROM.readString(bTokenAddr);
  ret.trim();
  return ret;
}
void ESPConf::handle() {
  dnsServer.processNextRequest();
  WiFiClient client = server.available();   
  if (client) {
    String header = "";
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(Header_Page);
            client.print("<label>AP SSID <abel>");
            client.print("<input type=\"text\" name=\"apName\" placeholder=\"Enter AP_SSID\" value=\"" + getAPSSID() + "\">");
            client.print("<label>AP Password</label>");
            client.print("<input type=\"password\"  name=\"apPasswd\" placeholder=\"Enter AP Password\" value=\"" + getAPPasswd() + "\">");
            client.print("<label>WiFi SSID</label>");
            client.print("<input type=\"text\" name=\"wifiName\" placeholder=\"Enter WiFi_SSID\" value=\"" + getWiFiSSID() + "\">");
            client.print("<label>WiFi Password</label>");
            client.print("<input type=\"password\"  name=\"wifiPasswd\" placeholder=\"Enter WiFi Password\" value=\"" + getWiFiPasswd() + "\">");
            client.print("<label>Blynk token key</label>");
            client.print("<input type=\"text\"  name=\"bToken\" placeholder=\"Enter Blynk token\" value=\"" + getBToken() + "\">");
            client.print(Footer_Page);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
  server1.handleClient();
}
void ESPConf::pushArgs(String arg, int addr) {
  EEPROM.writeString(addr, arg);
  EEPROM.commit();
}
void ESPConf::run() {
  CreateAP();
  if (Mode != "Blynk")
  WiFiConnect();
  Serial.println("Initial setup !!!");
  Serial.print("WiFi SSID   = ");
  Serial.println(getWiFiSSID());
  Serial.print("WiFi PASS   = ");
  Serial.println(getWiFiPasswd());
  Serial.print("Blynk token = ");
  Serial.println(getBToken());
}
void ESPConf::setMode(String mode) {
  Mode = mode;
}
void ESPConf::wizard() {
  setAPSSID("ESPConfIoTWebConfig");
  CreateAP();
  Serial.println("Initial setup !!!");
  Serial.print("AP Name = ");
  Serial.println(getAPSSID());
}
void ESPConf::WiFiConnect() {
  WiFi.begin(getWiFiSSID().c_str(), getWiFiPasswd().c_str());
  while (WiFi.status() != WL_CONNECTED);
}
void ESPConf::CreateAP() {
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  if (getAPPasswd() == "\0")
    WiFi.softAP(getAPSSID().c_str());
  else
    WiFi.softAP(getAPSSID().c_str(), getAPPasswd().c_str());
  startWebServer();
}
bool ESPConf::checkSetup() {
  if (getAPSSID() != "\0" && getAPPasswd() != "\0" && getWiFiSSID() != "\0" && getWiFiPasswd() != "\0")
    return	true;

  return false;
}
void ESPConf::startWebServer() {
  dnsServer.start(DNS_PORT, "*", apIP);
  server.begin();
  server1.on("/config", HTTP_POST, []() {
    EEPROM.writeString(0,   server1.arg("apName"));
    EEPROM.writeString(30,  server1.arg("apPasswd"));
    EEPROM.writeString(60,  server1.arg("wifiName"));
    EEPROM.writeString(90,  server1.arg("wifiPasswd"));
    EEPROM.writeString(120, server1.arg("bToken"));
    EEPROM.commit();
    server1.send(200, "text/html", "<center><h2>Update configure success. REBOOT!!! </h2></center>");
    delay(4000);
    ESP.restart();
  });
  server1.begin();
}
void IRAM_ATTR ESPConf::resetSys() {
  portENTER_CRITICAL_ISR(&mux);
  EEPROM.writeString(0 , "\0");
  EEPROM.writeString(30, "\0");
  EEPROM.writeString(60, "\0");
  EEPROM.writeString(90, "\0");
  EEPROM.writeString(120, "\0");
  EEPROM.commit();
  portEXIT_CRITICAL_ISR(&mux);
  Serial.println("System reset in 4 s.");
  delay(4000);
  ESP.restart();
}
