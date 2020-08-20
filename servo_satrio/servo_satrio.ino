//ultrasonic
int trig= D2;           
int echo= D1;             
long durasi, jarak; 
void setup() {
  // put your setup code here, to run once:
  //ultrasonic
  Serial.begin(9600);
  pinMode(trig, OUTPUT);  
  pinMode(echo, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(trig, LOW);
  delayMicroseconds(8);
  digitalWrite(trig, HIGH);
  delayMicroseconds(8);
  digitalWrite(trig, LOW);
  delayMicroseconds(8);
  
  durasi= pulseIn(echo, HIGH); 
  jarak= (durasi/2) / 29.1;     
  Serial.println(jarak);  
  delay(1000);

}
