#include <SoftwareSerial.h>
SoftwareSerial SIM900A(8,9);
#define trigPin 5
#define echoPin 6
#define pompa 7

String incoming_char;

void setup() {
  SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  delay(100);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pompa, OUTPUT);

  Serial.println ("SIM900A Mengirim SMS");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+628973234445\"\r"); // Replace with your mobile number
  delay(1000);
  Serial.println ("Set SMS Content");
  SIM900A.println("Monitoring kolam ON");// The SMS text you want to send
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println (" ->SMS Selesai dikirim");
  SIM900A.write(26);
}

void loop() {
 
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58.2;

  receivedMessage();
  if(incoming_char.indexOf("Lapor")>0 || incoming_char.indexOf("lapor")>0){
   sendStatus();
  }
  
  if (distance >= 50){
    Serial.println("Air Kolam Kosong");
    //SendAirKosong();
  }
    else if (distance >= 25 && distance <= 10){
    Serial.println("Air Kolam Aman");
    //SendAirAman();
  }
    else if (distance <= 10){
    Serial.println("Air Kolam Penuh");
    SendAirPenuh();
  }
 
    Serial.print(distance);
    Serial.println(" cm");
 
  delay(500);
}
void receivedMessage(){
    if(SIM900A.available() > 0)
    {
      incoming_char=SIM900A.readString();
        Serial.print(incoming_char);
        delay(10);
    }
  }
void sendStatus(){
    Serial.println ("SIM900A Mengirim SMS");
    SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    Serial.println ("Set SMS Number");
    SIM900A.println("AT+CMGS=\"+628973234445\"\r"); // Replace with your mobile number
    delay(1000);
    Serial.println ("Set SMS Content");
    //SIM900A.println("ketinggian air :");
    //SIM900A.println(distance);
    //SIM900A.println("C ");// The SMS text you want to send
    delay(100);
    Serial.println ("Finish");
    SIM900A.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    Serial.println (" ->SMS Selesai dikirim");
  }
void SendAirKosong(){
   Serial.println ("SIM900A Mengirim SMS");
    SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    Serial.println ("Set SMS Number");
    SIM900A.println("AT+CMGS=\"+628973234445\"\r"); // Replace with your mobile number
    delay(1000);
    Serial.println ("Set SMS Content");
    SIM900A.println("Air Kolam Kosong");// The SMS text you want to send
    delay(100);
    Serial.println ("Finish");
    SIM900A.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    Serial.println (" ->SMS Selesai dikirim");
}
void SendAirAman(){
   Serial.println ("SIM900A Mengirim SMS");
    SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    Serial.println ("Set SMS Number");
    SIM900A.println("AT+CMGS=\"+628973234445\"\r"); // Replace with your mobile number
    delay(1000);
    Serial.println ("Set SMS Content");
    SIM900A.println("Air Kolam Aman");// The SMS text you want to send
    delay(100);
    Serial.println ("Finish");
    SIM900A.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    Serial.println (" ->SMS Selesai dikirim");
}
void SendAirPenuh(){
   Serial.println ("SIM900A Mengirim SMS");
    SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    Serial.println ("Set SMS Number");
    SIM900A.println("AT+CMGS=\"+628973234445\"\r"); // Replace with your mobile number
    delay(1000);
    Serial.println ("Set SMS Content");
    SIM900A.println("Air Kolam Penuh");// The SMS text you want to send
    delay(100);
    Serial.println ("Finish");
    SIM900A.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    Serial.println (" ->SMS Selesai dikirim");
}
