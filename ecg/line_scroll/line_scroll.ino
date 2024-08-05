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
  html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";  // Load Chart.js library
  html += "<script>";
  html += "let ecgData = [];";
  html += "let labels = [];";
  html += "function fetchData() {";
  html += "  fetch('/ecg').then(response => response.text()).then(data => {";
  html += "    ecgData.push(parseInt(data));";
  html += "    labels.push('');";
  html += "    if (ecgData.length > 50) { ecgData.shift(); labels.shift(); }";  // Keep last 50 data points
  html += "    updateChart();";
  html += "  });";
  html += "}";
  html += "function updateChart() {";
  html += "  ecgChart.data.labels = labels;";
  html += "  ecgChart.data.datasets[0].data = ecgData;";
  html += "  ecgChart.update();";
  html += "}";
  html += "setInterval(fetchData, 100);";  // Fetch data every 100 milliseconds
  html += "</script>";
  html += "</head>";
  html += "<body><h1>ECG Data</h1>";
  html += "<canvas id='ecgChart' width='400' height='200'></canvas>";
  html += "<script>";
  html += "const ctx = document.getElementById('ecgChart').getContext('2d');";
  html += "const ecgChart = new Chart(ctx, {";
  html += "  type: 'line',";
  html += "  data: {";
  html += "    labels: [],";
  html += "    datasets: [{";
  html += "      label: 'ECG',";
  html += "      borderColor: 'rgba(255, 34, 7, 1)',";
  html += "      backgroundColor: 'rgba(255, 33, 30, 0.53)',";
  html += "      data: [],";
  html += "    }],";
  html += "  },";
  html += "  options: {";
  html += "    animation: false,";
  html += "    scales: {";
  html += "      x: {";
  html += "        type: 'category',";
  html += "        position: 'bottom'";
  html += "      },";
  html += "      y: {";
  html += "        beginAtZero: true";
  html += "      }";
  html += "    }";
  html += "  }";
  html += "});";
  html += "</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleECG() {
  int ecgValue = analogRead(A0);
  String ecgData = String(ecgValue);
  server.send(200, "text/plain", ecgData);
}
