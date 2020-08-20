/* --- www.nyebarilmu.com --- */
 
#define waktutunda 5 //untuk 5 detik
 
const int ledPin = 13; //Menggunakan led built in Arduino
const int SensorPir = 2; //menggunakan pin ke-2 arduino
 
unsigned long now = millis();
unsigned long TriggerAkhir = 0;
boolean waktumulai = false;
 
void setup() {
  Serial.begin(115200); //baut komunikasi serial monitor
  
  pinMode(SensorPir, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SensorPir), deteksigerakan, RISING);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}
 
void loop() {
  now = millis();
  if(waktumulai && (now - TriggerAkhir > (waktutunda*1000))) {
    Serial.println("Tidak ada gerakan!");
    digitalWrite(ledPin, LOW);
    waktumulai = false;
  }
}
 
void deteksigerakan() {
  Serial.println("GERAKAN TERDETEKSI");
  digitalWrite(ledPin, HIGH);
  waktumulai = true;
  TriggerAkhir = millis();
}
 
