void setup () {
  Serial.begin(9600);
  pinMode(5, OUTPUT); // Trig
  pinMode(2, INPUT);  // Echo
}
void loop () {
  digitalWrite(5, LOW);  // clear the signal so that there are no errors
  delayMicroseconds(2);
  digitalWrite(5, HIGH); // send ultrasonic signal
  delayMicroseconds(10); 
  digitalWrite(5, LOW);  // switch off
  int duration = pulseIn(2, HIGH); // count the seconds until the reflected signal arrives
  float cm = duration / 58;
  Serial.println(cm);
  delay(500);
}
