#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

const char* ssid = "";
const char* password = "";

const int echoPin = 2;
const int trigPin = 4;
const int pirPin = 5;
const int buzzerPin = 22;

const char* serverName = "http://192.168.1.102:3000";
const char* configEndpoint = "/api/configuration";
const char* historyEndpoint = "/api/history";

const char* serialNumber = "51170740-312f-4c81-bc33-997c220cba83";
bool isLegalDevice = false;

String startTime;
String endTime;
String userId;
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

  configTime(0, 0, "pool.ntp.org");
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  timeinfo.tm_hour += 2; 
  if (timeinfo.tm_hour >= 24) { 
    timeinfo.tm_hour -= 24;
  }

  int currentHour = timeinfo.tm_hour;
  int currentMinute = timeinfo.tm_min;
  int currentTimeInMinutes = currentHour * 60 + currentMinute;

  int startTimeInMinutes = getTimeInMinutes(startTime);
  int endTimeInMinutes = getTimeInMinutes(endTime);

  //Serial.println("Current time: "+ String(currentTimeInMinutes));

  if (currentTimeInMinutes >= startTimeInMinutes && currentTimeInMinutes <= endTimeInMinutes) {
    if (isDeviceEnabled("distance")) {
      float distance = getDistance();
      Serial.println("Distance: " + String(distance) + " cm");
              
      if (distance <= distanceThreshold) {   
        if (isDeviceEnabled("buzzer")) {   
          playAlertTone();
          sendHistoryData("Buzzer", "Activated due to proximity threshold");
        }
        sendHistoryData("Distance", "Someone at the " + String(distance) + " cm" );
      }
    }

    if (isDeviceEnabled("infrared")) {
      if (checkMotion()) {       
        if (isDeviceEnabled("buzzer")) {   
          playAlertTone();  
          sendHistoryData("Buzzer", "Activated due to motion detection"); 
        }
        sendHistoryData("Infrared", "There is motion");
      }
    }
  } else {
    Serial.println("Outside of active time range.");
  } 
  
  delay(1000);
}

int getTimeInMinutes(String time) {
  int hour = time.substring(0, 2).toInt();
  int minute = time.substring(3, 5).toInt();
  return hour * 60 + minute;  
}

void initRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Getting configuration for the " + String(serialNumber));
    HTTPClient http;
    String url = String(serverName) + String(configEndpoint) + "?serialNumber=" + String(serialNumber);

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
  userId = doc["userId"].as<int>();
   
  JsonArray enabledDevicesR = doc["enabledDevices"];
  for (String device : enabledDevicesR) {
    enabledDevices[enabledDevicesCount] = device;
    enabledDevicesCount++;
  }

  Serial.println("Parsed Configuration:");
  Serial.println("Start Time: " + startTime);
  Serial.println("End Time: " + endTime);
  Serial.println("User id: " + userId);
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

bool checkMotion(){
  int currentState = digitalRead(pirPin);

  if (currentState == HIGH) {
      Serial.println("Motion detected");
      return true;
  } else {
    Serial.println("No motion detected");
    return false;
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

void sendHistoryData(String deviceName, String metadata) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    StaticJsonDocument<512> jsonDoc;
    jsonDoc["userId"] = userId;
    jsonDoc["deviceName"] = deviceName;
    jsonDoc["time"] = getCurrentTime();
    jsonDoc["metadata"] = metadata;
  
    String jsonData;
    serializeJson(jsonDoc, jsonData);  

    Serial.println("JSON Data to send: " + jsonData);
  
    String url = String(serverName) + String(historyEndpoint);
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("History sent successfully: " + response);
    } else {
      Serial.println("Error sending history: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Error: Not connected to Wi-Fi");
  }
}

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "unknown";
  }

  timeinfo.tm_hour += 2;
   if (timeinfo.tm_hour >= 24) {
    timeinfo.tm_hour -= 24;
  }
  
  char timeStr[20];
  strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
  return String(timeStr);
}
