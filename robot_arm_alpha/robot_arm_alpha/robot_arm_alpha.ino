/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/
#include <Servo.h>

const int SERVO0=9;
const int SERVO1=10;
const int SERVO2=11;

Servo myservo0;
Servo myservo1;
Servo myservo2;

int angle[3] = {90,90,90};
int newAngle = 0;
int index = 0;
int tempIndex = 0;
char ch;

const int maxChar = 3;
char angleVal[maxChar];
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  
  // initialize serial:
  Serial.begin(38400);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  // Define digital PWM pins for each servo
  myservo0.attach(SERVO0);
  myservo1.attach(SERVO1);
  myservo2.attach(SERVO2);
  // Initialize all servos to 90 degrees
  myservo0.write(angle[0]);
  myservo1.write(angle[1]);
  myservo2.write(angle[2]);
  
}

void loop() {
  
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // control servos using sequence stored in input string
    
    while( inputString.charAt(index) != '\n' ){
      Serial.print("Beginning of loop ");
      Serial.println(inputString.charAt(index));

      ch = inputString.charAt(index);
      ++index;
      if( inputString.charAt(index) == ':' ){
        Serial.println(inputString.charAt(index));
        index++;
        Serial.print("tempIndex: ");
        Serial.println(tempIndex);
        tempIndex = 0;
        while(isDigit(inputString.charAt(index)) ){//&& (tempIndex < maxChar)){
          Serial.println("inside angle loop");
          angleVal[tempIndex] = inputString.charAt(index);
          tempIndex++;
          index++;
        }
        Serial.print("index after angle loop: ");
        Serial.println(index);
        Serial.print("The angle value: ");
        Serial.println(atoi(angleVal));
        Serial.print("The servo: ");
        Serial.println(ch);
        // call writeServo function here
        newAngle = atoi(angleVal);
        //void writeServo( char servo, int newAngle, int angle[]) {
        writeServo( ch, newAngle, angle);

        index++;
        //tempIndex =0;

      }
      else {
        break;
      }
      //memset(angle[0], 0, maxChar);
        for (int i =0; i<=maxChar;i++){
          angleVal[i] = 'B';
        }
      //index++;
      Serial.print("the index at end of loop: ");
      Serial.println(index);
      Serial.print("char at index end of loop: ");
      Serial.println(inputString.charAt(index));
    }
    Serial.println("made it out");
    Serial.println(inputString.charAt(index));
    
    // clear the strings
    inputString = "";//"2:80,0:0,1:0,2:145,1:30,0:150,2:90,1:90,";
    index = 0;
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void writeServo( char servo, int newAngle, int angle[]) {
  int i = servo - '0'; //convert char to int
  Serial.print("the servo number is ");
  Serial.println(i);
  if(newAngle >= 0 && newAngle <= 180){

      if(newAngle < angle[i]) {

        for(; angle[i] >= newAngle; angle[i] -= 1) {
          delay(5);
          if (servo == '0'){
            myservo0.write(angle[i]);
          }
          else if (servo == '1'){
            myservo1.write(angle[i]);
            //Serial.println(angle[i]);
          }
          else {
            myservo2.write(angle[i]);
          }
        }
      }
      else 

        for(; angle[i] <= newAngle; angle[i] += 1){
          delay(5);

          if (servo == '0'){
            myservo0.write(angle[i]);
          }
          else if (servo == '1'){
            //Serial.println(newAngle);
            //Serial.println(angle[i]);

            myservo1.write(angle[i]);
          }
          else {
            myservo2.write(angle[i]);
          }
      } 
          angle[i] = newAngle;

    }
}