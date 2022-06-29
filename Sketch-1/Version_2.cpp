const int startBtn = 2;
const int increaseBtn = 3;
const int LEDs[] = {8, 9, 10, 11, 12, 13};
volatile int delayTime = 3000;
int state = 0;

void setup()
{
  Serial.begin(9600);
  for (int pinLed = 8; pinLed < 13; pinLed++) {
      pinMode(pinLed, OUTPUT);
  }
  pinMode(startBtn, INPUT);
  attachInterrupt(digitalPinToInterrupt(increaseBtn), acelerateLED, RISING);
}

void loop()
{
  delay(500);

  if (isPoweredOn()) {
    bounceSequence();
  }
  else {
      shutdown();
  }
}

bool isPoweredOn() {
  int previousState = state;
  int input = digitalRead(startBtn);
  state = input == HIGH ? !state: state;
  return state;
}

void acelerateLED()
{
  delayTime -= delayTime * 0.10;
}

void shutdown()
{
  for (int i = 0; i < sizeof(LEDs); i++)
    digitalWrite(LEDs[i], LOW);
}

int bounce(int i){
  digitalWrite(i, HIGH);
  delay(delayTime);
  digitalWrite(i, LOW);
}

void bounceSequence()
{
  while(delayTime > 0){
    for(int i = 8; i < 13; i++){
        bounce(i);
        if (!isPoweredOn())
            return;
    }
    delayTime -= delayTime * 0.10;
    Serial.println(delayTime);
    for(int i = 13; i > 8; i--){
        bounce(i);
        if (!isPoweredOn())
            return;
    }
    delayTime -= delayTime * 0.10;
    Serial.println(delayTime);
    if(delayTime <= 10)
    {
      delayTime = 3000;
    }    
  }
}