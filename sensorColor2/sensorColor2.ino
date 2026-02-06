#define S0 13
#define S1 12
#define S2 14
#define S3 27
#define sensorOut 26

volatile unsigned long pulse = 0;

unsigned long r = 0, g = 0, b = 0;

enum ColorState {
  SET_RED,
  READ_RED,
  SET_GREEN,
  READ_GREEN,
  SET_BLUE,
  READ_BLUE
};

ColorState state = SET_RED;

uint32_t stateTime = 0;
const uint32_t MeasureTime = 200;
void ISR_count(){
  pulse++;
}

void setup() {
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 pinMode(sensorOut, INPUT);
 
// // Setting frequency-scaling to 20%
 digitalWrite(S0,HIGH);
 digitalWrite(S1,LOW);
 
 Serial.begin(9600);
 Serial.println("oooo");
 attachInterrupt(digitalPinToInterrupt(sensorOut), ISR_count, RISING);
}

uint32_t ptime = 0;
void readColor(){
  switch(state){
    case(SET_RED):
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    pulse = 0;
    stateTime = millis();
    state = READ_RED;
    break;

    case(READ_RED):
    if(millis() - stateTime > MeasureTime){
      r = pulse;
      state = SET_GREEN;
    }
    break;

    case(SET_GREEN):
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    pulse = 0;
    stateTime = millis();
    state = READ_GREEN;
    break;

    case(READ_GREEN):
    if(millis() - stateTime > MeasureTime){
      g = pulse+5;
      state = SET_BLUE;
    }
    break;

    case(SET_BLUE):
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    pulse = 0;
    stateTime = millis();
    state = READ_BLUE;
    break;

    case(READ_BLUE):
    if(millis() - stateTime > MeasureTime){
      b = pulse;
      state = SET_RED;
    }
    
  }
}

void loop() {
 ptime = 0;
 readColor();
 if(millis()-ptime > 20){
 Serial.print("r");
 Serial.print(r);
 Serial.print(" : ");
 Serial.print("g");
 Serial.print(g);
 Serial.print(" : ");
 Serial.print("b");
 Serial.print(b);
 Serial.println(" : ");
 if (r > g && r > b){
  Serial.println("red");
 }
 else if (g > r && g> b){
  Serial.println("green");
 }
 else if(b > g && b>r){
  Serial.println("blue");
 }
 ptime = millis();
 }
}
//
