#include <LiquidCrystal.h>
const int temperature = A0;
const int humidity  = A1;
const int alarm = 6;
const int led = 10;
float temp = 0;
int rawValue = 0;
double voltage = 0;
int hum = 1;
int humidity_set = 0;
int humidity_invert = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup(){
  Serial.begin(9600);
  pinMode(temperature, INPUT);
  pinMode(humidity, INPUT);
  pinMode(alarm, OUTPUT);
  pinMode(led, OUTPUT);
  lcd.begin(16, 2);
  startupMessage();
}

void loop(){
  setValues();
  
  // This is for update LCD and don't bug the °C and %
  delay(500);
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

void setValues() {
  digitalWrite(led,LOW);
  rawValue = analogRead(temperature);
  voltage = (rawValue / 1023.0) * 5000;
  temp = (voltage-500) * 0.1;
  hum = analogRead(humidity);
  humidity_set = map(hum, 0, 1023, 0 , 100);
  humidity_invert = (100 - humidity_set);
  
  String temp_output = "TEMP: " + String(temp) + String((char)178) + "C";
  String hum_output = "HUMEDAD: " + String(humidity_invert) + "%";
  
  lcd.setCursor(0, 0);
  lcd.print(temp_output);
  lcd.setCursor(0, 1);
  lcd.print(hum_output);
  
  if(temp > 35 || temp < 5)
  {
    digitalWrite(led,HIGH);
    tone(6,261.63,100);// DO durante 0.1 s
  
   
  
  }
}
