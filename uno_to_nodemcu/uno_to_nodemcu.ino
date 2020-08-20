
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2 
DHT dht(DHTPIN, DHTTYPE);
int temp,humi;
String str;
void setup(){
 dht.begin();
 Serial.begin(115200);
 Serial.begin(115200);
 Serial.print(char(169)); // Copyright Symbol
 Serial.println(" Myengineeringstuffs.com");
 delay(2000);
}
void loop()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  Serial.print("H: ");
  Serial.print(humi); 
  Serial.print("% ");
  Serial.print(" T: ");
  Serial.print(temp); 
  Serial.print(char(176));
  Serial.println("C");
  str =String('H')+String(humi)+String('T')+String(temp);
  Serial.println(str);
  delay(5000);
}


