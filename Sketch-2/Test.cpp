#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#define Type DHT11

int dhtPin = 10;
int alarm = 6;
int lap = 1;
DHT HT(dhtPin, Type);
SFE_BMP180 sensorPressure;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
double humidity, tempC;
char status;
double pressure, tempPressure, relativePressure, altitudeSea;
double seaLevel = 1014.0;
double auxPressure, auxHumidity, precipitation;

const int buttonPin = 13;
int state = 0;
int menu = 0;

int WhichScreen =1;   // This variable stores the current Screen number
boolean hasChanged = true;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(alarm, OUTPUT);
  HT.begin();
  sensorPressure.begin();
  lcd.begin(16, 2);
  startupMessage();
}

void loop()
{
  if (hasChanged == true) {
  switch(WhichScreen) {
    case 1:
      tempHum();
      break;
    case 2:
      setTemp();
      break;
  
    }
}
 
    //-------------------------------
    // BEGIN of the switch debouncing code
    int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
 
if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
 
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
 
      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        hasChanged = true;
        WhichScreen++;
       
       
      }
    } else {
      hasChanged = false;
    }
  }
  lastButtonState = reading;
  // END of the switch Debouncing code
  // --------------------------------------
  if (WhichScreen > 2){
    WhichScreen = 1;
  }
}

void startupMessage() {
  lcd.setCursor(0, 0);
  lcd.print("Estacion"); //
  lcd.setCursor(0, 1);
  lcd.print("Meteorologica"); //
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Equipo 1");
  delay(2000);
  lcd.clear();
  delay(100);
}

void tempHum(){ 
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  
  String temp_output = "TEMP: " + String(tempC) + String((char)223) + "C";
  String hum_output = "HUMEDAD: " + String(humidity) + "%";
  int i = 10;
  while (i > 0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(temp_output);
    lcd.setCursor(0, 1);
    lcd.print(hum_output);
    tempAlarm(tempC);
    delay(3000);
    pressureAltitude();
    delay(3000);
    tempHum();
    i++;
    if(i <= 1){
      i = 10;
    }
  }
}

void pressureAltitude(){
  auxPressure = relativePressure;
  status = sensorPressure.startTemperature();
  delay(status);
  sensorPressure.getTemperature(tempPressure);
  status = sensorPressure.startPressure(3);
  delay(status);
  sensorPressure.getPressure(pressure, tempPressure);
  relativePressure = pressure * 0.750062;
  altitudeSea = sensorPressure.altitude(pressure, seaLevel);

  if(lap == 1){
    precipitation = 0;
  }
  else if(auxPressure >= relativePressure){
    precipitation = (auxPressure - relativePressure) / (humidity * 0.1) * 100;
  }
  else if(auxPressure == relativePressure){
    precipitation = precipitation;
  }
  else{
    precipitation = (relativePressure - auxPressure) / (humidity * 0.1) * 100;
  }
  
  String pres_output = "BAR: " + String(relativePressure) + " mmHg";
  String alt_output = "ALT: " + String(altitudeSea) + " m";
  String prec_output = "PRECIP: " + String(precipitation) + "%";
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(pres_output);
  lcd.setCursor(0, 1);
  lcd.print(prec_output);
  tempAlarm(tempC);
  lap++;  
}

void setTemp(){
  String tempMin_output = "MIN: 5" + String((char)223) + "C";
  String tempMax_output = "MAX: 36" + String((char)223) + "C";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(tempMin_output);
  lcd.setCursor(0, 1);
  lcd.print(tempMax_output);
}

void tempAlarm(float tempC){
  if(tempC >= 36 || tempC < 5){
    digitalWrite(alarm, HIGH);
    delay(300);
    digitalWrite(alarm, LOW);
    delay(300);
  }
}
