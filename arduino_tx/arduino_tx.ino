#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(2,3);
 
void setup() {
  Serial.begin(9600);
  s.begin(9600);
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
  root["data1"] = analogRead(A0);
  root["data2"] = 1;
if(s.available()>0)
{
 root.printTo(s);
}
  Serial.println("Send Success");
}
