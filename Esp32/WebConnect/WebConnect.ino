#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "MERCUSYS_770A";
const char* password = "oleh76moha";

const char* serverName = "https://dummyjson.com/users/1";

void setup() {
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
  Serial.println("Starting connect to Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {  
    HTTPClient http;

    http.begin("https://dummyjson.com/users/1");

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end(); 
  }

  delay(1000); 
}
