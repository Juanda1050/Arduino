#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#define Type DHT11

const int  RS = 12, E = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

const int alarm = 6;
const int editBtn = 7;
const int nextBtn = 8;
const int downBtn = 9;
const int upBtn = 10;
const int dhtPin = 13;

DHT HT(dhtPin, Type);
SFE_BMP180 sensorPressure;

double humidity, tempC;
char status;
double pressure, tempPressure, relativePressure, altitudeSea;
int lap = 1;
double seaLevel = 1014.0;
double precipitation, kPaPressure;

int displayValue = 0;
int editValue = 0;
int maxTemp = 40, minTemp = 5;
bool edit = false;
void displayTemp(void);
void displayPres(void);
void displayPrecip(void);
void setTemp(void);
void showError(void);

void setup()
{
  Serial.begin(9600);
  pinMode(alarm, OUTPUT);
  pinMode(editBtn, INPUT);
  pinMode(nextBtn, INPUT);
  pinMode(downBtn, INPUT);
  pinMode(upBtn, INPUT);
  HT.begin();
  sensorPressure.begin();
  lcd.begin(16, 2);
  startupMessage();
}

void loop()
{
  if(digitalRead(editBtn) == HIGH){
    edit = !edit;
    editValue = 0;
    displayValue = 0;
    delay(1000);
  }

  if(edit == false){
    lcd.noCursor();
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
      break;
    }
    delay(1000);
  }
  else {
    delay(1000);
    lcd.clear();
    if(digitalRead(nextBtn) == HIGH){
      editValue++;
    }
    switch(editValue){
      case 0: 
        lcd.noCursor();
        setTemp();
        if(digitalRead(upBtn) == HIGH){
          minTemp++;
          if(minTemp >= maxTemp) minTemp = 0;
        }
        if(digitalRead(downBtn) == HIGH) {
          minTemp--;
          if(minTemp <= 0) minTemp = maxTemp - 1;
        }
        lcd.setCursor(0, 1);
        lcd.cursor();
        break;
      case 1:
        lcd.noCursor();
        setTemp();
        if(digitalRead(upBtn) == HIGH){
          maxTemp++;
          if(maxTemp >= 50) maxTemp = minTemp + 1;
        }
        if(digitalRead(downBtn) == HIGH) {
          maxTemp--;
          if(maxTemp <= minTemp) maxTemp = 50;
        }
        lcd.setCursor(7, 1);
        lcd.cursor();
        break;
      default:
        editValue = 0;
        lcd.noCursor();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SET ALARM");
        lcd.setCursor(0, 1);
        lcd.print("TEMP MIN - MAX");
        break;
    }
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

void getDHT11(void){
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
}

void getBMP180(void){
  getDHT11();
  status = sensorPressure.startTemperature();
  delay(status);
  sensorPressure.getTemperature(tempPressure);
  status = sensorPressure.startPressure(3);
  delay(status);
  sensorPressure.getPressure(pressure, tempPressure);
  relativePressure = pressure * 0.750062;
  kPaPressure = pressure * 0.1;
  altitudeSea = sensorPressure.altitude(pressure, seaLevel);

  if(lap == 1){
    precipitation = 0;
  }
  else if(humidity == 0 || humidity <= tempC){
    precipitation = 0;
  }
  else{
    precipitation = ((humidity - tempC) / kPaPressure) * 100;
  }

  lap++;
}

void displayTemp(void){ 
  getDHT11();
  
  String temp_output = "TEMP: " + String(tempC, 1) + String((char)223) + "C ";
  String hum_output = "HUMEDAD: " + String(humidity, 0) + "%";
  
  lcd.setCursor(0, 0);
  lcd.print(temp_output);
  lcd.setCursor(0,1);
  lcd.print(hum_output);
  tempAlarm(tempC);
}

void displayPres(void){
  getDHT11();
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
  getDHT11();
  String prec_output = "PRECIP: " + String(precipitation) + "%";
  String kpa_output = "BAR: " + String(kPaPressure, 1) + " kPa";
 
  lcd.setCursor(0,0);
  lcd.print(kpa_output);
  lcd.setCursor(0, 1);
  lcd.print(prec_output);
  tempAlarm(tempC);
}

void setTemp(){
  getDHT11();
  String alarm_output = "TEMP ALARM SET";
  String tempSet_output = String(minTemp) + String((char)223) + "C" + " - " + String(maxTemp) + String((char)223) + "C";
  
  lcd.setCursor(0, 0);
  lcd.print(alarm_output);
  lcd.setCursor(0, 1);
  lcd.print(tempSet_output);
  tempAlarm(tempC);
}

void showError(){
  lcd.setCursor(0, 0);
  lcd.print("Error display");
}

void tempAlarm(float tempC){
  if(tempC >= (double)maxTemp || tempC <= (double)minTemp){
    digitalWrite(alarm, HIGH);
    delay(300);
    digitalWrite(alarm, LOW);
    delay(300);
  }
  
}
