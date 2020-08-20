#include <SoftwareSerial.h>
SoftwareSerial s(D2,D3);
#include <ArduinoJson.h>

#define relay D4


 
void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  s.begin(9600);

  pinMode(relay,OUTPUT);
  while (!Serial) continue;
 
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("Data From Arduino To Nodemcu Success...");
  root.prettyPrintTo(Serial);
  Serial.print("Data 1 ");
  Serial.println("");
  int data1=root["data1"];
  Serial.print(data1);
  Serial.print("   Data 2 ");
  int data2=root["data2"];
  Serial.print(data2);
  Serial.println("");
   if(data1 >= 300){
    digitalWrite(relay,HIGH);
    Serial.println("Relay On");
  }
  else{
    digitalWrite(relay,LOW);
    Serial.println("Relay Off");
  }
  Serial.println("---------------------xxxxx--------------------");
  delay(1500);
}
