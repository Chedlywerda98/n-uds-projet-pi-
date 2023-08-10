#include<Wire.h>

const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

const int buzzerPin = 32;
const int flamePin = 35;
const int gazPin = 34 ;
const int relayPompe = 33;

#define address_PCF 0x38 //PCF EXPANDER ADDRESS

void setup() {
  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);  
  pinMode(buzzerPin, OUTPUT);
  pinMode(flamePin, INPUT);
  Wire.begin();
 Wire.beginTransmission(address_PCF); // Commence la transmission sur l'adresse défini plus haut 0x20 champ adresse 
 Wire.write(0x00);              // On envoie la donnée p0
 Wire.endTransmission();
}
void loop() {
  // read the sensor on analog A0:
  int sensorReading = analogRead(flamePin);
  // map the sensor range (four options):
  // ex: 'long int map(long int, long int, long int, long int, long int)'
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  
  // range value:
  switch (range) {
 
  case 0:    // A fire closer than 1.5 feet away.
    Serial.println("** Close Fire **");
    digitalWrite(buzzerPin, HIGH);
    Wire.beginTransmission(address_PCF); // Commence la transmission sur l'adresse défini plus haut 0x20 champ adresse 
    Wire.write(0x01);              // On envoie la donnée p0 allumage du led Red
    Wire.endTransmission();
    delay(500);
    Wire.beginTransmission(address_PCF); // Commence la transmission sur l'adresse défini plus haut 0x20 champ adresse 
    Wire.write(0x00);              // Leds etteints
    Wire.endTransmission();
    delay(500);   
    lcd.clear(); lcd.setCursor(0,0); lcd.print("Close Fire Detected"); lcd.setCursor(0,1); lcd.print(range); lcd.print("m");
    break;
  
  case 1:    // A fire between 1-3 feet away.
    Serial.println("** Distant Fire **");
    digitalWrite(buzzerPin, HIGH);
    Wire.beginTransmission(address_PCF); // Commence la transmission sur l'adresse défini plus haut 0x20 champ adresse 
    Wire.write(0x02);              // On envoie la donnée p1 allumage du led Orangé
    Wire.endTransmission();
    delay(500);
    Wire.beginTransmission(address_PCF); // Commence la transmission sur l'adresse défini plus haut 0x20 champ adresse 
    Wire.write(0x00);              // Leds eteints
    Wire.endTransmission();
    delay(500);   
    lcd.clear(); lcd.setCursor(0,0); lcd.print("Distant Fire Detected"); lcd.setCursor(0,1); lcd.print(range); lcd.print("m");
    break;
  
  case 2:    // No fire detected.
    Serial.println("No Fire");
    digitalWrite(buzzerPin, LOW);
    Wire.beginTransmission(address_PCF); // Commence la transmission sur l'adresse défini plus haut 0x20 champ adresse 
    Wire.write(0x04);              // On envoie la donnée p1 allumage du led Orangé
    Wire.endTransmission();   
    lcd.clear(); lcd.setCursor(0,0); lcd.print("No Fire Detected");
    break;
  }
  delay(1);  // delay between reads
}
