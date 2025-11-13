//#include "HX711.h"

#define calibration_factor_a 23 // This value is obtained using the SparkFun_HX711_Calibration sketch

#define Y1_DOUT 2
#define Y1_CLK 3
#define Y2_DOUT 4
#define Y2_CLK 5
#define X1_DOUT 6
#define X1_CLK 7
#define X2_DOUT 8
#define X2_CLK 9


//SETTINGS 
const float MOTOR_SPEED = 60; // Motor speed 0-255 (50 is better and slower)
const float MIN_SCALE_MOTOR = 40; //Min load to stop motor moving in gram at least 10
const float MAX_SCALE_MOTOR_X1 = 10000;  // Max load per Axe in gram
const float MAX_SCALE_MOTOR_X2 = 10000;  // Max load per Axe in gram
const float MAX_SCALE_MOTOR_Y1 = 10000;  // Max load per Axe in gram
const float MAX_SCALE_MOTOR_Y2 = 10000;  // Max load per Axe in gram
const float DELAY_BETWEEN_CYCLE = 10000; // Wartezeit in Milliseconds
const float MAX_CYCLES = 500; //example 260000


int enA = 10;    // Speed of the motor
int X1_in1 = A5; // X2 Motor - Analog
int X1_in2 = A4;
int X2_in1 = 11; // X2 Motor - Digital 1
int X2_in2 = 12;

int Y1_in1 = A3; // Y1 Motor - Analog
int Y1_in2 = A2;
int Y2_in1 = A1; // Y2 Motor - Analog
int Y2_in2 = A0;

int BUTTON_STOP_ON_ZERO_FORCE = 13;

HX711 scaleX1;          // Verstärker Platine of Scale X1
HX711 scaleX2;          // Verstärker Platine of Scale X2
HX711 scaleY1;          // Verstärker Platine of Scale Y1
HX711 scaleY2;          // Verstärker Platine of Scale Y2
float scaleValueX1 = 0; // current weight from the scale
float scaleValueX2 = 0; // current weight from the scale
float scaleValueY1 = 0; // current weight from the scale
float scaleValueY2 = 0; // current weight from the scale


float cyclesCounter = 0; 
float stopDone = false;
/**
* Target of this script is to sumulate temperature changes on the surface of the covers.
*
* WARNING: Make sure all Motor ropes are in loose state at begin. 
*
*/

// runs once on arduino start
void setup()
{
  Serial.begin(57600);
  Serial.println("=======================================");
  Serial.println("= TEMPERATURE COVER TESTING PROCEDURE =");
  Serial.println("=======================================");
  Serial.println("START INITIALIZATION");
 
  scaleX1.begin(X1_DOUT, X1_CLK);
  scaleX1.set_scale(calibration_factor_a); // This value is obtained by using the SparkFun_HX711_Calibration sketch
  scaleX1.tare();                          // Assuming there is no weight on the scale at start up, reset the scale to 0

  scaleX2.begin(X2_DOUT, X2_CLK);
  scaleX2.set_scale(calibration_factor_a); // This value is obtained by using the SparkFun_HX711_Calibration sketch
  scaleX2.tare();                          // Assuming there is no weight on the scale at start up, reset the scale to 0

  scaleY1.begin(Y1_DOUT, Y1_CLK);
  scaleY1.set_scale(calibration_factor_a); // This value is obtained by using the SparkFun_HX711_Calibration sketch
  scaleY1.tare();                          // Assuming there is no weight on the scale at start up, reset the scale to 0

  scaleY2.begin(Y2_DOUT, Y2_CLK);
  scaleY2.set_scale(calibration_factor_a); // This value is obtained by using the SparkFun_HX711_Calibration sketch
  scaleY2.tare();                          // Assuming there is no weight on the scale at start up, reset the scale to 0

  // TODO: check why this kind of initialization do not work
  initScale(scaleX1, X1_DOUT, X1_CLK, "X1");
  initScale(scaleX2, X2_DOUT, X2_CLK, "X2");
  initScale(scaleY1, Y1_DOUT, Y1_CLK, "Y1");
  initScale(scaleY2, Y2_DOUT, Y2_CLK, "Y2");
  Serial.println();

  pinMode(BUTTON_STOP_ON_ZERO_FORCE, INPUT_PULLUP); // needed to avoid adding additional resistor for the button
  pinMode(enA, OUTPUT);                             // Speeed

  pinMode(X2_in1, OUTPUT);
  pinMode(X2_in2, OUTPUT);

  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);

  analogWrite(enA, MOTOR_SPEED); // Motor speed 50-255
}

void initScale(HX711 scale, float dout, float clk, char scaleName[])
{
  Serial.print("initScale ");
  Serial.print(scaleName);
  scale.begin(dout, clk);
  scale.set_scale(calibration_factor_a); // This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();                          // Assuming there is no weight on the scale at start up, reset the scale to 0
  Serial.print(" ");
}

float readScaleValue(HX711 scale, char scaleName[])
{
  float val = scale.get_units(); // read weight from scale
  Serial.print(scaleName);
  Serial.print(" ");
  Serial.print(10 * round(val / 10)); //here you can adjust the rounding of the weight 
  Serial.print(" ");
  return val;
}

void readAll()
{
  scaleValueX1 = readScaleValue(scaleX1, "X1");
  scaleValueX2 = readScaleValue(scaleX2, "X2");
  scaleValueY1 = readScaleValue(scaleY1, "Y1");
  scaleValueY2 = readScaleValue(scaleY2, "Y2");
  Serial.println();
}

/**
* Stops one full cycle when all motors are on loose 
*/
void controlLoop()
{
  stopDone = 0;
  // Auszeitloop (Schalter)
  // stops when all scales are under 100 and the schalter is pressed
  while (scaleValueX1 <= MIN_SCALE_MOTOR && scaleValueX2 <= MIN_SCALE_MOTOR && scaleValueY1 <= MIN_SCALE_MOTOR && scaleValueY2 <= MIN_SCALE_MOTOR && digitalRead(BUTTON_STOP_ON_ZERO_FORCE) == HIGH)
  {
    if (stopDone == 0) {
      Serial.println("Button Off"); // we hang in the loop
      Serial.print("Cycles done "); Serial.println(cyclesCounter);
      Serial.println();
      stopMotorAll();
    }
    stopDone = 1;
  }
}

void stopMotorAll() {
    Serial.println("STOP ALL MOTORS");
    stopMotorX1();
    stopMotorX2();
    stopMotorY1();
    stopMotorY2();
}

void stopMotorX1() {
    //Serial.print("STOP X1 ");
    digitalWrite(X1_in1, LOW); // bremsen  der Last High High
    digitalWrite(X1_in2, LOW);
}

void stopMotorX2() {
    //Serial.print("STOP X2 ");
    digitalWrite(X2_in1, LOW); // bremsen  der Last High High
    digitalWrite(X2_in2, LOW);
}

void stopMotorY1() {
    //Serial.print("STOP Y1 ");
    digitalWrite(Y1_in1, LOW); // bremsen  der Last High High
    digitalWrite(Y1_in2, LOW);
}

void stopMotorY2() {
    //Serial.print("STOP Y2 ");
    digitalWrite(Y2_in1, LOW); // bremsen  der Last High High
    digitalWrite(Y2_in2, LOW);
}

void tightMotorX1() {
    digitalWrite(X1_in1, LOW);
    digitalWrite(X1_in2, HIGH);
}

void tightMotorX2() {
    digitalWrite(X2_in1, LOW);
    digitalWrite(X2_in2, HIGH);
}

void tightMotorY1() {
    digitalWrite(Y1_in1, LOW);
    digitalWrite(Y1_in2, HIGH);
}

void tightMotorY2() {
    digitalWrite(Y2_in1, LOW);
    digitalWrite(Y2_in2, HIGH);
}

void looseMotorX1() {
  digitalWrite(X1_in1, HIGH);
  digitalWrite(X1_in2, LOW);
}

void looseMotorX2() {
  digitalWrite(X2_in1, HIGH);
  digitalWrite(X2_in2, LOW);
}

void looseMotorY1() {
  digitalWrite(Y1_in1, HIGH);
  digitalWrite(Y1_in2, LOW);
}

void looseMotorY2() {
  digitalWrite(Y2_in1, HIGH);
  digitalWrite(Y2_in2, LOW);
}

void cycle() {
  {
    Serial.println();
    Serial.print("Start Cycle "); 
    Serial.println(++cyclesCounter);
    Serial.println("START ALL MOTORS TO TIGHTEN");
    tightMotorX1();
    tightMotorX2();
    tightMotorY1();
    tightMotorY2();
    do
    {
      //scaleValueX2 = scaleX2.get_units();
      readAll();
      if (scaleValueX1 > MAX_SCALE_MOTOR_X1) {
          stopMotorX1();
      }
      if (scaleValueX2 > MAX_SCALE_MOTOR_X2) {
          stopMotorX2();
      }
      if (scaleValueY1 > MAX_SCALE_MOTOR_Y1) {
          stopMotorY1();
      }
      if (scaleValueY2 > MAX_SCALE_MOTOR_Y2) {
          stopMotorY2();
      }
    } while (scaleValueX1 < MAX_SCALE_MOTOR_X1 || scaleValueX2 < MAX_SCALE_MOTOR_X2 || scaleValueY1 < MAX_SCALE_MOTOR_Y1 || scaleValueY2 < MAX_SCALE_MOTOR_Y2);

    // deleteme if (scaleValue < 100 ) ;
    stopMotorAll();

    Serial.println("WAIT");
    delay(DELAY_BETWEEN_CYCLE);

    // und zurück
    Serial.println("START ALL MOTORS TO LOOSEN");
    looseMotorX1();
    looseMotorX2();
    looseMotorY1();
    looseMotorY2();
    do
    {
      readAll();
      if (scaleValueX1 < MIN_SCALE_MOTOR -30 ) {
          stopMotorX1();
      }
      if (scaleValueX2 < MIN_SCALE_MOTOR -30) {
          stopMotorX2();
      }
      if (scaleValueY1 < MIN_SCALE_MOTOR -30) {
          stopMotorY1();
      }
      if (scaleValueY2 < MIN_SCALE_MOTOR -30) {
          stopMotorY2();
      }
    } while (scaleValueX1 > MIN_SCALE_MOTOR || scaleValueX2 > MIN_SCALE_MOTOR || scaleValueY1 > MIN_SCALE_MOTOR || scaleValueY2 > MIN_SCALE_MOTOR );


    stopMotorAll();  
    delay(DELAY_BETWEEN_CYCLE);
  }
}

// Main loops all the time , til error
void loop()
{
  if (cyclesCounter<=MAX_CYCLES) {
    readAll();
    controlLoop();
    cycle();
  } 
  if (cyclesCounter>=MAX_CYCLES) {
    Serial.println("GOODBEY!");
    digitalWrite(BUTTON_STOP_ON_ZERO_FORCE, HIGH);
    delay(1000);
  }
}
