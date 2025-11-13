void setup() {
  Serial.begin(57600);
  // put your setup code here, to run once:
  Serial.println("=======================================");
  Serial.println("=        JKU BULB GENERATOR           =");
  Serial.println("=======================================");
  Serial.println("START INITIALIZATION");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("=======================================");
   // if there's any serial available, read it:
  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    int red = Serial.parseInt();
    
    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {      
      Serial.print(red); 
    }
  }
}
