#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
//#include "smartLamp.h"

int Led_OnBoard = 2;                   

const char* ssid = "Pkm_asia";               
const char* password = "sistemkomputer";

const char *host = "192.168.43.106";

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  pinMode(Led_OnBoard, OUTPUT);       
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);      
  delay(1000);
  WiFi.mode(WIFI_STA);        
  
  WiFi.begin(ssid, password);  
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(Led_OnBoard, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(Led_OnBoard, HIGH);
    delay(250);
  }

  digitalWrite(Led_OnBoard, HIGH);

  Serial.println("");
  Serial.println("Connected to Network/SSID");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
}

void loop() {
  // put your main code here, to run repeatedly:
  HTTPClient http;    
 
  String LdrValueSend, postData;
  int ldrvalue=analogRead(A0);  
  LdrValueSend = String(ldrvalue);   
 
  //Post Data
  postData = "ldrvalue=" + LdrValueSend;
  
  http.begin("http://192.168.43.106/iot_php/InsertDB.php");             
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");   
 
  int httpCode = http.POST(postData);   
  String payload = http.getString();    

  //Serial.println("LDR Value=" + ldrvalue);
  Serial.println(httpCode);   
  Serial.println(payload);    
  Serial.println("LDR Value=" + LdrValueSend);
  
  http.end();  

  delay(4000);  
  digitalWrite(Led_OnBoard, LOW);
  delay(1000);
  digitalWrite(Led_OnBoard, HIGH);
}
