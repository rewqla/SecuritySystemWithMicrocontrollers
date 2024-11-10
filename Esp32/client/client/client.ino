#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "MERCUSYS_770A";
const char* password = "oleh76moha";

const char* serverName = "http://192.168.1.102:3000";
const char* configEndpoint = "/api/configuration";

const char* serialNumber = "51170740-312f-4c81-bc33-997c220cba83";
bool isLegalDevice = false;

void setup() {
  Serial.begin(115200);
  delay(3000);
  WiFi.begin(ssid, password);
  Serial.println("Starting connect to Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  initRequest();
}

void loop() {

  delay(10000);  
}

void initRequest() {
  if (WiFi.status() == WL_CONNECTED) {  
    HTTPClient http;
    String url = String(serverName) + String(configEndpoint) + "?serialNumber=" + String(serialNumber);
    Serial.println("Requesting URL: " + url);

    http.begin(url); 
    int httpResponseCode = http.GET(); 
    
    if (httpResponseCode > 0) {
      if (httpResponseCode == 200) {
        isLegalDevice = true;
        String response = http.getString();
        Serial.println("HTTP Response code: 200 (OK)");
        Serial.println("Response: " + response);
      } else if (httpResponseCode == 404) {
        Serial.println("Error 404: Invalid serial number");
      } else {
        Serial.println("Error: HTTP Response code " + String(httpResponseCode));
      }
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  } else {
    Serial.println("Error: Not connected to Wi-Fi");
  }
}
