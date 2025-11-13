//#include "HX711.h"

// 4 Motoren die Synchron bewegt werden
// -------------------
//      M1  M2  M3  M4
// -------------------
// PU+  A0  A1  10  11
// DR+  A4  A5   2   3 
// MF+   4   5   6   7 
// ALM+  8   9  A2  A3
// -------------------
// Einschalten Box: 12
// Endscahlter: 13

int M1_in1 = 4; //A0;
int M1_in2 = 8; //A4;
int M2_in1 = A1;
int M2_in2 = A5;
int M3_in1 = A2;
int M3_in2 = 2;
int M4_in1 = A3;
int M4_in2 = 3;

int targetHeight = 0;

int currentHeight = 0;

int COMMAND_STOP = 0;
int COMMAND_UP = 1;
int COMMAND_DOWN = 2;
int COMMAND_CALIBRATE = 999;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println("=======================================");
  Serial.println("=        JKU BUBLE GENERATOR          =");
  Serial.println("=======================================");

  // set pin mode 
  //M1
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
}

void stopMotorAll() {
    Serial.println("STOP ALL MOTORS");
    stopMotorM1();
    //stopMotorM2();
    //stopMotorM3();
    //stopMotorM4();
}

void stopMotorM1() {
    Serial.println("M1 STOP");
    digitalWrite(M1_in1, LOW); // bremsen  der Last High High
    digitalWrite(M1_in2, LOW);
}

void moveMotorDownM1() {
    Serial.println("M1 UP");
    digitalWrite(M1_in1, LOW);
    digitalWrite(M1_in2, HIGH);
}

void moveMotorUpM1() {
    Serial.println("M1 DOWN");
    digitalWrite(M1_in1, HIGH);
    digitalWrite(M1_in2, LOW);
}

void calibrate() {
  Serial.println("M1 calibrate");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("=======================================");
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    int input = Serial.parseInt();
    
    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {      
      // Serial.print(input); 
      if (input == COMMAND_STOP) {
        stopMotorAll();
      }
      if (input == COMMAND_UP) {
        moveMotorUpM1();
      }
      if (input == COMMAND_DOWN) {
        moveMotorDownM1();
      }
      if (input == COMMAND_CALIBRATE) {
        calibrate();
      }
      targetHeight = input;
    }
  }
  Serial.print("targetHeight:");
  char* buffer;
  Serial.println(targetHeight);
  delay(1000);
} 