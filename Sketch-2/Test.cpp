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

int WhichScreen = 1 ; 
boolean hasChanged = true;
int buttonState;            
int lastButtonState = LOW;  
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;
    
const int buttonPin = 13;
const int upBtn = 8;
const int downBtn = 7;
int state = 0;
int upCounter = 5; 
int downCounter = 36;   
int up_buttonState = 0;         
int up_lastButtonState = 0;    
int down_buttonState = 0;        
int down_lastButtonState = 0;    

bool bPress = false;

unsigned long prevMillis = 0;
unsigned long curMillis;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode( upBtn , INPUT_PULLUP);
  pinMode( downBtn , INPUT_PULLUP);
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
      displayValues();
      break;
    case 2:
      setTemp();
      break;
    }
  }
 
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
 
  if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == HIGH) {
          hasChanged = true;
          WhichScreen++;
        }
      } else {
        hasChanged = false;
      }
    }
    lastButtonState = reading;

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

void displayValues(){
  static unsigned long diff;

  curMillis = millis();
  diff = curMillis - prevMillis;
  switch (diff) {
    case 1000:
      tempHum();
      break;
    case 4000:
      pressureAltitude();
      break;
    case 10000:
      lcd.clear();
      prevMillis = millis();
      break;
  }
}

void tempHum(){ 
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  
  String temp_output = "TEMP: " + String(tempC) + String((char)223) + "C";
  String hum_output = "HUMEDAD: " + String(humidity) + "%";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(temp_output);
  lcd.setCursor(0, 1);
  lcd.print(hum_output);
  tempAlarm(tempC);
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
  checkUp();
  checkDown();
  lcd.clear();
  String tempMin_output = "MIN: " + String(downCounter) + String((char)223) + "C";
  String tempMax_output = "MAX: " + String(upCounter) + String((char)223) + "C";
  if( bPress){
    bPress = false;
    lcd.setCursor(0, 0);
    lcd.print(tempMin_output);
    lcd.setCursor(0, 1);
    lcd.print(tempMax_output);
  }

}

void showError(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error display");
}

void tempAlarm(float tempC){
  if(tempC >= upCounter || tempC < downCounter){
    digitalWrite(alarm, HIGH);
    delay(300);
    digitalWrite(alarm, LOW);
    delay(300);
  }
}

void checkUp(){
  up_buttonState = digitalRead(upBtn);
  if (up_buttonState != up_lastButtonState) {
    if (up_buttonState == LOW) {
      bPress = true;
      upCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(upCounter);
      if(upCounter >= 50){
        upCounter = 21;
      }

    } else {
      Serial.println("off");
    }
    delay(50);

  }
  up_lastButtonState = up_buttonState;

}

void checkDown(){
  down_buttonState = digitalRead(downBtn);
  if (down_buttonState != down_lastButtonState) {
    if (down_buttonState == LOW) {
      bPress = true;
      downCounter--;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(downCounter);
      if(downCounter <= -20){
        downCounter = 20;
      }

    } else {
      Serial.println("off");
    }
    delay(50);

  }
  down_lastButtonState = down_buttonState;
}
