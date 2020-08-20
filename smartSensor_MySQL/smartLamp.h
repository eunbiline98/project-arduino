#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "PageIndex.h"

#define ON_Board_LED 2  
#define LIGHT D6 

const char* ssid = "Robotic_Garage";  //--> Your wifi SSID Name
const char* password = "robotic77"; //--> Your wifi Password

ESP8266WebServer server(80);  //--> Server on port 80

void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleLIGHT() {
  String LightState = "OFF";
  String t_state = server.arg("LIGHTState");
  Serial.print(t_state);
  if(t_state == "1")  {
    digitalWrite(LIGHT,HIGH); //LED ON
    LightState = "ON";
    Serial.println(" | LIGHT ON");
  }
  else  {
    digitalWrite(LIGHT,LOW); //LED OFF
    LightState = "OFF";  
    Serial.println(" | LIGHT OFF"); 
  }
  server.send(200, "text/plane", LightState); 
}

void setup(void){
  Serial.begin(115200);
  delay(500);
  
  WiFi.begin(ssid, password); 
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT);
  digitalWrite(ON_Board_LED, HIGH);

  pinMode(LIGHT,OUTPUT); 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);

  }
  digitalWrite(ON_Board_LED, HIGH); 
 
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  server.on("/", handleRoot); 
  server.on("/setLIGHT", handleLIGHT);  
  
  server.begin(); //--> Start server
  Serial.println("HTTP server started");
}
void loop(void){
  server.handleClient();
}
