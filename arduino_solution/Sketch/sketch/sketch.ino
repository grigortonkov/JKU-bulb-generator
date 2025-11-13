int targetHeight = 0;

int currentHeight = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println("=======================================");
  Serial.println("=        JKU BUBLE GENERATOR          =");
  Serial.println("=======================================");
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
      Serial.print(input); 
      targetHeight = input;
    }
  }
  Serial.print("targetHeight:");
  char* buffer;
  Serial.print(targetHeight);
  Serial.print("\n");
  delay(1000);
} 