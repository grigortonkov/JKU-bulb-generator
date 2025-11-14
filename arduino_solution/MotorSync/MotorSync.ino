//#include "HX711.h"

// 4 Motoren die Synchron bewegt werden
// -------------------
//      M1  M2  M3  M4
// -------------------
// PU+  A0  A1  10  11 //Pulse signal input positive end
// DR+  A4  A5   2   3 //Direction signal input positive end
// MF+   4   5   6   7 //Enable signal input positive end
// ALM+  8   9  A2  A3 //Alarm signal output positive end
// -------------------
// Einschalten Box: 12
// Endscahlter: 13

int M1_PU = A0; //Pulse
int M1_DR = A4; //Direction
int M2_in1 = A1;
int M2_in2 = A5;
int M3_in1 = A2;
int M3_in2 = 2;
int M4_in1 = A3;
int M4_in2 = 3;

int EINSCHALTEN_BOX_PIN = 12;
int ENDSCHALTER_PIN = 13;

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

  pinMode(EINSCHALTEN_BOX_PIN, INPUT);
  pinMode(ENDSCHALTER_PIN, INPUT);
}

/**
* move in mm
*/
void moveLinear(int M1_PUL, float mm) {
  // der Motor microstept mit 40000 Steps für eine Umdrehung
  float ueberstzung = 48.0/32.0;
  Serial.println(ueberstzung);
  long microsteps = 40000*ueberstzung*mm;
  Serial.print("Move steps:");
  Serial.println(microsteps);

    for(long x = 0; x < microsteps; x++) // repeat 400 times a revolution when setting 400 on driver
      {
        int endschalterErreicht = digitalRead(ENDSCHALTER_PIN);
        if (endschalterErreicht==1) {
          digitalWrite(M1_PUL,HIGH); // Output high
          delayMicroseconds(50); // set rotate speed
          digitalWrite(M1_PUL,LOW); // Output low
          delayMicroseconds(50); // set rotate speed
        }
      }
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
    digitalWrite(M1_PU, LOW); // bremsen  der Last High High
    digitalWrite(M1_DR, LOW);
}

void moveMotorDownM1(float mm) {
    Serial.println("M1 UP");
    digitalWrite(M1_DR, HIGH); // set high level direction
    moveLinear(M1_PU, mm); 
}

void moveMotorUpM1(float mm) {
    Serial.println("M1 DOWN");
    digitalWrite(M1_DR, LOW); // set high level direction
    moveLinear(M1_PU, mm); 
}

void calibrate() {
  Serial.println("calibrate START");

  float totalDistance = 0;
  float MAX_DISTANCE = 30.0;
  for (int stepInMM = 0; stepInMM < MAX_DISTANCE; stepInMM++) {
      int endschalterErreicht = digitalRead(ENDSCHALTER_PIN);
      if (endschalterErreicht==1) {
        moveMotorDownM1(1.0);
        totalDistance += 1.0;
        Serial.print("Reached:");
        Serial.println(totalDistance);
      }
  }


   Serial.println("calibrate END");
}

int boxIsOnState=-1, endschalterErreichtState=-1;
void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("=======================================");

  int boxIsOn = digitalRead(EINSCHALTEN_BOX_PIN);
  int endschalterErreicht = digitalRead(ENDSCHALTER_PIN);
 
  if (boxIsOnState != boxIsOn) {
    Serial.print("Device ON:");
    Serial.println(boxIsOn);
    boxIsOnState = boxIsOn;
   }
   if (endschalterErreichtState != endschalterErreicht) {
   Serial.print("Bottom reached:");
   Serial.println(endschalterErreicht);
   endschalterErreichtState = endschalterErreicht;
   }
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
      else if (input == COMMAND_UP) {
        moveMotorUpM1(1.);
      }
      else if (input == COMMAND_DOWN) {
        moveMotorDownM1(1.);
      }
      else if (input == COMMAND_CALIBRATE) {
        calibrate();
      } else {
      targetHeight = input;
      }
    }
  }
  Serial.print("targetHeight:");
  char* buffer;
  Serial.println(targetHeight);
  delay(1000);
} 