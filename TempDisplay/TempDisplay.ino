/*
 * The following sketch will provide a temp monitoring circuit as well as a auto dimmer for the LCD screen.
 * Kevin Farley 5/22/2019
*/

// include the library code:
#include <LiquidCrystal.h>
#include <math.h>
#include <SPI.h>
#include <Ethernet.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int LightSensorPin = 0;
int Thermistor1Pin = 1;
int Thermistor2Pin  = 2;
int BacklightOut = 2;
int TestIPPin = 7;

int Vo, Vo2, Lo;
float T1, C1, C2, T2, tick;
bool Test;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(BacklightOut,OUTPUT);
  pinMode(TestIPPin, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  
  Vo = analogRead(Thermistor1Pin);
  Vo2 = analogRead(Thermistor2Pin);
  Lo = analogRead(LightSensorPin);
  Test = digitalRead(TestIPPin);

  C1 = Thermister(Vo);
  T1 = CtoF(C1);

  C2 = Thermister(Vo2);
  T2 = CtoF(C2);

  // Turn On backlight
  if(Lo<900){
    analogWrite(BacklightOut,255);
  } else {
    analogWrite(BacklightOut,0);
  }

  // set the cursor to column 0, line 0
  // (note: line 0 is the first row):
  lcd.setCursor(0, 0);
  if(Test){
    lcd.print("Temp 1 = ");
    lcd.print(T1,1);   
    lcd.print(" F");
  
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    lcd.print("Temp 2 = ");
    lcd.print(T2,1);   
    lcd.print(" F");
  } else {
    lcd.clear();
    lcd.print("Testing");
  }
  tick = 4.7/1023;

  Serial.print(Test);
  Serial.print("Raw 1: ");
  Serial.print(Vo);
  Serial.print(" - ");
  Serial.print((tick*Vo));
  Serial.print(" Vdc Temp1: ");
  Serial.print(T1);

  Serial.print(" --- Raw 2: ");
  Serial.print(Vo2);
  Serial.print(" - ");
  Serial.print((tick*Vo2));
  Serial.print(" Vdc Temp2: ");
  Serial.print(T2);
  Serial.print(" --- Light: ");
  Serial.println(Lo);
  delay(1000);
}

double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 return Temp;
}

double CtoF(double CTemp) {
  double Temp;
  Temp = (CTemp * 9.0)/ 5.0 + 32.0;
  return Temp;
}
