int buzzer = 9;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  playTone(122, 200);  
  delay(200);          
  
  playTone(250, 200); 
  delay(200);
}

void playTone(int frequency, int duration) {
  tone(buzzer, frequency, duration);  
  delay(duration + 20);                   
}
