const int startBtn = 2;
const int increaseBtn = 3;
int delayTime = 1000;
int state = 0;
bool turnOff = false;
bool access = false;
bool increaseSpeed = false;

const int LEDs[] = {8, 9, 10, 11, 12, 13};

void setup()
{
  Serial.begin(9600);
  for (int pinLed = 8; pinLed < 13; pinLed++) {
      pinMode(pinLed, OUTPUT);
  }
  pinMode(startBtn, INPUT);
  pinMode(increaseBtn, INPUT);
}

bool isPoweredOn() {
  int previousState = state;
  int input = digitalRead(startBtn);
  state = input == HIGH ? !state: state;
  Serial.println(state);
  return state;
}

void shutdown()
{
  for (int i = 0; i < sizeof(LEDs); i++)
    digitalWrite(LEDs[i], LOW);
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

int bounce(int i){
  digitalWrite(i, HIGH);
  delay(delayTime);
  digitalWrite(i, LOW);
}

void bounceSequence()
{
  while(delayTime > 10){
    for(int i = 8; i < 13; i++){
        bounce(i);
        if (!isPoweredOn())
            return;
    }
    delayTime -= delayTime * 0.10;
    for(int i = 13; i > 8; i--){
        bounce(i);
        if (!isPoweredOn())
            return;
    }
    delayTime -= delayTime * 0.10;
  }
}