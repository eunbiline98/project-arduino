
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
ESP8266WebServer server(80);
 
const char* ssid = "Robotic_Garage";
const char* password = "robotic77";
 
void setup() {
  // Setting Mode pin 
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
 
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi terhubung ");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
// Kode bagian Server
  server.on("/setpin", HTTP_POST, setPin);
  server.begin();
  Serial.println("Server berjalan...");
}
 
void loop() {
  server.handleClient();
}
 
// Fungsi untuk menangani request /setpin
void setPin() {
  if (server.hasArg("pin") && server.hasArg("state")) {
    int statusPin = server.arg("state").toInt();
    // Konversi nama pin ke alamat pin
    String namaPin = server.arg("pin");
    static const uint8_t pinAddress[] = {16,  5,  4,  0,  2, 14, 12, 13, 15,  3,  1};
    
    digitalWrite(pinAddress[namaPin.substring(1).toInt()], statusPin);
    server.send(200, "text/plain", "Perintah sudah dilakukan");
  }
  else {
    server.send(200, "text/plain", "Format perintah salah / tidak ada");
  }
}
