const int vibrationPin = 11; // Connect the SW-520D signal pin to digital pin 2

void setup() {
  Serial.begin(9600);
  pinMode(vibrationPin, INPUT);
}

void loop() {
  int vibrationValue = digitalRead(vibrationPin);

  if (vibrationValue == HIGH) {
    Serial.println("Vibration detected!");
  } else {
    Serial.println("No vibration");
  }

  delay(500); // Adjust the delay as needed
}
