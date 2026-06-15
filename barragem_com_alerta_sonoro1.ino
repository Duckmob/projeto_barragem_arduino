#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

const int pinPing = 7;
const int pinPiezoAlert = 8;
const int pinLedOn = 5;
const int pinLedAlert = 6;

const int limiarCritico = 140; 
const int limiarSeguro = 145;  

bool isScreaming = false;

void setup() {
  lcd.begin(16, 2);
  
  lcd.setCursor(0,0);
  lcd.print("Alerta Barragem");
  
  pinMode(pinPiezoAlert, OUTPUT);
  pinMode(pinLedOn, OUTPUT);
  pinMode(pinLedAlert, OUTPUT);
  
  digitalWrite(pinLedOn, HIGH); 
}

void loop() {
  long duration, cm;
  
  // Emissão do pulso
  pinMode(pinPing, OUTPUT);
  digitalWrite(pinPing, LOW);
  delayMicroseconds(2);
  digitalWrite(pinPing, HIGH);
  delayMicroseconds(5);
  digitalWrite(pinPing, LOW);
  
  // Recepção
  pinMode(pinPing, INPUT);
  duration = pulseIn(pinPing, HIGH);
  cm = microsecondsToCm(duration);
  
  // Correção da Exibição no LCD
  lcd.setCursor(0, 1);
  lcd.print("dist: ");
  lcd.print(cm); // Imprime a variável separadamente
  lcd.print(" cm    "); // Adiciona espaços para limpar resíduos antigos
  
  // Lógica de Controle
  if (cm <= limiarCritico && !isScreaming) {
    isScreaming = true;
    digitalWrite(pinLedAlert, HIGH);
    tone(pinPiezoAlert, 600);
  } else if (cm >= limiarSeguro && isScreaming) {
    isScreaming = false;
    digitalWrite(pinLedAlert, LOW);
    noTone(pinPiezoAlert);
  }
  
  delay(100); 
}

long microsecondsToCm(long duration) {
  // Nota: Para maior precisão, considere usar 'float' em vez de 'long'
  return duration / 58; 
}