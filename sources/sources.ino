// Define constants

// engine
#define enableSideA 4
#define enableSideB 7
#define engPinA1 3
#define engPinA2 2
#define engPinB1 6
#define engPinB2 5

// car
boolean run;

// light
#define ledPin 13
boolean led_on;

// bluetooth
#define forwardByte 70
#define backwardByte 66
#define brakeByte 88
#define turnLeftByte 76
#define turnRightByte 82
#define ledLightByte 68

int incomingByte = 0;

 
void setup() {  
  // init serial rx
  Serial.begin(9600);
  
  // engine 
  pinMode(enableSideA, OUTPUT);
  pinMode(engPinA1, OUTPUT);
  pinMode(engPinA2, OUTPUT);
 
  pinMode(enableSideB, OUTPUT);
  pinMode(engPinB1, OUTPUT);
  pinMode(engPinB2, OUTPUT);
  
  // light
  pinMode(ledPin, OUTPUT);

  // initialize variables
  led_on = false;   
}
 

void loop() {
  
  // start engines
  // #motorAOn();
  // #motorBOn();
  enableMotors();
  
  if (Serial.available() > 0) {
    
    // get bluetooth incoming bytes. Works like yield in python =)
    incomingByte = Serial.read();
    
    // debug
    Serial.print("Received: ");
    Serial.println(incomingByte);
    
    // engine control
    if (incomingByte == forwardByte) {
      coast(100);
      forward(100);
    } else if (incomingByte == backwardByte) {
      coast(100);
      backward(100);
    } else if (incomingByte == brakeByte) {
      coast(100);
      brake(100);
    } else if (incomingByte == turnLeftByte) {
      coast(100);
      turnLeft(100);
    } else if (incomingByte == turnRightByte) {
      coast(100);
      turnRight(100);
    }
    
    // light control
    if (incomingByte == ledLightByte) {
      if (led_on) {
        digitalWrite(ledPin, LOW);
        led_on = false;
      } else {
        digitalWrite(ledPin, HIGH);
        led_on = true;
      }
    }
    
 }
 
 // main robobrain tact
 delay(200);
}

//////////////////////
// ENGINE FUNCTIONS //
//////////////////////

// Define Low Level H-Bridge Commands
 
// enable motors
void motorAOn() 
{
  digitalWrite(enableSideA, HIGH);
}
 
void motorBOn()
{
  digitalWrite(enableSideB, HIGH);
}
 
// disable motors
void motorAOff()
{
  digitalWrite(enableSideB, LOW);
}
 
void motorBOff()
{
  digitalWrite(enableSideA, LOW);
}
 
// motor A controls
void motorAForward()
{
  digitalWrite(engPinA1, HIGH);
  digitalWrite(engPinA2, LOW);
}
 
void motorABackward()
{
  digitalWrite(engPinA1, LOW);
  digitalWrite(engPinA2, HIGH);
}
 
// motor B contorls
void motorBForward()
{
  digitalWrite(engPinB1, HIGH);
  digitalWrite(engPinB2, LOW);
}
 
void motorBBackward()
{
  digitalWrite(engPinB1, LOW);
  digitalWrite(engPinB2, HIGH);
}
 
// coasting and braking
void motorACoast()
{
  digitalWrite(engPinA1, LOW);
  digitalWrite(engPinA2, LOW);
}
 
void motorABrake()
{
  digitalWrite(engPinA1, HIGH);
  digitalWrite(engPinA2, HIGH);
}
 
void motorBCoast()
{
  digitalWrite(engPinB1, LOW);
  digitalWrite(engPinB2, LOW);
}
 
void motorBBrake()
{
  digitalWrite(engPinB1, HIGH);
  digitalWrite(engPinB2, HIGH);
}
 
// Define High Level Commands

void enableMotors()
{
  motorAOn();
  motorBOn();
}
 
void disableMotors()
{
  motorAOff();
  motorBOff();
}
 
void forward(int time)
{
  motorAForward();
  motorBForward();
  delay(time);
}
 
void backward(int time)
{
  motorABackward();
  motorBBackward();
  delay(time);
}
 
void turnLeft(int time)
{
  motorABackward();
  motorBForward();
  delay(time);
}
 
void turnRight(int time)
{
  motorAForward();
  motorBBackward();
  delay(time);
}
 
void coast(int time)
{
  motorACoast();
  motorBCoast();
  delay(time);
}
 
void brake(int time)
{
  motorABrake();
  motorBBrake();
  delay(time);
}
