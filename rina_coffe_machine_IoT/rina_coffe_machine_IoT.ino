//firebase setup
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "coffeemachine-5b489.firebaseio.com"
#define FIREBASE_AUTH "3eZ39zipdZGwfKwcba3LKwZzCqm8F133ulj4Wnog"
#define WIFI_SSID "F.adelia"
#define WIFI_PASSWORD "adelia123."
FirebaseData firebaseData;

//Servo PinOut
#include <Servo.h>
Servo motorServo;
const int servoPin = D7;

//Attach LoadCell HX711
#include "HX711.h"
const int LOADCELL_DOUT_PIN = D5;
const int LOADCELL_SCK_PIN = D6;
HX711 scale;
float calibration_factor = 839.60;
float GRAM;
float totalCoffee;        //set total of Cofee (gr)  ->Firebase

//solenoid valve
#define valve D0

//flowsensor
#define flow_sensor  D3
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned int totalWater = 120;

void pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  //load cell setuup
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();

  //Attach Servo Motor
  motorServo.attach(servoPin);
  motorServo.write(0);

  //solenoid valve setup
  pinMode(valve, OUTPUT);

  //flowsensor setup
  pinMode(flow_sensor, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(flow_sensor), pulseCounter, FALLING);

  //WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // getCoffeeValueLimit
  String berat_kopi;
  Firebase.getString(firebaseData, "takar_kopi");
  berat_kopi = firebaseData.stringData();
  Serial.println(berat_kopi);

  if (berat_kopi == "20") {
    totalCoffee = 20.00;
    timbang_kopi();
  }
  if (berat_kopi == "25") {
    totalCoffee = 25.00;
    timbang_kopi();
  }
  if (berat_kopi == "30") {
    totalCoffee = 30.00;
    timbang_kopi();
  }
}

//Functiozn Declarations
void timbang_kopi() {
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 4;
  Serial.print("netto:");
  Serial.println(GRAM);
  Serial.print("netto kopi:");
  Serial.println(totalCoffee);

  while (GRAM <= totalCoffee) {
    motorServo.write(90);
    delay(100);
    Serial.println("Coffee Servo Open");
    GRAM = scale.get_units(), 4;
    Serial.print("netto:");
    Serial.println(GRAM);
  }
  motorServo.write(0);
  Serial.println("Coffee Servo Close");
  delay(1000);
  //  flow_water_control();
}

//Pour Water
void flow_water_control() {
  String statuss;
  Firebase.getString(firebaseData, "takar_kopi");
  statuss = firebaseData.stringData();

  if (statuss == "1") {
    digitalWrite(valve, HIGH);
    Serial.println("Solenoid Opened");
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      pulse1Sec = pulseCount;
      pulseCount = 0;
      flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
      previousMillis = millis();
      flowMilliLitres = (flowRate / 60) * 1000;
      totalMilliLitres += flowMilliLitres;
      // Print the flow rate for this second in litres / minute
      Serial.print("Flow rate: ");
      Serial.print(int(flowRate));  // Print the integer part of the variable
      Serial.print("L/min");
      Serial.print("\t");       // Print tab space
      // Print the cumulative total of litres flowed since starting
      Serial.print("Output Liquid Quantity: ");
      Serial.print(totalMilliLitres);
      Serial.print("mL / ");
      Serial.print(totalMilliLitres / 1000);
      Serial.println("L");
    }
    if (totalMilliLitres >= totalWater) {
      digitalWrite(valve, LOW);
      Serial.println("Coffee Ready to Serve");
      // Reset the pulse counter
      pulseCount = 0;
      totalMilliLitres = 0;
    }
  }
  if (statuss == 0) {
    digitalWrite(valve, LOW);
    Serial.println("Solenoid Closed");
  }
}
