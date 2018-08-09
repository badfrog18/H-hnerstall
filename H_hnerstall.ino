#include <dht11.h>
#include "RTClib.h"
#include <Wire.h> //Wire.h Bibliothek einbinden

#include <LiquidCrystal_I2C.h> //LiquidCrystal_I2C.h Bibliothel einbinden

LiquidCrystal_I2C lcd1(0x27,2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //Hier wird

//Display 1 als lcd1 benannt, die Adresse angegeben „0x3F“ und die

//Pinnbelegung durch das I²C Modul angegeben

//Temperatursensor
dht11 DHT11;
#define DHT11PIN 8
//RTC-Modul
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Lux-Sensor
int Lux=A0;
int Luxwert=0;

int sensoroben = 9;
int sensorunten = 10;
int motorhinauf = 11;
int motorhinunter = 12;
int led = 13;
int sensorobenstat;
int sensoruntstat;
int torstatus =1;
int lichtmerker;
int torhand=7;
int handauf=6;
int handzu=5;
int ledstatus=1;
int torhandstat;
int handaufstat;
int handzustat;


void setup()

{
  lcd1.begin (16,2);
//wird angegeben(16 Zeichen, zwei Zeilen)

lcd1.backlight(); //Beleuchtung Display 1 einschalten



pinMode (sensoroben, INPUT);
pinMode (sensorunten, INPUT);
pinMode (motorhinauf, OUTPUT);
pinMode (motorhinunter, OUTPUT);
pinMode (handauf, INPUT);
pinMode (handzu, INPUT);
pinMode (led, OUTPUT);
pinMode (torhand, INPUT);
digitalWrite (sensoroben, HIGH);
digitalWrite (sensorunten, HIGH);
digitalWrite (motorhinauf, HIGH);
digitalWrite (motorhinunter, HIGH);
digitalWrite (led, HIGH);
digitalWrite (torhand, HIGH);
digitalWrite (handauf, HIGH);
digitalWrite (handzu, HIGH);

//RTC-Modul Uhrzeit Setzen
#ifndef ESP8266

#endif
delay(3000); // wait for console opening

  if (! rtc.begin()) {
    lcd1.setCursor(8,0);
    
    lcd1.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd1.setCursor(8,0);
    
    lcd1.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2018, 7, 30, 19, 37, 0));
  }
 

}

void loop()


{
sensorobenstat = digitalRead (sensoroben);
sensoruntstat = digitalRead (sensorunten);
torhandstat = digitalRead (torhand);
handaufstat = digitalRead (handauf);
handzustat = digitalRead (handzu);

  //RTC
  DateTime now = rtc.now();
  
  
  //Lux- Einlesen
Luxwert =analogRead (Lux);

//Temperatur Einlesen

int chk = DHT11.read(DHT11PIN);
  
//Tor Programm

if ((sensorobenstat==LOW)&& (torhandstat==HIGH))
{torstatus=2;}

if ((sensoruntstat==LOW) && (torhandstat==HIGH))
{torstatus=3;}

if ((now.hour() >=7) && (now.hour() <20) && (Luxwert >=300) && (sensorobenstat==HIGH) && (torhandstat==HIGH))
{torstatus=4;}

if ( (Luxwert <=200) && (sensoruntstat==HIGH) && (torhandstat==HIGH))
{torstatus=5;}

//Handprogramm

if ((handaufstat == LOW) && (torhandstat == LOW) )
{torstatus=4;}

if ((handzustat == LOW)&& (torhandstat == LOW))
{torstatus =5;}

if ((handaufstat == HIGH) && (handzustat == HIGH)&&(torhandstat==LOW))
{torstatus=1;}

switch (torstatus)

{

case 1:
digitalWrite (motorhinauf, HIGH);
digitalWrite (motorhinunter, HIGH);
lcd1.setCursor (8,1);
lcd1.print ("OFF     ");
break;

  case 2:
digitalWrite (motorhinauf, HIGH);
lcd1.setCursor(8,1);
  lcd1.print("offen    ");
break;

case 3:
digitalWrite (motorhinunter, HIGH);
lcd1.setCursor(8,1);
  lcd1.print("zu       ");
break;

case 4:
digitalWrite (motorhinauf, LOW);
digitalWrite (motorhinunter, HIGH);

  lcd1.setCursor(8,1);
  lcd1.print("oeffnet   ");
break;

case 5:
digitalWrite (motorhinauf, HIGH);
digitalWrite (motorhinunter, LOW);

  lcd1.setCursor(8,1);
  lcd1.print("schliest");
break;


}







  
// Beleuchtung

if ((now.hour()>=5) && (now.hour()<=9) && (Luxwert < 500))
{  ledstatus=1;}

if ((now.hour()>=15) && (now.hour()<=21) && (Luxwert <500))
  {   ledstatus=1; }
 
if (Luxwert >600)

{ledstatus =2;}

if (now.hour() >=21)
{ledstatus =2;}


switch (ledstatus)

{
  case 1:
  digitalWrite (led, LOW);
  break;
  
  case 2:
  digitalWrite (led, HIGH);
break;
}




//LCD Ausgabe  
  
lcd1.setCursor(0,0); //Text soll beim ersten Zeichen in der ersten Reihe

lcd1.print((float)DHT11.temperature, 0); //Display 1 soll oben „Display 1“ anzeigen
lcd1.print ("C");

lcd1.setCursor(4,0); //Text soll beim ersten Zeichen in der zweiten Reihe


lcd1.print((float)DHT11.humidity, 0); //Display 1 soll unten „Funduino GmbH“
lcd1.print("%");

lcd1.setCursor(8,0);
  lcd1.print(now.hour(), DEC);
    lcd1.print(':');
    lcd1.print(now.minute(), DEC);
    lcd1.print(':');
    lcd1.print(now.second(), DEC);
    lcd1.println();
lcd1.setCursor(0,1);
lcd1.print(Luxwert);
lcd1.setCursor(4,1);
lcd1.print("Lux");

delay (500);
}

