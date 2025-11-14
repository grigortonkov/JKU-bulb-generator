// source: https://help.stepperonline.com/en/article/can-you-send-me-a-schematic-that-how-to-wire-the-stepper-driver-to-an-arduino-d5ot82/
long x;
int M1_PUL = A0;
int M1_DIR = A4;

void setup() {
   Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println("=======================================");

  pinMode(M1_PUL,OUTPUT); // set Pin9 as PUL
  pinMode(M1_DIR,OUTPUT); // set Pin8 as DIR
}

/**
* move in mm
*/
void moveLinear(int mm) {
  // der Motor microstept mit 40000 Steps für eine Umdrehung
  float ueberstzung = 48.0/32.0;
  Serial.println(ueberstzung);
  long microsteps = 40000*ueberstzung*mm;
  Serial.print("Move steps:");
  Serial.println(microsteps);

    for(long x = 0; x < microsteps; x++) // repeat 400 times a revolution when setting 400 on driver
      {
        digitalWrite(M1_PUL,HIGH); // Output high
        delayMicroseconds(50); // set rotate speed
        digitalWrite(M1_PUL,LOW); // Output low
        delayMicroseconds(50); // set rotate speed
      }
}

void loop() {
  Serial.println("START");
  moveLinear(1);
  delay(1000);
    /*
  digitalWrite(M1_DIR,HIGH); // set high level direction
  // der Motor microstept mit 40000 Steps für eine Umdrehung
  for (int y = 0;y<2; y++) {
    for(x = 0; x < 20000; x++) // repeat 400 times a revolution when setting 400 on driver
      {
        digitalWrite(M1_PUL,HIGH); // Output high
        delayMicroseconds(50); // set rotate speed
        digitalWrite(M1_PUL,LOW); // Output low
        delayMicroseconds(50); // set rotate speed
      }
    }
  delay(1000); //pause 1 second
  

  digitalWrite(M1_DIR,LOW); // set high level direction
  for(x = 0; x < 4000; x++)
  {
    digitalWrite(M1_PUL,HIGH);
    delayMicroseconds(500);
    digitalWrite(M1_PUL,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
  */
  Serial.println("END");
}