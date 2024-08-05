#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "HUAWEI Y7a";           // Replace with your network SSID
const char* password = "12345678";   // Replace with your network password

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for the connection to establish
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/ecg", handleECG);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>ECG Monitor</title>";
  html += "<script>";
  html += "function fetchData() {";
  html += "  fetch('/ecg').then(response => response.text()).then(data => {";
  html += "    document.getElementById('ecgData').innerText = data;";
  html += "  });";
  html += "}";
  html += "setInterval(fetchData, 1000);"; // Fetch data every second
  html += "</script>";
  html += "</head>";
  html += "<body><h1>ECG Data</h1>";
  html += "<p id='ecgData'>Loading...</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleECG() {
  int ecgValue = analogRead(A0);
  String ecgData = String(ecgValue);
  server.send(200, "text/plain", ecgData);
}
