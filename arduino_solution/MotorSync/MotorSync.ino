//#include "HX711.h"

// 4 Motoren die Synchron bewegt werden
// -------------------
//      M1  M2  M3  M4
// -------------------
// PU+  A0  A1  10  11 // Pulse signal input positive end
// DR+  A4  A5   2   3 // Direction signal input positive end
// MF+   4   5   6   7 // Enable signal input positive end
// ALM+  8   9  A2  A3 // Alarm signal output positive end
// -------------------
// Einschalten Box: 12
// Endscahlter: 13

int M1_PU = A0; // Pulse
int M1_DR = A4; // Direction
int M2_PU = A1;
int M2_DR = A5;
int M3_PU = 10;
int M3_DR = 2;
int M4_PU = 11;
int M4_DR = 3;

int EINSCHALTEN_BOX_PIN = 12;
int ENDSCHALTER_PIN = 13;

float targetHeight = 0;

float currentHeight = -999; // needs to calibrate first

int COMMAND_STOP = 0;
int COMMAND_UP = -1;
int COMMAND_DOWN = 1;
int COMMAND_CALIBRATE = 999;

const float MAX_DISTANCE = 5.00; //200.0; // 200 mm Bewegungsfreiheit von 0 bis Max Point // der wert muss einmal ausgemessen werden

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
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  pinMode(EINSCHALTEN_BOX_PIN, INPUT);
  pinMode(ENDSCHALTER_PIN, INPUT);
  Serial.println("Waiting for commands. Any number. 0-Stop, 1..200-move, 999-calibrate.");
  printStatus();
}

/**
* move in mm
*/
void moveLinear(int M1_PUL, float mm) {
  // der Motor microstept mit 40000 Steps für eine Umdrehung
  float ueberstzung = 48.0/32.0;
  // Serial.println(ueberstzung);
  long microsteps = 40000*ueberstzung*mm;
  // Serial.print("Move steps:");
  // Serial.println(microsteps);

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

/**
* move in mm
*/
void moveLinearAll(float mm) {
  int boxIsOn = digitalRead(EINSCHALTEN_BOX_PIN);
  if (boxIsOn == 1) {
    // User hat die Box nicht aktiviert 
    Serial.println("Box is OFF");
    return;
  }
  // der Motor microstept mit 40000 Steps für eine Umdrehung
  float ueberstzung = 48.0/32.0;
  // Serial.println(ueberstzung);
  long microsteps = 40000*ueberstzung*mm;
  // Serial.print("Move steps:");
  // Serial.println(microsteps);

    for(long x = 0; x < microsteps; x++) // repeat 400 times a revolution when setting 400 on driver
      {
        int endschalterErreicht = digitalRead(ENDSCHALTER_PIN);
        if (endschalterErreicht==1) {
          digitalWrite(M1_PU,HIGH); // Output high
          digitalWrite(M2_PU,HIGH); // Output high
          digitalWrite(M3_PU,HIGH); // Output high
          digitalWrite(M4_PU,HIGH); // Output high

          delayMicroseconds(50); // set rotate speed

          digitalWrite(M1_PU,LOW); // Output low
          digitalWrite(M2_PU,LOW); // Output low
          digitalWrite(M3_PU,LOW); // Output low
          digitalWrite(M4_PU,LOW); // Output low

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

void moveMotorDownAll(float mm) {
    Serial.println("M ALL DOWN");
    digitalWrite(M1_DR, HIGH); // set high level direction
    digitalWrite(M2_DR, HIGH); // set high level direction
    digitalWrite(M3_DR, HIGH); // set high level direction
    digitalWrite(M4_DR, HIGH); // set high level direction
    moveLinearAll(mm);
    printStatus();
}

void moveMotorUpAll(float mm) {
    Serial.println("M ALL UP");
    digitalWrite(M1_DR, LOW); // set high level direction
    digitalWrite(M2_DR, LOW); // set high level direction
    digitalWrite(M3_DR, LOW); // set high level direction
    digitalWrite(M4_DR, LOW); // set high level direction
    moveLinearAll(mm);
    printStatus();
}

void moveMotorDownM1(float mm) {
    Serial.println("M1 DOWN");
    digitalWrite(M1_DR, HIGH); // set high level direction
    moveLinear(M1_PU, mm);
    printStatus();
}

void moveMotorUpM1(float mm) {
    Serial.println("M1 UP");
    digitalWrite(M1_DR, LOW); // set high level direction
    moveLinear(M1_PU, mm);
    printStatus();
}

void moveMotorTo(float position) {
 Serial.print("moveMotorTo:");  Serial.println(position);
 targetHeight = position;
 if (currentHeight>=0) {
   while (abs(currentHeight-targetHeight)>=0.1) {
     Serial.print("Move from ");
     Serial.print(currentHeight);
     Serial.print(" to ");
     Serial.println(targetHeight);
      if (currentHeight<targetHeight) {
        moveMotorDownAll(0.1);
        currentHeight += 0.1;
      }
      if (currentHeight>targetHeight) {
        moveMotorUpAll(0.1);
        currentHeight -= 0.1;
      }
      printStatus();
   }
 } else {
  Serial.println("Calibrate first!");
 }
}

void printStatus() {
  int boxIsOn = digitalRead(EINSCHALTEN_BOX_PIN);
  int endschalterErreicht = digitalRead(ENDSCHALTER_PIN);

  Serial.print("{Status:{");
  Serial.print("targetHeight:");
  Serial.print(targetHeight);
  Serial.print(",currentHeight:");
  Serial.print(currentHeight);
  Serial.print(",boxIsOn:");
  Serial.print(boxIsOn);
  Serial.print(",endschalterErreicht:");
  Serial.print(endschalterErreicht);
  Serial.print("}}");
  Serial.println();
}

/**
Nach unten bewegen bis endschalterErreicht dann auf Position 0
*/
void calibrate() {
  Serial.println("calibrate START");

  float totalDistance = 0;

  // Nach unten bewegen bis endschalterErreicht 
  for (int stepInMM = 0; stepInMM < MAX_DISTANCE; stepInMM++) {
      int endschalterErreicht = digitalRead(ENDSCHALTER_PIN);
      if (endschalterErreicht==1) {
        moveMotorDownAll(1.0);
        totalDistance += 1.0;
        Serial.print("Reached:");
        Serial.println(totalDistance);
      }
  }
    currentHeight = MAX_DISTANCE;
    for (int stepInMM = 0; stepInMM < MAX_DISTANCE-1; stepInMM++) {
        moveMotorUpAll(1.0);
        currentHeight -=1;
        printStatus();
    }

   Serial.println("calibrate END");
}

int boxIsOnState=-1, endschalterErreichtState=-1;
void loop() {
  // put your main code here, to run repeatedly:


  int boxIsOn = digitalRead(EINSCHALTEN_BOX_PIN);
  int endschalterErreicht = digitalRead(ENDSCHALTER_PIN);

  if (boxIsOnState != boxIsOn) {
    //Serial.print("Device ON:");
    //Serial.println(boxIsOn);
    boxIsOnState = boxIsOn;
   }
   if (endschalterErreichtState != endschalterErreicht) {
   //Serial.print("Bottom reached:");
   //Serial.println(endschalterErreicht);
   endschalterErreichtState = endschalterErreicht;
   }
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    int input = Serial.parseInt();
    // String strInput = Serial.readString();
    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {      
      // Serial.print(input); 
      printStatus();
      // Serial.print(strInput); 
      if (input == COMMAND_STOP) {
        stopMotorAll();
        printStatus();
      }
      else if (input == COMMAND_UP) {
        // moveMotorUpM1(1.);
        moveMotorUpAll(1.);
        printStatus();
      }
      else if (input == COMMAND_DOWN) {
        //moveMotorDownM1(1.);
        moveMotorDownAll(1.);
        printStatus();
      }
      else if (input == COMMAND_CALIBRATE) {
        calibrate();
        printStatus();
      } else {
        if (input < MAX_DISTANCE) { 
            moveMotorTo(input);
            printStatus();
        }
      }
    }
  }

  delay(1000);
} 