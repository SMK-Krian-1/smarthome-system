#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define RightMotorSpeed 5
#define RightMotorDir 0
#define LeftMotorSpeed 4
#define LeftMotorDir 2
#define rainPin A0
#define buzzPin D5

SimpleTimer timer;

char auth[] = "bf029a6633d3457580b7e35989b021a6";
char ssid[] = "oo";
char pass[] = "220822089";
int timerId;
int rain;
int range;
int cuaca = 1;

void customLoop() {
  digitalWrite(buzzPin, HIGH);
  delay(100);
  digitalWrite(buzzPin, LOW);
  delay(100);
}

void readRain() {
  rain = analogRead(rainPin);
  range = map(rain, 0, 1024, 0, 3);
  switch (range) {
    case 0:
      if (cuaca == 0) {
        Serial.println("hujan, tidak akan menyalakan alarm");
        digitalWrite(buzzPin, LOW);
      }
      else {
        Serial.println("hujan");
        digitalWrite(buzzPin, HIGH);
        cuaca = 0;
      }
      break;
    case 1:
      if (cuaca == 0) {
        cuaca = 1;
        Serial.println("tidak hujan");
        digitalWrite(buzzPin, LOW);
        cuaca = 1;
      } else {
        Serial.println("tidak hujan emang!");
      }
      break;
  }
  delay(500);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  pinMode(buzzPin, OUTPUT);
  pinMode(rainPin, INPUT);
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
}

void loop()
{
  readRain();
  Blynk.run();
  timer.run();
}

void halt()
{
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
}

void right()
{
  digitalWrite(RightMotorDir, LOW);
  digitalWrite(LeftMotorDir, HIGH);
  digitalWrite(RightMotorSpeed, HIGH);
  digitalWrite(LeftMotorSpeed, HIGH);
  delay(3500);
}

void left()
{
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir, LOW);
  digitalWrite(RightMotorSpeed, HIGH);
  digitalWrite(LeftMotorSpeed, HIGH);
  delay(4500);
}

BLYNK_WRITE(V2)
{
  if (param[0]) {
    Blynk.virtualWrite(V2, LOW);
    right();
    halt();
  } else halt();
}

BLYNK_WRITE(V1)
{
  if (param[0]) {
    Blynk.virtualWrite(V1, LOW);
    left();
    halt();
  } else halt();
}
