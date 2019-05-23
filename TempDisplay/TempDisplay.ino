/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <math.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int LightSensorPin = 0;
int ThermistorPin = 1;
int BacklightOut = 10;

int Vo, Lo;
float R1 = 9500;
float logR2, R2, T, C;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Farley's House!");
  pinMode(BacklightOut,OUTPUT);
  Serial.begin(57600);
}

void loop() {
  
  Vo = analogRead(ThermistorPin);
  Lo = analogRead(LightSensorPin);
  R2 = R1 * ((1023.0 / (float)Vo) - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  C = T - 273.15;
  T = (C * 9.0)/ 5.0 + 32.0; 

  // Turn On backlight
  if(Lo<900){
    analogWrite(BacklightOut,255);
  } else {
    analogWrite(BacklightOut,0);
  }

  // set the cursor to column 0, line 0
  // (note: line 0 is the first row):
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(C,1);   
  lcd.print(" C");

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print("Temp = ");
  lcd.print(T,1);   
  lcd.print(" F");

  Serial.print("Raw: ");
  Serial.print(Vo);
  Serial.print(" Temp: ");
  Serial.println(T);
  delay(1000);

}
