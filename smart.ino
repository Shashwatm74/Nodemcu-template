#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

WiFiClient wifiClient;

// WiFi credentials for hotspot (replace with desired name and password)
const char* ssid = "MyServoControl";
const char* password = "12345678";

// Ultrasonic sensor setup (sensor 1)
const int trigPin1 = D5;
const int echoPin1 = D6;

// Ultrasonic sensor setup (sensor 2)
const int trigPin2 = D7; // Change pins as needed for sensor 2
const int echoPin2 = D8; // Change pins as needed for sensor 2

unsigned long currentTime = millis();
unsigned long lastTime = currentTime;
long duration[2]; // Array to store durations for both sensors
int distance[2]; // Array to store distances for both sensors

ESP8266WebServer server(80);

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5);

// Define fixed values for fallback (replace with your desired location)
const float defaultLatitude = 12.8396; // Example latitude (replace with yours)
const float defaultLongitude =80.1550; // Example longitude (replace with yours)

float Latitude, Longitude;
int year, month, date, hour, minute, second;
String DateString, TimeString, LatitudeString, LongitudeString;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Configure WiFi hotspot
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.print("Connecting to hotspot... ");
  Serial.println(ssid);

  // Print hotspot IP address
  IPAddress ip = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(ip);

  // Pin configuration for ultrasonic sensors and GPS
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  SerialGPS.begin(9600); // Ensure correct baud rate for GPS module

  // Define server routes
  server.on("/", handleDistance);
  // server.on("/gps", handleGPS); // New route for GPS data
  server.begin();
}

void loop() {
  server.handleClient(); // Handle incoming client requests

  // Read GPS data periodically (e.g., every second)
  if (millis() - lastTime >= 2000) { // Add a delay of 2 seconds
    lastTime = millis();

    if (gps.location.isUpdated()) {
      if (gps.location.isValid()) {
        // Process and format valid GPS data
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude, 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude, 6);
        // ... (similar logic for date and time parsing)
        Serial.println("Valid GPS data received!"); // Print confirmation in serial monitor
      } else {
        // Use pre-defined values if no valid GPS data
        Latitude = defaultLatitude;
        LatitudeString = String(Latitude, 6);
        Longitude = defaultLongitude;
        LongitudeString = String(Longitude, 6);
      }
    } else {
      // Use pre-defined values if no new data
      Latitude = defaultLatitude;
      LatitudeString = String(Latitude, 6);
      Longitude = defaultLongitude;
      LongitudeString = String(Longitude, 6);
    }
  }
}

// Handle distance measurement and display on webpage
void handleDistance() {
  // Measure distance for both sensors
  currentTime = millis();
  if (currentTime - lastTime >= 100) {
    lastTime = currentTime;
    // Sensor 1 measurement
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration[0] = pulseIn(echoPin1, HIGH);
    distance[0] = duration[0] * 0.034 / 2; // cm (adjust speed of sound if needed)


    // Sensor 2 measurement
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration[1] = pulseIn(echoPin2, HIGH);
    distance[1] = duration[1] * 0.034 / 2; // cm (adjust speed of sound if needed)

String response = "<!DOCTYPE html><html><head><title>Sensor Data</title></head><body><h1>Sensor Readings</h1>";
response += "<p>Distance (Sensor 1): ";
response += String(distance[0]);
response += " cm</p>";
response += "<p>Distance (Sensor 2): ";
response += String(distance[1]);
response += " cm</p>";
// Add GPS data
response += "<p>Latitude: ";
response += LatitudeString;
response += "</p>";
response += "<p>Longitude: ";
response += LongitudeString;
response += "</p>";
// ... (other sensor data or elements)
response += "</body></html>";
  // Send webpage content to client
  server.send(200, "text/html", response);
}
}
// Handle GPS data request and send formatted data
// void handleGPS() {
//   if (gps.location.isValid()) {
//     // Prepare JSON response with GPS data
//       String response = "<!DOCTYPE html><html><head><title>GPS</title></head><body><h1>GPS Readings</h1>";
//   response += "<p>Latitue: ";
//   response += LatitudeString;
//   response += "<p>Longitude: ";
//   response += LongitudeString;
//   response += "</body></html>";

//   // Send webpage content to client
//   server.send(200, "text/html", response);
//   } else {
//       String response = "<!DOCTYPE html><html><head><title>GPS</title></head><body><h1>GPS Readings</h1>";
//   response += "<p>Latitue: ";
//   response += LatitudeString;
//   response += "<p>Longitude: ";
//   response += LongitudeString;
//   response += "</body></html>";
//     // Send error message if no valid GPS data
//     // server.send(500, "text/plain", "No valid GPS data available.");
//     server.send(500, "text/html", response);

//   }
// }

