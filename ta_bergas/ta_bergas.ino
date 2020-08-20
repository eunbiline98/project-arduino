#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>
SoftwareSerial s(D2,D3);
#include <ArduinoJson.h>

const char *ssid = "Robotic_Garage";  
const char *password = "robotic77";
 
const char *host = "192.168.1.9"; 
 
void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  s.begin(115200);
   WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  while (!Serial) continue;
 
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.print("Data 1 ");
  Serial.println("");
  int data1=root["data1"];
  Serial.print(data1);
  Serial.println("---------------------xxxxx--------------------");

  HTTPClient http;
//  int data1=root["data1"];
  String d = String (data1);
  //String postData = ;
  http.begin("http://192.168.1.9/ta/simpan.php?id=1&nilai="+d);              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(d);   
  String payload = http.getString();    
 
  Serial.println(httpCode);   
  Serial.println(payload);    
 
  http.end();  
  
  delay(2000); 
}
