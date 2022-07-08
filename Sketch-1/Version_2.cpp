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

const int changeBtn = 13;
int state = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(changeBtn, INPUT);
  pinMode(alarm, OUTPUT);
  HT.begin();
  sensorPressure.begin();
  lcd.begin(16, 2);
  startupMessage();
}

void loop()
{
  
  setValues();
  lap++;  
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

void setValues(){
  auxPressure = relativePressure;
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  
  status = sensorPressure.startTemperature();
  delay(status);
  sensorPressure.getTemperature(tempPressure);
  status = sensorPressure.startPressure(3);
  delay(status);
  sensorPressure.getPressure(pressure, tempPressure);
  relativePressure = pressure * 0.750062;
  altitudeSea = sensorPressure.altitude(pressure, seaLevel);
  
  Serial.println("Humedad " + String(humidity));
  Serial.println("Temperatura " + String(tempC));
  Serial.println("Presion " + String(relativePressure));

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
  
  Serial.println("Aux Humedad " + String(auxHumidity));
  Serial.println("Aux Presion " + String(auxPressure));
  Serial.println("Precipitacion " + String(precipitation));
  Serial.println("");
  
  displayValues(humidity, tempC, relativePressure, altitudeSea, precipitation);
}

void displayValues(double humidity, double tempC, double relativePressure, double altitudeSea, double precipitation){ 
  String temp_output = "TEMP: " + String(tempC) + String((char)223) + "C";
  String hum_output = "HUMEDAD: " + String(humidity) + "%";
  String pres_output = "BAR: " + String(relativePressure) + " mmHg";
  String alt_output = "ALT: " + String(altitudeSea) + " m";
  String prec_output = "PRECIP: " + String(precipitation) + "%";
  
  lcd.setCursor(0, 0);
  lcd.print(temp_output);
  lcd.setCursor(0, 1);
  lcd.print(hum_output);
  tempAlarm(tempC);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(pres_output);
  lcd.setCursor(0, 1);
  lcd.print(alt_output);
  tempAlarm(tempC);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(prec_output);
  delay(3000);
  lcd.clear();
}

void setTemp(){
  String tempMin_output = "MIN: 5" + String((char)223) + "C";
  String tempMax_output = "MAX: 36" + String((char)223) + "C";
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP"); //
  lcd.setCursor(0, 1);
  lcd.print("MIN - MAX"); //
  delay(1000);
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
