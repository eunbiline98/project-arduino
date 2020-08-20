int pbuttonPin = A1;// connect output to push button
int pbuttonPin1 = A2;// connect output to push button
int pbuttonPin2 = A3;// connect output to push button
int relayPin = 13;// Connected to relay (LED)

#define pin_cw 10
#define pin_cww 11

int sens_a = 0;
int sens_b = 0;
int hujan  = 0;
void setup() {
  // Robojax.com code and video tutorial for push button ON and OFF
  Serial.begin(9600);
  pinMode(pbuttonPin, INPUT_PULLUP); 
  pinMode(pbuttonPin1, INPUT_PULLUP);
  pinMode(pbuttonPin2, INPUT_PULLUP);
  pinMode(pin_cw,OUTPUT);
  pinMode(pin_cww,OUTPUT);
}

void loop() {
// Robojax.com code and video tutorial for push button ON and OFF
  int st = 0;
  sens_a = digitalRead(pbuttonPin);// read the push button value
  sens_b = digitalRead(pbuttonPin1);
  hujan = digitalRead(pbuttonPin2);

 if(st==0){
   if(sens_a == HIGH && hujan == LOW){
    Serial.println("motor down");
    digitalWrite(pin_cww,HIGH);
    digitalWrite(pin_cw,LOW);
  }
  if(sens_a == LOW && hujan == LOW){
    Serial.println("motor stop down");
    digitalWrite(pin_cww,LOW);
    digitalWrite(pin_cw,LOW);
  }
  st = 1;
}
if(st==1){
   if(sens_b == LOW && hujan == HIGH){
    Serial.println("motor stop up");
    digitalWrite(pin_cww,LOW);
    digitalWrite(pin_cw,LOW);
    
  }
  if(sens_b == HIGH && hujan == HIGH){
    Serial.println("motor up");
    digitalWrite(pin_cww,LOW);
    digitalWrite(pin_cw,HIGH);
  }
  st = 0;
}
}
