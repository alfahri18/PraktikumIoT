#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Universitas Mulawarman";
const char* password = "";
const int BUZZER_PIN = D2;

#define DHTPIN D4 // pin digital sensor DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80); // Port untuk web server

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(BUZZER_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  Serial.println(WiFi.localIP());
  dht.begin(); // Memulai sensor DHT11

  server.on("/", handleRoot); // Halaman utama
  server.on("/temperature", handleTemperature); // Halaman untuk menampilkan suhu
  server.on("/humidity", handleHumidity); // Halaman untuk menampilkan kelembaban
  server.begin(); // Memulai web server
}

void loop() {
  server.handleClient(); // Menangani permintaan dari klien
}


void handleRoot() {
  server.send(200, "text/html", "<html><body><h1>Sensor DHT11 Monitoring</h1><p><a href=\"/temperature\">Lihat Suhu</a></p><p><a href=\"/humidity\">Lihat Kelembaban</a></p></body></html>"); // Menampilkan halaman utama
}

void handleTemperature() {
  float temp = dht.readTemperature(); // Membaca nilai suhu
  String content = "<html><body><h1>Temperature</h1><p>" + String(temp) + " &#8451;</p><p><a href=\"/\">Kembali ke halaman utama</a></p></body></html>"; // Menampilkan nilai suhu
  server.send(200, "text/html", content);
    if (temp > 29 ) {
    tone(BUZZER_PIN, 100);
  } else {
    noTone(BUZZER_PIN);
  }
}



void handleHumidity() {
  float hum = dht.readHumidity(); // Membaca nilai kelembaban
  String content = "<html><body><h1>Humidity</h1><p>" + String(hum) + " %</p><p><a href=\"/\">Kembali ke halaman utama</a></p></body></html>"; // Menampilkan nilai kelembaban
  server.send(200, "text/html", content);
}
