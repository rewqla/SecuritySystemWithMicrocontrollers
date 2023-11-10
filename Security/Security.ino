#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

const int pirPin = 7;
const int trigPin = 5;
const int echoPin = 2;
const int securityDistance = 50; 
long distance = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  calculateDistance();
  
  if (distance > 0 && distance < securityDistance) {
    Serial.println("Object Found");
    
    int currentState = digitalRead(pirPin);

    if (currentState == HIGH) {          
       Serial.println("Intruder detected! Security alert!");
       //SendMessage();
    } else {
      Serial.println("No intruder within the security range.");
    }
  } else {
    Serial.println("No Object Found");
  }
  delay(1000);
}

void SendMessage() {
  Serial.println("Send a message");
  mySerial.println("AT+CSCS=\"UCS2\"");
  delay(1000);
  mySerial.println("AT+CMGF=1"); 
  delay(1000);
  mySerial.println("AT+CMGS=\"+\""); 
  delay(1000);
  mySerial.println("Intruder detected");
  delay(1000);
  mySerial.println((char)26); 
  delay(1000);

  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

long calculateDistance() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  int duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.0;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}
