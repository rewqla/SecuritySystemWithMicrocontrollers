#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  if (mySerial.available()) {
    SendMessage();
  } else {
    Serial.println("Device is not connected or failed to initialize.");
  }
}

void loop() {

}

void SendMessage(){
  Serial.println("Send a message");
  mySerial.println("AT+CSCS=\"UCS2\""); // Set character set to UCS2 (Unicode)
  delay(1000);
  mySerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  mySerial.println("AT+CMGS=\"+\""); //Add a phone number
  delay(1000);
  mySerial.println("Send sms");
  delay(1000);
  mySerial.println((char)26); // Send Ctrl+Z to end the message
  delay(1000);

  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  
  Serial.println("End of function");
}
