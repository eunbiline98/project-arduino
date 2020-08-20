#define sens_pintu 12
int sensor;
#define led 13
void setup() {
  // put your setup code here, to run once:
pinMode(sens_pintu ,INPUT_PULLUP);
pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
sensor = digitalRead(sens_pintu);
if (sensor == 0){
  digitalWrite(led,LOW);
  
}
else {
  digitalWrite(led,HIGH);
}
}
