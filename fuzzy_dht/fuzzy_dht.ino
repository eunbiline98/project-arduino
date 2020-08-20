/* COMFORT SOFT SENSOR

Short description: determine atmospheric comfort with fuzzy logic.

Hardware setup: Arduino UNO, DHT11

Projects Homepage: www.instructable.com

Copyrights by vonPongrac
Arduino IDE 1.6.3
*/
#include "DHT.h"

#define DHTPIN 2 // signal pin of DHT11 is connected to pin D2
#define DHTTYPE DHT22 // choosing a DHT11 software

float trimf(float x,float a, float b, float c); // defining trangular membrship function
float comfort(float T, float V); // definign function for calculating comfort

DHT dht(DHTPIN, DHTTYPE); // creating class dht from DHT library

void setup() {
  Serial.begin(9600); // start serial communication
  Serial.println("Comfort Meter");
  
  dht.begin(); // begin DHT class
}

void loop() {
  float h = dht.readHumidity(); // read relative humidity from DHT11
  float t = dht.readTemperature(); // read temperature from DHT11
  if (isnan(t) || isnan(h)) // if we cannot receive data from DHT11, signal error
  {
      Serial.println("Failed to read from DHT");
  }else
  {
    float comf = comfort(t,h); // calculate comfort
    
    // printing results
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    Serial.print("Comfort: "); 
    Serial.print(comf*100); // printing comfort in %
    Serial.println(" %");
  }
  delay(3000);
}

// Function for calculating aterniation to a membership function
  // x = our variable (measurment of temperature or relative humidity)
  // a = starting point of triangle
  // b = peak of triangle
  // c = ending point of triangle
float trimf(float x,float a, float b, float c){
  float f;
  if(x<=a)
     f=0;
  else if((a<=x)&&(x<=b)) 
     f=(x-a)/(b-a);
  else if((b<=x)&&(x<=c)) 
     f=(c-x)/(c-b);
  else if(c<=x) 
     f=0;
  return f; 
}

// Function for predicting atmospheric comfort from temperature and relative humidity
float comfort(float T, float V){
  float C1 = 1; // singleton, comfortable
  float C2 = 0.5; // singleton, uncomfortable
  float C3 = 0; // singleton, very uncomfortable
  if(T>35) T=35; // if temperature is above 35°C, set temperature to maksimum in our range
  // Fuzzy rules
  float w1 = min (trimf(V, 0, 50, 100), trimf(T, 10, 22.5, 35)); // comfortable
  float w2 = min (trimf(V, 0, 50, 100), trimf(T, 10, 10, 22.5)); // uncomfortable
  float w3 = min (trimf(V, 0, 50, 100), trimf(T, 22.5, 35, 35)); // uncomfortable
  float w4 = min (trimf(V, 0, 0, 50), trimf(T, 10, 22.5, 35)); // uncomfortable
  float w5 = min (trimf(V, 0, 0, 50), trimf(T, 10, 10, 22.5)); // very uncomfortable
  float w6 = min (trimf(V, 0, 0, 50), trimf(T, 22.5, 35, 35)); // very uncomfortable
  float w7 = min (trimf(V, 50, 100, 100), trimf(T, 10, 10, 22.5)); // very uncomfortable
  float w8 = min (trimf(V, 50, 100, 100), trimf(T, 10, 22.5, 35)); // uncomfortable
  float w9 = min (trimf(V, 50, 100, 100), trimf(T, 22.5, 35, 35)); // very uncomfortable
  // Defuzzyfication
  float z = (w1*C1 + w2*C2 + w3*C2 + w4*C2 + w5*C3 + w6*C3 + w7*C3 + w8*C2 + w9*C3)/(w1+w2+w3+w4+w5+w6+w7+w8+w9);
  return z;
}
