const int pirPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
}

void loop() {
  int currentState = digitalRead(pirPin);

  if (currentState == HIGH) {
    Serial.println("Object is detected");
  } else {
    Serial.println("No object");
  }
  
  delay(1000);
}
