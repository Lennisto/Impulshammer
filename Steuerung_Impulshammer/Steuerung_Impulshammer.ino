// IRLIZ44N MOS FET PWM Motoransteuerung
// Schaltung IRLIU44N und Pinbelegung
// 1 - Gate > 180 Ohm > Arduino Pin 9
// 2 - Drain > Motor > +
// 3 - Source > GND Arduino und Netzteil
// Widerstand mit 10k Ohm von Gate zu Source

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Variablen

int test1=0;
int Pulsdauer = 50;
int Wiederholfrequenz = 2000;
int Leistung = 255;

//Zustände der Inputs
int ZustandTasterA;
int ZustandTasterB;

//Pin Belegungen der Inputs
int InputTasterA = 2; // TasterA --> Einzelbetrieb 
int InputTasterB = 7; // TasterB --> Dauerbetrieb
int InputPotiA = 1; // PotiA --> Pulsdauer
int InputPotiB = 2; // PotiB --> Wiederholfrequenz
int InputPotiC = 3; // PotiC --> Leistung

//Pin Belegungen der Outputs
int Hubmagnet = 3; // Gate des Mosfets an Pin 3 (PWM)


void setup() {

  lcd.begin(16,2);
  
  lcd.setCursor(0,0);
  lcd.print("Pulsdauer=");
  lcd.setCursor(11,0);
  lcd.print(Pulsdauer);
  lcd.setCursor(14,0);
  lcd.print("ms");

  int leistung = Leistung*0.3922;
  lcd.setCursor(0,1);
  lcd.print("Leistung=");
  lcd.setCursor(10,1);
  lcd.print(leistung);
  lcd.setCursor(14,1);
  lcd.print("%");

  
  Serial.begin(9600); 
  pinMode(InputTasterA,INPUT);
  pinMode(InputTasterB,INPUT);
  pinMode(Hubmagnet, OUTPUT);
  
}



void loop() 
{



//Taster
  
  ZustandTasterA = digitalRead(InputTasterA);
  //Serial.println(ZustandTasterA,DEC);
   
  ZustandTasterB = digitalRead(InputTasterB);
  //Serial.println(ZustandTasterB,DEC);


//Potentiometer

  Pulsdauer = analogRead(InputPotiA);
  Pulsdauer = map(Pulsdauer, 0, 1023, 20, 100); // Intervall der Pulsdauer: 20ms - 100ms
  //Serial.println(Pulsdauer);
  delay(20);

  Wiederholfrequenz = analogRead(InputPotiA);
  Wiederholfrequenz = map(Wiederholfrequenz, 0, 1023, 200, 2000); // Intervall der Wiederholfrequenz: 20ms - 2000ms
  //Serial.println(Leistung);
  delay(20);

  Leistung = analogRead(InputPotiA);
  Leistung = map(Leistung, 0, 1023, 150, 255); // Intervall der PWM: 150 - 255
  //Serial.println(Leistung);
  delay(20);



  analogWrite(Hubmagnet, Leistung);
  delay(Pulsdauer);
  analogWrite(Hubmagnet, Leistung);
  delay(Wiederholfrequenz);

}



