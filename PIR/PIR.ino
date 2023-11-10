const int pirPin = 12;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
}

void loop() {
  int currentState = digitalRead(pirPin);

  if (currentState == HIGH) {
    // Add a brief delay before confirming motion to reduce false positives
    delay(500);

    // Check the PIR sensor state again
    int currentState2 = digitalRead(pirPin);

    if (currentState2 == HIGH) {
      Serial.println("Motion detected");
    }
  } else {
    Serial.println("No motion detected");
  }

  delay(1000);
}
