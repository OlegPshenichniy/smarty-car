#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

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

// led rgb
int redpin = 11; 
int bluepin = 10;
int greenpin = 9;
int rgbpowerpin = 8;
int rgb_val;
boolean rgb_on;

// bluetooth
#define forwardByte 70
#define backwardByte 66
#define brakeByte 88
#define turnLeftByte 76
#define turnRightByte 82
#define ledLightByte 68
#define rgbLightByte 79
int incomingByte = 0;

// LCD
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ultrasonic
#define echoPin 12 // Echo Pin
#define trigPin 11 // Trigger Pin
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

 
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
  
  // led rgb
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(rgbpowerpin, OUTPUT);
  
  // ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // LCD
  lcd.begin();
  lcd.backlight(); 
  lcd.print("Distance sm: ");
  lcd.setCursor(6, 1);

  // initialize variables
  led_on = false;
  rgb_on = false;  
}
 

void loop() {
  
  // start engines
  // #motorAOn();
  // #motorBOn();
  enableMotors();
  
  // ultrasonic get distance
  distance = getDistance();
  // display on lcd
  lcd.print(distance);
  lcd.print("      ");
  
  // check safe distance
  if (distance < 20) {
    coast(100);
    brake(100);
  } 

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
    
    // rgb light control
    if (incomingByte == rgbLightByte) {
      if (rgb_on) {
        digitalWrite(rgbpowerpin, LOW);
        rgb_on = false;
      } else {
        digitalWrite(rgbpowerpin, HIGH);
        rgb_on = true;
      }
    }
    
 }
 
 // chage rgb color
 if (rgb_on) {
    ledRgbOn();
 } 
 
 // main robobrain tact
 delay(200);
 
 // move lcd cursor
 lcd.setCursor(6, 1);
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

// led rgb function
void ledRgbOn() 
{
    analogWrite (11, random(0, 255));
    analogWrite (10, random(0, 255));
    analogWrite (9, random(0, 255));
}

// ultrasonic
int getDistance() {
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  
  return distance;
}
