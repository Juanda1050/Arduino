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
int displayValue = 0;

void displayTemp(void);
void displayPres(void);
void displayPrecip(void);
void setTemp(void);
void showError(void);

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
  if(digitalRead(nextBtn) == HIGH){
    displayValue++;
  }
  switch(displayValue){
    case 0:
      lcd.clear();
      displayTemp();
    break;
    case 1:
      lcd.clear();
      displayPres();
    break;
    case 2:
      lcd.clear();
      displayPrecip();
    break;
    case 3:
      lcd.clear();
      setTemp();
    break;
    default:
      lcd.clear();
      displayValue = 0;
      displayTemp();
  }
  delay(1000);
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

void getDHT11(void){
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
}

void getBMP180(void){
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

  lap++;
}

void displayTemp(void){ 
  getDHT11();
  
  String temp_output = "TEMP: " + String(tempC, 1) + String((char)223) + "C ";
  String hum_output = "HUM: " + String(humidity, 0) + "%";
  
  lcd.setCursor(0, 0);
  lcd.print(temp_output);
  lcd.setCursor(0,1);
  lcd.print(hum_output);
  tempAlarm(tempC);
}

void displayPres(void){
  getBMP180();
  
  String pres_output = "BAR: " + String(relativePressure, 1) + " mmHg";
  String alt_output = "ALT: " + String(altitudeSea) + " m";
  
  lcd.setCursor(0,0);
  lcd.print(pres_output);
  lcd.setCursor(0, 1);
  lcd.print(alt_output);
  tempAlarm(tempC);
}

void displayPrecip(void){
  getBMP180();  
  String prec_output = "PRECIP: " + String(precipitation) + "%";
  
  lcd.setCursor(0,1);
  lcd.print(prec_output);
}

void setTemp(){
  String tempMin_output = "MIN: 5" + String((char)223) + "C";
  String tempMax_output = "MAX: 36" + String((char)223) + "C";
  lcd.setCursor(0, 0);
  lcd.print(tempMin_output);
  lcd.setCursor(0, 1);
  lcd.print(tempMax_output);
}

void showError(){
  lcd.setCursor(0, 0);
  lcd.print("Error display");
}

void tempAlarm(float tempC){
  if(tempC >= 36 || tempC < 5){
    digitalWrite(alarm, HIGH);
    delay(300);
    digitalWrite(alarm, LOW);
    delay(300);
  }
}
