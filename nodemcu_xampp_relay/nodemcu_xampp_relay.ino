#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char* ssid = "Robotic_Garage";
const char* password = "robotic77";
 HTTPClient http;

const char* host= "http://192.168.1.9/relay_nodemcu/Getstatus.php";
void setup() {
  pinMode(D4,OUTPUT);
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

delay(3000);
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {

  http.begin(host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode=http.GET();
  String payload=http.getString(); // get data from webhost continously
  Serial.print(payload);
  if(payload == "1")  // if data == 1 -> LED ON
  {
    digitalWrite(D4,HIGH);
    }
   else if (payload == "0") // if data == 0 -> LED OFF
   {
    digitalWrite(D4,LOW);
    }
  delay(500);
   http.end();

}
