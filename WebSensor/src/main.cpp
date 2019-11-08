#include <Arduino.h>

int leds[3] = {0, 16, 5};
int LDR = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LDR, INPUT);
  for (size_t i = 0; i < sizeof(leds); i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

int LDRvalor = 0;
void loop() {
  // put your main code here, to run repeatedly:
  LDRvalor = analogRead(LDR);
  Serial.println(LDRvalor);
  Serial.printf("Valor do LDR = %.1f\n", LDRvalor*3.3/4095.0);
  delay(200);
}
