// Pin analog di A0, ini untuk power
#define PIN_YL69 D6
 
unsigned int getAnalog(){
  const int sampling = 100;
  unsigned int analog = 0;
  for (int i = 0; i < sampling; i++){
      digitalWrite(PIN_YL69, HIGH);
      int nilaiSensor = analogRead(A0);
      digitalWrite(PIN_YL69, LOW);
      analog += nilaiSensor;
  }
  analog = analog / sampling;
  return analog;
}
 
uint8_t getSoil(){
  // Batas atas
  const int val_max = 870;
  // Batas bawah
  const int val_min = 320;
  const int sampling = 100;
   
  unsigned long analog = 0;
  for (int i = 0; i < sampling; i++){
      digitalWrite(PIN_YL69, HIGH);
      int nilaiSensor = analogRead(A0);
      digitalWrite(PIN_YL69, LOW);
      analog += nilaiSensor;
  }
  analog = analog / sampling;
  if(analog > val_max){
      analog = val_max;
  }
  else if(analog < val_min){
      analog = val_min;
  }
  uint8_t persen = map(analog,val_max,val_min,0,100);
  return persen;
}
 
void setup(){
  Serial.begin(115200);
  pinMode(PIN_YL69, OUTPUT);
  digitalWrite(PIN_YL69, LOW);
}
 
void loop(){
  Serial.print("Data: ");
  Serial.print(getAnalog());
  Serial.println(" ");
  Serial.print("Kelembaban Tanah: ");
  Serial.print(getSoil());
  Serial.println('%');
  delay(1000);
}
