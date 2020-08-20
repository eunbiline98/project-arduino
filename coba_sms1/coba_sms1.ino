#include <SoftwareSerial.h>
#define trigPin 5
#define echoPin 6

//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(8,9); //SIM900 Tx & Rx is connected to Arduino #7 & #8

String incoming_char;
String message = "";
String air = "";
int duration;
int kondisi=0;
long distance;
int relay = 7;
int sms_air;
int airMin = 100;
int airMax = 10;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay, OUTPUT);
  
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"08973234445\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("monitoring kolam air ON"); //text content
  updateSerial();
  mySerial.write(26);
}

void loop()
{
//  delMessage();
  receivedMessage();
   if(incoming_char.indexOf("Status")>0 || incoming_char.indexOf("status")>0){
   cek_Air();
   sendCek();
  }

  cek_Air();
  if(distance <= airMax){
     kondisi = 0;
      sendStatus();
      digitalWrite(relay, LOW);
    }
    if(distance >= airMax){
     kondisi = 0;
      //sendAman();
      digitalWrite(relay,HIGH);
    }
}

void cek_Air(){
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(50);
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   distance = duration*0.034/2;
   Serial.print("distance : ");
   Serial.println(distance);
   air = distance;
//    air = String(sms_air);
  }

void sendStatus(){
     message = "ketinggian air :" + String(distance)+" Cm" +" kolam air penuh";
     sendMessage(message);
     delMessage();
  }
  void sendCek(){
     message = "ketinggian air :" + String(distance)+" Cm";
     sendMessage(message);
     delMessage();
  }
    void sendAman(){
     message = "ketinggian air :" + String(distance)+" Cm"+" kolam air Aman";
     sendMessage(message);
     delMessage();
  }

void perintah(){
//   if(incoming_char.indexOf("relay on")>0 || incoming_char.indexOf("Relay on")){
//      message = "Relay on, pompa air telah menyala";
//      sendMessage(message);
//      digitalWrite(relay, HIGH);
//    }
//  else if(incoming_char.indexOf("relay Off")>0 || incoming_char.indexOf("Relay off")>0){
//      message = "Relay off, pompa air telah mati";
//      sendMessage(message);
//      digitalWrite(relay, LOW);
//    }


  }

void receivedMessage(){
    if(mySerial.available() > 0)
    {
      incoming_char=mySerial.readString();
        Serial.print(incoming_char);
        delay(10);
    }
  }

  void sendMessage(String message){
   mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"08973234445\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print(message); //text content
  updateSerial();
  mySerial.write(26);
  }

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void delMessage() {  // Delete All messages
  //mySerial.println( "AT+CMGF=1" ); // Did not help
 //delay(1000);                                        //Did not help
  mySerial.print("AT+CMGDA=");
  mySerial.println("DEL ALL");
  delay(500);
 Serial.println( "All Messages Deleted" );
} 
