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

const int nextBtn = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(alarm, OUTPUT);
  pinMode(nextBtn, INPUT);
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
  Serial.println(counter);

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
  String temp_output = "T:" + String(tempC, 1) + String((char)223) + "C ";
  String hum_output = "H:" + String(humidity, 0) + "%";
  String pres_output = "BAR:" + String(relativePressure, 1) + " mmHg";
  String alt_output = "ALT:" + String(altitudeSea) + " m";
  String prec_output = "PRECIP:" + String(precipitation) + "%";
  
  lcd.setCursor(0, 0);
  lcd.print(temp_output);
  lcd.print(hum_output);
  lcd.setCursor(0,1);
  lcd.print(prec_output);
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
}

void tempAlarm(float tempC){
  if(tempC >= 36 || tempC < 5){
    digitalWrite(alarm, HIGH);
    delay(300);
    digitalWrite(alarm, LOW);
    delay(300);
  }
}
