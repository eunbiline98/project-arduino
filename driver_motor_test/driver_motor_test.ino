#define pin_cw 10
#define pin_cww 11
void setup() {
  // put your setup code here, to run once:
pinMode(pin_cw,OUTPUT);
pinMode(pin_cww,OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.available()){
  int inByte = Serial.read();
  if(inByte == 'a')
  {digitalWrite(pin_cww,HIGH);
  digitalWrite(pin_cw,LOW);
  Serial.println("arah motor kanan");
  }
  
  if(inByte == 'b')
  {digitalWrite(pin_cw,HIGH);
  digitalWrite(pin_cww,LOW);
  Serial.println("arah motor kiri");
}

  if(inByte == 'c')
  {digitalWrite(pin_cw,LOW);
  digitalWrite(pin_cww,LOW);
  Serial.println("Stop");
}
  }
}
