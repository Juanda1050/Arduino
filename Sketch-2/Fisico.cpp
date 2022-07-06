#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#define Type DHT11

int dhtPin = 10;
DHT HT(dhtPin, Type);
SFE_BMP180 sensorPressure;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float humidity;
float tempC;
char status;
double pressure, tempPressure, relativePressure, altitudeSea;
double seaLevel = 1014.0;

void setup()
{
  Serial.begin(9600);
  HT.begin();
  sensorPressure.begin();
  lcd.begin(16, 2);
  startupMessage();
}

void loop()
{
  
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  
  status = sensorPressure.startTemperature();
  delay(status);
  sensorPressure.getTemperature(tempPressure);
  status = sensorPressure.startPressure(3);
  delay(status);
  sensorPressure.getPressure(pressure, tempPressure);
  relativePressure = pressure * 0.1;
  altitudeSea = sensorPressure.altitude(pressure, seaLevel);
  
  Serial.println(humidity);
  Serial.println(tempC);
  String temp_output = "TEMP: " + String(tempC) + String((char)223) + "C";
  String hum_output = "HUMEDAD: " + String(humidity) + "%";
  String pres_output = "PRES: " + String(relativePressure) + " KPa";
  String alt_output = "P.ATM: " + String(altitudeSea) + " m";
  
  lcd.setCursor(0, 0);
  lcd.print(temp_output);
  lcd.setCursor(0, 1);
  lcd.print(hum_output);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(pres_output);
  lcd.setCursor(0, 1);
  lcd.print(alt_output);
  delay(3000);
  lcd.clear();
  
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
