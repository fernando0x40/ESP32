#include <Arduino.h>
#include <LDR.h>

#define LDR_Pin 4
#define MAX_ANALOG 4095

int leds[3] = {0, 16, 5};
LDR *ldr;

void setup() {
  // put your setup code here, to run once:
  ldr = new LDR(LDR_Pin, MAX_ANALOG);
  Serial.begin(115200);
  pinMode(ldr->get_Input_Pin(), INPUT);
  for (size_t i = 0; i < sizeof(leds); i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("Valor do LDR = %.2f%%\n", ldr->get_Percent(analogRead));
  delay(30);
}
