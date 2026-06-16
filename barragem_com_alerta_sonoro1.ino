#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>


Adafruit_LiquidCrystal lcd(0);

const int pinPing = 7;
const int pinPiezoAlert = 8;
const int pinLedOn = 5;
const int pinLedAlert = 6;
const int pinServo = 3;

const int limiarCritico = 140; 
const int limiarSeguro = 145;  

bool isScreaming = false;

Servo servoValvula;

void setup() {
  lcd.begin(16, 2);
  
  lcd.setCursor(0,0);
  lcd.print("Alerta Barragem");
  
  pinMode(pinPiezoAlert, OUTPUT);
  pinMode(pinLedOn, OUTPUT);
  pinMode(pinLedAlert, OUTPUT);
  
  servoValvula.attach(pinServo);
  
  digitalWrite(pinLedOn, HIGH);
  
  // the valve is closed
  servoValvula.write(0);
}

void loop() {
  
  long duration, cm;
  
  // Pulse emission
  pinMode(pinPing, OUTPUT);
  digitalWrite(pinPing, LOW);
  delayMicroseconds(2);
  digitalWrite(pinPing, HIGH);
  delayMicroseconds(5);
  digitalWrite(pinPing, LOW);
  
  // Receive
  pinMode(pinPing, INPUT);
  duration = pulseIn(pinPing, HIGH);
  cm = microsecondsToCm(duration);
  
  // Show the distance on LCD
  lcd.setCursor(0, 1);
  lcd.print("dist: ");
  lcd.print(cm);
  lcd.print(" cm    ");
  
  if (cm <= limiarCritico && !isScreaming) {
    
    isScreaming = true;
    digitalWrite(pinLedAlert, HIGH);
    tone(pinPiezoAlert, 600);
    servoValvula.write(90);
    
  } else if (cm >= limiarSeguro && isScreaming) {
    
    isScreaming = false;
    digitalWrite(pinLedAlert, LOW);
    noTone(pinPiezoAlert);
    servoValvula.write(0);
  }
  
  delay(100); 
}

long microsecondsToCm(long duration) {
  
  return duration / 58; 
}
