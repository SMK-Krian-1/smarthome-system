int data;
int inputPin = 2;                                         // Memilih input PIR Sensor
int pirState = LOW;                                 // Pada saat mulai, Matikan sensor
int val = 0;                                               // Variable pembaca status pin
int pinSpeaker = 10;                               // Memilih pin buzzer, dengan pin PWM
//Inialisasi pin sensor hujan
const int pinHujan = A5;
//Inialisasi pin buzzer
const int pinBuzzer = 8;
//Deklarasi variable data pembacaan sensor hujan



void setup() {
  pinMode(inputPin, INPUT);               // jadikan Sensor sebagai Input
  pinMode(pinSpeaker, OUTPUT);      // jadikan buzzer sebagai output
  Serial.begin(9600);
  //Inialisasi status I/O
 pinMode(pinHujan, INPUT);
 pinMode(pinBuzzer, OUTPUT);
}
void loop(){
  
  val = digitalRead(inputPin);
  if (val == HIGH) {
    playTone(300, 160);
    delay(150);
 
    if (pirState == LOW) {
      Serial.println("Terdeteksi suatu pergerakan!");
      pirState = HIGH;
    }
  } else {
      playTone(0, 0);
      delay(300);  
      if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
  //Variabel data adalah hasil pembacaan pin sensor hujan
 data = analogRead(pinHujan);

//Range output sensor berkisar 0 sampai 1023
 //Hujan terdeteksi
 //Saat output sensor berniali <= 500
 if (data <= 500)
 {
 //Alarm dibunyikan
 digitalWrite(pinBuzzer, HIGH);
 delay(500);
 digitalWrite(pinBuzzer, LOW);
 delay(500);}
 
}
void playTone(long duration, int freq) {
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(pinSpeaker,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(pinSpeaker, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}
