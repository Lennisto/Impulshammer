// IRLIZ44N MOS FET PWM Motoransteuerung
// Schaltung IRLIU44N und Pinbelegung
// 1 - Gate > 180 Ohm > Arduino Pin 9
// 2 - Drain > Motor > +
// 3 - Source > GND Arduino und Netzteil
// Widerstand mit 10k Ohm von Gate zu Source

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Variablen

int Pulsdauer = 50;
int Wiederholfrequenz = 2000;
int Leistung = 255;

//Zustände der Inputs
int ZustandTasterA;
int ZustandTasterB;

//Pin Belegungen der Inputs
int InputTasterA = 2; // TasterA --> Einzelbetrieb 
int InputTasterB = 3; // TasterB --> Dauerbetrieb
int InputPotiA = 1; // PotiA --> Pulsdauer
int InputPotiB = 2; // PotiB --> Wiederholfrequenz
int InputPotiC = 3; // PotiC --> Leistung

//Pin Belegungen der Outputs
int Hubmagnet = 9; // Gate des Mosfets an Pin 3 (PWM)

int tic;
bool pressed=false;

volatile byte dauerbetrieb=LOW;

void setup() {
  //Setup KraM
  lcd.begin(16,2);
  Serial.begin(9600); 
  pinMode(InputTasterA,INPUT);
  pinMode(InputTasterB,INPUT);
  pinMode(Hubmagnet, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(InputTasterB), toggle, RISING);

  tic=millis();
  lcd_print_static();
}

void loop() 
{
  lcd_print_dynamic();
//Taster
  
  ZustandTasterA = digitalRead(InputTasterA);

  if((ZustandTasterA==true)&&(dauerbetrieb==false))
  {
    einzel_betrieb();
  }
  if((ZustandTasterA==false)&&(dauerbetrieb==false))
  {
    pressed=false;
  }

  if(dauerbetrieb==true)
  {
    fire(Leistung,Wiederholfrequenz,Pulsdauer);
  }

  
  //Serial.println(ZustandTasterA,DEC);
   
  
  //Serial.println(ZustandTasterB,DEC);


//Potentiometer

  Pulsdauer = analogRead(InputPotiA);
  //Serial.println(Pulsdauer);
  Pulsdauer = map(Pulsdauer, 0, 1010, 20, 100); // Intervall der Pulsdauer: 20ms - 100ms
  //delay(20);

  Wiederholfrequenz = analogRead(InputPotiB);
  //Serial.println(Wiederholfrequenz);
  Wiederholfrequenz = map(Wiederholfrequenz, 0, 1010, 2000, 200); // Intervall der Wiederholfrequenz: 20ms - 2000ms
  //delay(20);

  Leistung = analogRead(InputPotiC);
  //Serial.println(Leistung);
  Leistung = map(Leistung, 0, 1010, 150, 255); // Intervall der PWM: 150 - 255
  //delay(20);

}


void toggle()
{
  dauerbetrieb=!dauerbetrieb;
  delayMicroseconds(20);
}


void lcd_print_static()
{
  lcd.setCursor(0,0);
  lcd.print("Pulsdauer=");
  lcd.setCursor(14,0);
  lcd.print("ms");
  lcd.setCursor(0,1);
  lcd.print("Leistung =");
  lcd.setCursor(14,1);
  lcd.print("%");
}
void lcd_print_dynamic()
{
  int toc=millis();
  if((toc-tic)>100)
  {
    lcd.setCursor(10,0);
    lcd.print("    ");
    lcd.setCursor(10,1);
    lcd.print("    ");
    tic=millis();
  }
  lcd.setCursor(10,0);
  lcd.print(Pulsdauer);
  double leistung=0.393*Leistung; 
  lcd.setCursor(10,1);
  lcd.print((int)leistung);
}



void einzel_betrieb()
{
  if(pressed==false)
  {
    fire(Leistung,Pulsdauer,Pulsdauer);
    pressed=true;
  }
  
}


void fire(int leistung,int frequenz,int pulsdauer)
{
   int wait=frequenz-pulsdauer;
   analogWrite(Hubmagnet, leistung);
   delay(pulsdauer);
   analogWrite(Hubmagnet, 0);
   delay(wait);
}

