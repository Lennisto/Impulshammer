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
int InputTasterB = 7; // TasterB --> Dauerbetrieb
int InputPotiA = 1; // PotiA --> Pulsdauer
int InputPotiB = 2; // PotiB --> Wiederholfrequenz
int InputPotiC = 3; // PotiC --> Leistung

//Pin Belegungen der Outputs
int Hubmagnet = 3; // Gate des Mosfets an Pin 3 (PWM)

int tic;
bool pressed=false;
bool pressed_dauer=false;
bool dauerbetrieb=false;

void setup() {
  //Setup KraM
  lcd.begin(16,2);
  Serial.begin(9600); 
  pinMode(InputTasterA,INPUT);
  pinMode(InputTasterB,INPUT);
  pinMode(Hubmagnet, OUTPUT);

  tic=millis();
  lcd_print_static();
}



void loop() 
{


  lcd_print_dynamic();
//Taster
  
  ZustandTasterA = digitalRead(InputTasterA);
  ZustandTasterB = digitalRead(InputTasterB);

  if((ZustandTasterA==true)&&(dauerbetrieb==false))
  {
    einzel_betrieb();
  }
  if((ZustandTasterA==false)&&(dauerbetrieb==false))
  {
    pressed=false;
  }

  
  if((ZustandTasterB==true))
  {
    dauerbetrieb=true;
    pressed_dauer=true;
    delay(20);
  }
  if(dauerbetrieb==true)
  {
    fire(Leistung,Wiederholfrequenz,Pulsdauer);
  }

  if((ZustandTasterB==true)&&(pressed_dauer==false))
  {
    dauerbetrieb=false;
  }
   if((ZustandTasterB==false))
  {
    pressed_dauer=false;
  }
  

  
  //Serial.println(ZustandTasterA,DEC);
   
  
  //Serial.println(ZustandTasterB,DEC);


//Potentiometer

  Pulsdauer = analogRead(InputPotiA);
  Pulsdauer = map(Pulsdauer, 0, 1014, 20, 100); // Intervall der Pulsdauer: 20ms - 100ms
  //Serial.println(Pulsdauer);
  //delay(20);

  Wiederholfrequenz = analogRead(InputPotiB);
  Wiederholfrequenz = map(Wiederholfrequenz, 0, 1023, 200, 2000); // Intervall der Wiederholfrequenz: 20ms - 2000ms
  //Serial.println(Leistung);
  //delay(20);

  Leistung = analogRead(InputPotiC);
  Serial.println(Leistung);
  Leistung = map(Leistung, 0, 1010, 150, 255); // Intervall der PWM: 150 - 255
  
  //delay(20);


/*
  analogWrite(Hubmagnet, Leistung);
  delay(Pulsdauer);
  analogWrite(Hubmagnet, Leistung);
  delay(Wiederholfrequenz);*/

}


void lcd_print_static()
{
  lcd.setCursor(0,0);
  lcd.print("Pulsdauer=");
  lcd.setCursor(14,0);
  lcd.print("ms");
  lcd.setCursor(0,1);
  lcd.print("Leistung=");
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

