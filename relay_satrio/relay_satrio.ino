#define relay D3
void setup() {
  // put your setup code here, to run once:
pinMode(relay,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(relay,HIGH);
delay(5000);
digitalWrite(relay,HIGH);
delay(5000);

}
