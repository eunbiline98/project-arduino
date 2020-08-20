#define pin_cw 10
#define pin_cww 11
int sens_a = A1;
int sens_b = A2;

int pwm = 0;

#define RT0 10000   // Ω
#define B 3977      // K

#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ

//Variables
float RT, VR, ln, TX, T0, VRT;

void setup() {
  // put your setup code here, to run once:
pinMode(pin_cw,OUTPUT);
pinMode(pin_cww,OUTPUT);
pinMode(sens_a, INPUT_PULLUP);
pinMode(sens_b, INPUT_PULLUP);
Serial.begin(9600);
T0 = 25 + 273.15;

}

void loop() {
  // put your main code here, to run repeatedly:
  suhu();
  if (TX >= 35.00){
  digitalWrite(pin_cww,HIGH);
  digitalWrite(pin_cw,LOW);
  Serial.println("Jemuran Keluar");
  if(digitalRead(sens_a)==LOW){
  digitalWrite(pin_cw,LOW);
  digitalWrite(pin_cww,LOW);
  }
  }
  else{
  digitalWrite(pin_cww,LOW);
  digitalWrite(pin_cw,HIGH);
  Serial.println("Jemuran Keluar");
 if(digitalRead(sens_b)==LOW){
  digitalWrite(pin_cw,LOW);
  digitalWrite(pin_cww,LOW);
  }
}
}
void suhu(){
  VRT = analogRead(A0);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX = TX - 273.15;                 //Conversion to Celsius

  Serial.print("Temperature:");
  Serial.print("\t");
  Serial.print(TX);
  Serial.print("C\t\t");
  Serial.print(TX + 273.15);        //Conversion to Kelvin
  Serial.print("K\t\t");
  Serial.print((TX * 1.8) + 32);    //Conversion to Fahrenheit
  Serial.println("F");
  delay(500);

}
