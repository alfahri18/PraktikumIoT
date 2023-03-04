#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <toneAC.h>

const char* ssid = "Nama SSID WiFi";
const char* password = "Password WiFi";

#define DHTPIN D4 // pin digital sensor DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80); // Port untuk web server

const int buzzerPin = D5; // Pin untuk buzzer
int buzzerFrequency = 2000; // Frekuensi bunyi buzzer
int buzzerDuration = 1000; // Durasi bunyi buzzer dalam milidetik

void setup() {
  Serial.begin(9600);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Menampilkan alamat IP pada Serial Monitor
  dht.begin(); // Memulai sensor DHT11

  server.on("/", handleRoot); // Halaman utama
  server.on("/temperature", handleTemperature); // Halaman untuk menampilkan suhu
  server.on("/humidity", handleHumidity); // Halaman untuk menampilkan kelembaban
  server.begin(); // Memulai web server

  pinMode(buzzerPin, OUTPUT); // Mengatur pin buzzer sebagai OUTPUT
}

void loop() {
  server.handleClient(); // Menangani permintaan dari klien

  float temp = dht.readTemperature(); // Membaca nilai suhu
  if (temp > 28.0) { // Jika suhu melebihi 28 derajat Celsius
    toneAC(buzzerPin, buzzerFrequency, buzzerDuration); // Bunyi buzzer
  } else {
    noToneAC(buzzerPin); // Berhenti bunyi buzzer
  }
}

void handleRoot() {
  server.send(200, "text/html", "<html><body><h1>Sensor DHT11 Monitoring</h1><p><a href=\"/temperature\">Lihat Suhu</a></p><p><a href=\"/humidity\">Lihat Kelembaban</a></p></body></html>"); // Menampilkan halaman utama
}

void handleTemperature() {
  float temp = dht.readTemperature(); // Membaca nilai suhu
  String content = "<html><body><h1>Temperature</h1><p>" + String(temp) + " &#8451;</p><p><a href=\"/\">Kembali ke halaman utama</a></p></body></html>"; // Menampilkan nilai suhu
  server.send(200, "text/html", content);
}

void handleHumidity() {
  float hum = dht.readHumidity(); // Membaca nilai kelembaban
  String content = "<html><body><h1>Humidity</h1><p>" + String(hum) + " %</p><p><a href=\"/\">Kembali ke halaman utama</a></p></body></html>"; // Menampilkan nilai kelembaban
  server.send(200, "text/html", content);
}
