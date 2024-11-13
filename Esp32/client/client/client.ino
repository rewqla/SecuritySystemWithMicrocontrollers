#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "MERCUSYS_770A";
const char* password = "oleh76moha";

const int echoPin = 2;
const int trigPin = 4;
const int pirPin = 5;
const int buzzerPin = 22;

const char* serverName = "http://192.168.1.102:3000";
const char* configEndpoint = "/api/configuration";

const char* serialNumber = "51170740-312f-4c81-bc33-997c220cba83";
bool isLegalDevice = false;

String startTime;
String endTime;
int distanceThreshold = 30;
String enabledDevices[10];
int enabledDevicesCount = 0;

void setup() {
  Serial.begin(115200);
  delay(3000);
  WiFi.begin(ssid, password);
  Serial.println("Starting the program");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  initRequest();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
    //float distance = getDistance();
    //Serial.println("Distance: " + String(distance) + " cm");
    
   if (isDeviceEnabled("distance")) {
    Serial.println("Distance sensor is enabled");

    float distance = getDistance();
    Serial.println("Distance: " + String(distance) + " cm");

    if (distance <= distanceThreshold && isDeviceEnabled("buzzer")) {
      playAlertTone();
    }
  }

  if (isDeviceEnabled("infrared")) {
    checkMotion();
    
    if (isDeviceEnabled("buzzer")) {
      playAlertTone();
    }
  }

  delay(1000);
}

void initRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Getting configuration for the " + String(serialNumber));
    HTTPClient http;
    String url = String(serverName) + String(configEndpoint) + "?serialNumber=" + String(serialNumber);
    Serial.println("Requesting URL: " + url);

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      if (httpResponseCode == 200) {
        isLegalDevice = true;
        String response = http.getString();
        Serial.println("Retrieved configuration: " + response);

        parseConfigurationResponse(response);
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

void parseConfigurationResponse(String response) {
  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, response);
  if (error) {
    Serial.print("JSON deserialization failed: ");
    Serial.println(error.c_str());
    return;
  }

  startTime = doc["startTime"].as<String>();
  endTime = doc["endTime"].as<String>();
  distanceThreshold = doc["distanceThreshold"].as<int>();
  
  JsonArray enabledDevicesR = doc["enabledDevices"];
  for (String device : enabledDevicesR) {
    enabledDevices[enabledDevicesCount] = device;
    enabledDevicesCount++;
  }

  Serial.println("Parsed Configuration:");
  Serial.println("Start Time: " + endTime);
  Serial.println("End Time: " + endTime);
  Serial.println("Distance threshold: " + distanceThreshold);
  
  Serial.println("Enabled Devices:");
  for (int i = 0; i < enabledDevicesCount; i++) {
    Serial.println(enabledDevices[i] + "  ");
  }
}

bool isDeviceEnabled(String deviceName) {
  for (int i = 0; i < enabledDevicesCount; i++) {
    if (enabledDevices[i] == deviceName) {
      return true;
    }
  }
  return false;
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float distance = duration / 58.0;

  return distance;
}

void checkMotion(){
  int currentState = digitalRead(pirPin);

  if (currentState == HIGH) {
      Serial.println("Motion detected");
  } else {
    Serial.println("No motion detected");
  }
}

void playAlertTone(){
  playTone(1000, 200);  
  delay(200);          
  
  playTone(250, 200); 
  delay(200);
}

void playTone(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);  
  delay(duration + 20);                   
}
