const int echoPin = 2;
const int trigPin = 5;
const int pirPin = 7;
const int buzzer = 9;
const int vibrationPin = 11;

const int securityDistance = 50; 
long distance = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(vibrationPin, INPUT);
}

void loop() {
  distance = calculateDistance();
  
  if (distance > 0 && distance < securityDistance) {
    Serial.println("Object Found");
    
    int currentState = digitalRead(pirPin);
    int vibrationValue = digitalRead(vibrationPin);
    
    if (currentState == HIGH || vibrationValue == HIGH) {          
       Serial.println("Intruder detected! Security alert!");
       for (int i = 0;i<5;i++){
        playTone(122, 200);  
        delay(200);          
  
        playTone(250, 200); 
        delay(200);
       }
    } else {
      Serial.println("No intruder within the security range.");
    }
  } else {
    Serial.println("No Object Found");
  }
  delay(1000);
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

void playTone(int frequency, int duration) {
  tone(buzzer, frequency, duration);  
  delay(duration + 20);                   
}
