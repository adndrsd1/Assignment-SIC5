#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Define WiFi credentials
const char* ssid = "name_wifi";
const char* password = "password_wifi";

// Define server URL
const char* serverName = "http://ip_address_server:5000/api/data";

// Define pin for MQ sensor
#define MQPIN 34

void setup() {
  Serial.begin(115200);
  Serial.println("MQ sensor initialization");
  pinMode(MQPIN, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  // Wait a few seconds between readings
  delay(2000);

  // Read analog value from MQ sensor
  int sensorValue = analogRead(MQPIN);
  float voltage = sensorValue * (3.3 / 4095.0); // Convert to voltage assuming 3.3V reference and 12-bit ADC

  // Display the result
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("\tVoltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Send data if WiFi is connected
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);  // Specify the URL

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    // Create JSON object
    StaticJsonDocument<200> doc;
    doc["name"] = "Adinda Rizki";
    doc["sensorValue"] = sensorValue;
    doc["voltage"] = voltage;

    String requestBody;
    serializeJson(doc, requestBody);

    // Send HTTP POST request
    int httpResponseCode = http.POST(requestBody);
    
    if (httpResponseCode > 0) {
      String response = http.getString();  // Get the response to the request
      Serial.println(httpResponseCode);    // Print return code
      Serial.println(response);            // Print request answer
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End the HTTP connection
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
