#include "BluetoothSerial.h"

// general vars
char state = 'M';  // M=> manual A=> auto
char command;

// RC vars
char readings = 0;
BluetoothSerial Bluetooth;
unsigned long Last_time_command = 0;

// Flame System Vars:


// RC bins
#define motorA1 12  // IN1
#define motorA2 14  // IN2
#define motorB1 27  // IN3
#define motorB2 26  // IN4
#define ENA 13      // PWM for motor A
#define ENB 25      // PWM for motor B




void setup() {
  // 115200 is a faster, more standard speed for ESP32 debugging
  Serial.begin(9600);
  // servos

  // RC motors

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // THIS IS THE CORRECT LINE - with double quotes for the name
  Bluetooth.begin("yehia");
  command = state;
}

void loop() {



  if (command == 'M') {
    state = command;

  } else if (command == 'A') {
    state = command;
  }
  if (state == 'M') {
    if (Bluetooth.available()) {
      readings = Bluetooth.read();
      Serial.println("the user is holding");
      Last_time_command = millis();

    } else {
      readings = 0;
    }
    if (readings != 0) {
      command = readings;
      Last_time_command = millis();
    } else {

      Serial.println("the user is not holding");
    }



    Serial.println("reading:");
    Serial.println(readings);
    Manual_Control(command);

  } else if (state == 'A') {

    Auto_control();
  }
}

void Auto_control() {
}
void Manual_Control(char command) {

  if (command != 0) {
    // Read command from Bluetooth
    Serial.println("Received: ");  // Add this for debugging
    Serial.print(command);         // Add this for debugging
    controlMotors(command);
  }
  if (millis() - Last_time_command > 200) {
    Serial.println("No order recieved the car will stop");
    controlMotors('S');
      Last_time_command = millis();
  }
}
void controlMotors(char command) {
  switch (command) {
    case 'F':                       // Move Forward
      digitalWrite(motorA1, HIGH);  //
      digitalWrite(motorA2, LOW);   // RIGHT

      digitalWrite(motorB1, LOW);  //RIGHT
      digitalWrite(motorB2, HIGH);
      analogWrite(ENA, 255);  // Set speed (max)
      analogWrite(ENB, 255);  // Set speed (max)
      break;
    case 'B':                       // Move Backward
      digitalWrite(motorA1, LOW);   //LEFT FORWARD
      digitalWrite(motorA2, HIGH);  // RIGHT BACK
      digitalWrite(motorB1, HIGH);  // LEFT  BACK
      digitalWrite(motorB2, LOW);   ///RIGHT FORWARD
      analogWrite(ENA, 255);        // Set speed (max)
      analogWrite(ENB, 255);        // Set speed (max)
      break;
    case 'L':  // Turn Left
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, HIGH);
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, HIGH);
      analogWrite(ENA, 255);  // Set speed (max)
      analogWrite(ENB, 255);  // Set speed (max)
      break;
    case 'R':                       // Turn Right
      digitalWrite(motorA1, HIGH);  //LEFT
      digitalWrite(motorA2, LOW);   //RIGHT
      digitalWrite(motorB1, HIGH);  //LEFT
      digitalWrite(motorB2, LOW);   //RIGHT
      analogWrite(ENA, 255);        // Set speed (max)
      analogWrite(ENB, 255);        // Set speed (max)
      break;
    case 'S':  // Stop
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, LOW);
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, LOW);
      break;

    
    default:  // Default case
      break;
  }
}