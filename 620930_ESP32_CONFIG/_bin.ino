#define led_sts   LED_BUILTIN
#define sw_reset  0 //botton BOOT

void setup_loop() {
  pinMode(led_sts , OUTPUT);
  pinMode(sw_reset, INPUT_PULLUP);
  espConf.init();
  Serial.println();
  for (int i = 15 ; i > 0 ; i--) {
    digitalWrite(led_sts, HIGH);
    delay(250);
    Serial.print("Boot > ");
    Serial.print(i);
    Serial.println("\tConf Pls Push [pb_BOOT]");
    digitalWrite(led_sts, LOW);
    delay(250);
    if (digitalRead(sw_reset) == 0) {
      Serial.println("CONFIG MODE...");
      Serial.println("Initial setup !!!");
      Serial.print("AP Name = ");
      Serial.println(espConf.getAPSSID());
      Serial.print("AP Pass = ");
      Serial.println(espConf.getAPPasswd());
      espConf.CreateAP();
      reset = 1; i = 0;
    }
  }
  if (reset == 0) {
    espConf.begin("Blynk");
    setupStatus = espConf.getCheckSetup();
  }
}
//-------------------------------------------------------------
void Config() {
  espConf.handle();
  
  digitalWrite(led_sts, LOW);     delay(90);
  digitalWrite(led_sts, HIGH);    delay(90);
  digitalWrite(led_sts, LOW);     delay(90);
  digitalWrite(led_sts, HIGH);    delay(90);
  digitalWrite(led_sts, LOW);     delay(180);
}
