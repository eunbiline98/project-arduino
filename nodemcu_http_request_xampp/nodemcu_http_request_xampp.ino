#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
 
/* Set these to your desired credentials. */
const char *ssid = "Robotic_Garage";  //ENTER YOUR WIFI SETTINGS
const char *password = "robotic77";

//Web/Server address to read/write from 
const char *host = "192.168.1.9";   //https://circuits4you.com website or IP address of server
 
//=======================================================================
//                    Power on setup
//=======================================================================
 
void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  HTTPClient http;    
 
  String ADCData, station, postData;
  int adcvalue=analogRead(A0);  
  ADCData = String(adcvalue);  
  station = "A";
 
  //Post Data
  postData = "status=" + ADCData + "&station=" + station ;
  
  http.begin("http://192.168.1.9/ta/simpan.php?id=1&nilai="+ADCData);             
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    
 
  int httpCode = http.POST(postData);
  String payload = http.getString(); 
 
  Serial.println(httpCode);   
  Serial.println(payload);    
 
  http.end(); 
  
  delay(5000);  
}
