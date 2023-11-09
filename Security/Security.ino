const int pirPin = 3;
const int trigPin = 5;
const int echoPin = 2;
const int securityDistance = 50; 

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int currentState = digitalRead(pirPin);
  
  if (currentState == HIGH) {
    Serial.println("Motion detected!");
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    int duration = pulseIn(echoPin, HIGH);
    float cm = duration / 58.0;
    
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");

    if (cm < securityDistance) {
      Serial.println("Intruder detected! Security alert!");
    } else {
      Serial.println("No intruder within the security range.");
    }
  } else {
    Serial.println("No motion detected.");
  }
  
  delay(1000);
}
