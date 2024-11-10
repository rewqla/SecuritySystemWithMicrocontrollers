#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

const char* serverName = "http://192.168.1.102:3000";
const char* configEndpoint = "/api/configuration";

const char* serialNumber = "51170740-312f-4c81-bc33-997c220cba83";

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

    String url = String(serverName) + String(configEndpoint) + "?serialNumber="+ String(serialNumber);
  Serial.println(url);
    http.begin(url);

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

  delay(10000); 
}
