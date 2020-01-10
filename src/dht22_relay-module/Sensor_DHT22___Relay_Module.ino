#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#include <SimpleTimer.h>
#include <DHT.h>
#include <DHT_U.h>

char auth[] = "b1d57499cdce413698b349dcfa4bbb6a";
char ssid[] = "oo";
char pass[] = "220822089";

#define DHTPIN 2
#define DHTTYPE DHT22
#define pirPin D5
#define buzzPin D6
#define lampu0 D0
#define lampu1 D1
#define lampu2 D2
#define lampu3 D3

int pirVal = 0;
int pirState = LOW;
int timerId;
int motion;

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;


void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Serial.print("Temp = ");
  Serial.print(t);
  Serial.print(" Humi = ");
  Serial.println(h);
}

void customLoop() {
  digitalWrite(buzzPin, HIGH);
  delay(100);
  digitalWrite(buzzPin, LOW);
  delay(100);
}

void readPir() {
  if (motion == 0) {
    Serial.println("motion off");
    digitalWrite(buzzPin, LOW);
    delay(500);
  }
  else {
    pirVal = digitalRead(pirPin);   // read sensor value
    //  Blynk.virtualWrite(V7, pirVal);
    if (pirVal == HIGH) {           // check if the sensor is HIGH
      if (pirState == LOW) {
        timerId = timer.setTimer(100, customLoop, 30);
        //      digitalWrite(buzzPin, HIGH);
        Serial.println("Motion detected!");
        pirState = HIGH;       // update variable state to HIGH
      }
    }
    else {
      if (pirState == HIGH) {
        timer.deleteTimer(timerId);
        digitalWrite(buzzPin, LOW);
        Serial.println("Motion stopped!");
        pirState = LOW;       // update variable state to LOW
      }
    }
  }
  //  Blynk.virtualWrite(V7, pirVal);
}

void setup() {
  pinMode(lampu0, OUTPUT);
  pinMode(lampu1, OUTPUT);
  pinMode(lampu2, OUTPUT);
  pinMode(lampu3, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void allLamp(String action) {
  if (action == "on") {
    digitalWrite(lampu0, HIGH);
    digitalWrite(lampu1, HIGH);
    digitalWrite(lampu2, HIGH);
    digitalWrite(lampu3, HIGH);
  }
  if (action == "off") {
    digitalWrite(lampu0, LOW);
    digitalWrite(lampu1, LOW);
    digitalWrite(lampu2, LOW);
    digitalWrite(lampu3, LOW);
  }
}

BLYNK_WRITE(3) {
  if (param.asInt()) {
    Blynk.virtualWrite(D0, param.asInt());
    Blynk.virtualWrite(D1, param.asInt());
    Blynk.virtualWrite(D2, param.asInt());
    Blynk.virtualWrite(D3, param.asInt());
    allLamp("on");
  } else {
    Blynk.virtualWrite(D0, !param.asInt());
    Blynk.virtualWrite(D1, !param.asInt());
    Blynk.virtualWrite(D2, !param.asInt());
    Blynk.virtualWrite(D3, !param.asInt());
    allLamp("off");
  }
}

void demo() {
  digitalWrite(lampu0, HIGH);
  digitalWrite(lampu2, HIGH);
  delay(500);
  digitalWrite(lampu1, LOW);
  digitalWrite(lampu3, LOW);
  delay(500);
  digitalWrite(lampu1, LOW);
  digitalWrite(lampu3, LOW);
  delay(500);
  digitalWrite(lampu0, HIGH);
  digitalWrite(lampu2, HIGH);
  delay(500);
}

BLYNK_WRITE(8) {
  if (param.asInt()) {
    motion = 1;
  } else {
    motion = 0;
    digitalWrite(buzzPin, LOW);
  }
}

void loop() {
  readPir();
  Blynk.run();
  timer.run();
}
