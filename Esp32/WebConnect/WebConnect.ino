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

}
