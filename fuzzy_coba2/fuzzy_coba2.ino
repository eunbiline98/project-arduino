#include <NewPing.h>
#define TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 50 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
 
float sensorUS;
int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;
int switchPin = 7;
float defuz, pwm;
 
//Sensor Suhu
float PinSuhu = A2;
int vin;
int PinUS = A1;
float sensorSuhu;
float temp;
 
//Rule Base
float suhu [3];
float jarak [3];
float rule [3][3];
float rule00, rule01, rule02;
float rule10, rule11, rule12;
float rule20, rule21, rule22;

void FuzzySuhu(){
  // untuk suhu dingin
  if (sensorSuhu <= 22.5)
  { suhu [0] = 1;}
  else if (sensorSuhu > 22.5 && sensorSuhu <= 25)
  {  suhu [0] = (25 - sensorSuhu)/(25 - 22.5); }
  else
  { suhu [0] = 0;}
 
  // untuk suhu hangat
  if (sensorSuhu <= 22.5)
  { suhu [1] = 0;}
  else if (sensorSuhu > 22.5 && sensorSuhu <= 25)
  { suhu [1] = (sensorSuhu-22.5)/(25-22.5);}
  else if (sensorSuhu > 25 && sensorSuhu <= 27.5)
  { suhu [1] = (27.5-sensorSuhu)/(27.5 - 25);}
  else
 { suhu [1] = 0;}
 
  // untuk suhu panas
  if (sensorSuhu <= 25)
  { suhu [2] = 0;}
  else if (sensorSuhu > 25 && sensorSuhu <= 27.5)
  { suhu [2] = (sensorSuhu-25)/(27.5-25);}
  else
  { suhu [2] = 1;}
}

//suhu
  vin = analogRead(PinSuhu); 
  sensorSuhu = (500*vin)/1024; 
  // ultrasonic
unsigned int uS = sonar.ping();  // Send ping, get ping time in microseconds (uS).
  sensorUS = (sonar.convert_cm(uS)); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
void setMotor(int speed, boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, ! reverse);
  digitalWrite(in2Pin, reverse);
}

void FuzzyJarak(){
 // untuk  kondisi dekat
  if (sensorUS <= 10)
  { jarak [0] = 1;}
  else if (sensorUS > 10 && sensorUS <= 20)
  {  jarak [0] = (20 - sensorUS)/(20 - 10); }
  else
  { jarak [0] = 0;}

  // untuk kondisi sedang
  if (sensorUS <= 10)
  { jarak [1] = 0;}
  else if (sensorUS > 10 && sensorUS <= 20)
  { jarak [1] = (sensorUS -10)/(20-10);}
  else if (sensorUS > 20 && sensorUS <= 40)
  { jarak [1] = (40-sensorUS)/(40 - 20);}
  else
  { jarak [1] = 0;}

  // untuk kondisi jauh
  if (sensorSuhu <= 20)
  { jarak [2] = 0;}
  else if (sensorUS > 20 && sensorUS <= 30)
  { jarak [2] = (sensorUS-20)/(30-20);}
  else if (sensorUS > 30)
  { jarak [2] = 1;}
}

void FuzzyJarak(){
 // untuk  kondisi dekat
  if (sensorUS <= 10)
  { jarak [0] = 1;}
  else if (sensorUS > 10 && sensorUS <= 20)
  {  jarak [0] = (20 - sensorUS)/(20 - 10); }
  else
  { jarak [0] = 0;}
 
  // untuk kondisi sedang
  if (sensorUS <= 10)
  { jarak [1] = 0;}
  else if (sensorUS > 10 && sensorUS <= 20)
  { jarak [1] = (sensorUS -10)/(20-10);}
  else if (sensorUS > 20 && sensorUS <= 40)
  { jarak [1] = (40-sensorUS)/(40 - 20);}
  else
  { jarak [1] = 0;}
 
  // untuk kondisi jauh
  if (sensorSuhu <= 20)
  { jarak [2] = 0;}
  else if (sensorUS > 20 && sensorUS <= 30)
  { jarak [2] = (sensorUS-20)/(30-20);}
  else if (sensorUS > 30)
  { jarak [2] = 1;}
}
