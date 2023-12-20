#include <Arduino.h>
#include "notas.h"

const int LED1 = 5;
const int LED2 = 6;
const int LED3 = 7;
const int LDR = A1;
const int LDR2 = A2;
const int TRIGGER = 12;
const int LASER = 4;
const int BUZZER = 3;

bool laserON = false;
int VIDAS = 4;
int LDRMin = 1000; //VARIA SEGUN ENTORNO
int duracionNota = 100;
int resetG = 0;

const int numReadings = 10;
int readings[numReadings];
int index = 0;
const int numReadings2 = 10;
int readings2[numReadings2];
int index2 = 0;

void setup() {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LDR, INPUT);
    pinMode(LDR2, INPUT);
    pinMode(TRIGGER, INPUT_PULLUP);
    pinMode(LASER, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    digitalWrite(LASER, LOW);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    Serial.begin(9600);
}

void loop() {
    LDRValor();
    Serial.println(VIDAS);
    if (digitalRead(TRIGGER) == LOW) {
        if (!laserON) {
            EncenderLaser();
        } 
    }
    else{
        ApagarLaser();
    } 
    if ( (analogRead(LDR) <= LDRMin)) {
        ContVIDAS();
        delay(1000);
    }
}



void LDRValor() {
  int ldrRawValue = analogRead(LDR);
  int ldrRawValue2 = analogRead(LDR2);

  readings[index] = ldrRawValue;
  readings2[index] = ldrRawValue2;

  index = (index + 1) % numReadings;
  index2 = (index2 + 1) % numReadings2;

  int ldrSmoothedValue = 0;
  for (int i = 0; i < numReadings; i++) {
    ldrSmoothedValue += readings[i];
  }
  int ldrSmoothedValue2 = 0;
  for (int i = 0; i < numReadings2; i++) {
    ldrSmoothedValue2 += readings2[i];
  }
  ldrSmoothedValue /= numReadings;
  ldrSmoothedValue2 /= numReadings2;
  
  Serial.print("Smoothed LDR Value: ");
  Serial.println(ldrSmoothedValue);
  Serial.print("Smoothed LDR 2 Value: ");
  Serial.println(ldrSmoothedValue2);
}

void EncenderLaser() {
    int duracionNota = 25;
    int melodiaDisparo[] = {NOTE_D3, NOTE_A3, NOTE_D4, NOTE_A4, NOTE_D5};
    laserON = true;
    digitalWrite(LASER, HIGH);
  
    for (int i = 0; i < sizeof(melodiaDisparo) / sizeof(melodiaDisparo[0]); i++) {
        tone(BUZZER, melodiaDisparo[i], duracionNota);
        delay(duracionNota);
    }
    if ( (analogRead(LDR) <= LDRMin)) {
        ContVIDAS();
        delay(1000);
    }
    if ( (analogRead(LDR2) <= LDRMin)) {
        ContVIDAS();
        delay(1000);
    }
    digitalWrite(LASER, LOW);
    noTone(BUZZER);
    delay(100);
}

void ApagarLaser() {
    laserON = false;
    digitalWrite(LASER, LOW);
}

void ContVIDAS() {
    switch (VIDAS) {
        case 4:
          digitalWrite(LED3, LOW);
          damage();
          break;
        case 3:
          digitalWrite(LED2, LOW);
          damage();
          break;
        case 2:
          digitalWrite(LED1, LOW);
          damage();
          break;
    }
    VIDAS = VIDAS - 1;
    if(VIDAS == 1){
        sonidoDerrota();
    }
}

void sonidoDerrota() {
    int sonidoDerrota[] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5};
    int duracionNota = 300;
    int j = 2;
    while(j > 0){
    for (int i = 0; i < sizeof(sonidoDerrota) / sizeof(sonidoDerrota[0]); i++) {
        tone(BUZZER, sonidoDerrota[i], duracionNota);
        delay(300);
    }
    noTone(BUZZER);
    j--;
    }
}

void damage() {
  int frequency = 1800;
  int duration = 500;
  tone(BUZZER, frequency, duration);
  delay(duration);
}
