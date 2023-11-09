#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

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
      SendMessage();
    } else {
      Serial.println("No intruder within the security range.");
    }
  } else {
    Serial.println("No motion detected.");
  }
  
  delay(1000);
}

void SendMessage(){
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
