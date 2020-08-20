void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("CLEARDATA");
Serial.println("LABEL,Time,Started Time, Register value");
Serial.print("RESETTIMER");
}

void loop() {
  // put your main code here, to run repeatedly:
int in = analogRead(A0);
Serial.print("DATA,TIME,TIMER,");
Serial.println(in);
delay(1000);
}
