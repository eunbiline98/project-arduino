int AC_LOAD = 3;    // Output to Opto Triac pin
int dimming=128;  // Dimming level (0-255)  0 = ON, 255 = OFF 

float keluaran1;
float a,b,c;
float suhu, suhu_2;
float member_suhu;
float member_suhu_2;
float dingin, sejuk, normal, panas, sangatpanas;
float dingin2, sejuk2, normal2, panas2, sangatpanas_2;
float min1, min2, min3, min4, min5, min6, min7, min8, min9, min10, min11,min12, min13, min14, min15, min16, min17, min18, min19, min20, min21,min22, min23, min24, min25;
int PD=0, PL=65, NR=125, CP=175, SC=255;
float A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V,W, X, Y;
float output=0;
float crispy;
unsigned long interval=1000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.
void setup()
{
pinMode(AC_LOAD, OUTPUT);// Set AC Load pin as output //pwm
 attachInterrupt(0, zero_crosss_int, FALLING); //pin d2
Serial.begin(9600);
 
}
void zero_crosss_int() 
{
  
  int dimtime = (35*dimming);    // For 60Hz =>65   
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(10);         // triac On propogation delay
                                 //(for 60Hz use 8.33)
  digitalWrite(AC_LOAD, LOW);    // triac Off

}
void loop()

{
   unsigned long currentMillis = millis(); // grab current time
   if ((unsigned long)(currentMillis - previousMillis) >= interval) {
suhu = analogRead(A0) * 0.488;// konversi kedalam derajat celcius

suhu_2 = analogRead(A2) * 0.488;// konversi kedalam derajat celcius
keluaran1 = output;
dimming=keluaran1; 
crispy = map(dimming, 0,255,255,0);
membership();
defuzifikasi();
Output();
tampilan();
previousMillis = millis();
 }
}

void defuzifikasi()
{
A = min1*PD; B = min2*PD; C = min3*PL; D = min4*NR; E = min5*CP;
F = min6*PD; G = min7*PL; H = min8*PL; I = min9*NR; J = min10*CP;
K = min11*PL; L = min12*PL; M = min13*NR; N = min14*CP; O = min15*SC;
P = min16*NR; Q = min17*NR; R = min18*CP; S = min19*SC; T = min20*SC;
U = min21*CP; V = min22*CP; W = min23*SC; X = min24*SC; Y = min25*SC;
output = (A + B + C + D + E + F + G + H + I + J + K + L + M + N + O + P + Q + R + S + T + U + V + W + X + Y )/(min1 + min2 + min3 + min4 + min5+ min6 + min7 + min8 + min9 + min10 + min11 + min12 + min13 + min14 + min15 + min16 + min17 + min18 + min19 + min20 + min21 + min22 + min23 + min24 + min25);
}
void tampilan()
{

Serial.print("SUHU 1= ");
Serial.print(suhu);
Serial.print("*C");
Serial.print(" | ");
Serial.print("SUHU 2= ");
Serial.print(suhu_2);
Serial.print("*C");
Serial.print(" | ");
Serial.print(" Fuzzy output ");
Serial.print( output);
Serial.print(" |");
Serial.print(" Crisp output ");
Serial.print( crispy);
Serial.print(" |");
}
void membership()
{
//SENSOR suhu1
member_suhu = 0;
Sensor_1(a=0, b=30, c=35); //Buat Dingin
dingin = member_suhu;
Sensor_1(a=30, b=35, c=40); //Buat Sejuk
sejuk = member_suhu;
Sensor_1(a=35, b=40, c=45); //Buat Normal
normal = member_suhu;
Sensor_1(a=40, b=45, c=50); //Buat Panas
panas = member_suhu;
Sensor_1(a=45, b=50, c=55); //Buat Sangat Panas
sangatpanas = member_suhu;
//SENSOR suhu_2
member_suhu = 0;
Sensor_2(a=0, b=30, c=35); //Buat sangat sejuk2
dingin2 = member_suhu_2;
Sensor_2(a=30, b=35, c=40); //Buat sejuk2
sejuk2 = member_suhu_2;
Sensor_2(a=35, b=40, c=45); //Buat normal2
normal2 = member_suhu_2;
Sensor_2(a=40, b=45, c=50); //Buat panas2
panas2 = member_suhu_2;
Sensor_2(a=45, b=50, c=55); //Buat Sangat panas2
sangatpanas_2 = member_suhu_2;
min1 = min(dingin, dingin2);
min2 = min(dingin, sejuk2);
min3 = min(dingin, normal2);
min4 = min(dingin, panas2);
min5 = min(dingin, sangatpanas_2);
min6 = min(sejuk, dingin2);
min7 = min(sejuk, sejuk2);
min8 = min(sejuk, normal2);
min9 = min(sejuk, panas2);
min10 = min(sejuk, sangatpanas_2);
min11 = min(normal, dingin2);
min12 = min(normal, sejuk2);
min13 = min(normal, normal2);
min14 = min(normal, panas2);
min15 = min(normal, sangatpanas_2);
min16 = min(panas, dingin2);
min17 = min(panas, sejuk2);
min18 = min(panas, normal2);
min19 = min(panas, panas2);
min20 = min(panas, sangatpanas_2);
min21 = min(sangatpanas, dingin2);
min22 = min(sangatpanas, sejuk2);
min23 = min(sangatpanas, normal2);
min24 = min(sangatpanas, panas2);
min25 = min(sangatpanas, sangatpanas_2);
}
void Output ()
{

if (output < 32) { Serial.print("SANGAT TERANG");}
if (output >= 33 && output < 65) {Serial.print("TERANG");}
if (output >= 65 && output < 125) {Serial.print("AGAK TERANG");}
if (output >= 125 && output < 230) {Serial.print("REDUP");}
if (output >= 231 && output < 256) {Serial.print("PADAM");}
//if (output > 255) { output = 255; Serial.print("SANGAT CEPAT ");}
Serial.println();
}
//============ SENSOR SUHU ============
float Sensor_1(float a, float b, float c)
{
if ((suhu >= a) && (suhu < b))
{
member_suhu = (suhu - a) / (b - a);
}
if ((suhu >= b) && (suhu < c))
{
member_suhu = (c - suhu) / (c - b);
}
if ((suhu < 0) || (suhu > 70))
{
member_suhu = 1;
}
if ((suhu > c) || (suhu < a))
{
member_suhu = 0;
}
}
//============ SENSOR suhu_2 ============
float Sensor_2(float a, float b, float c)
{
if ((suhu_2 >= a) && (suhu_2 < b))
{
member_suhu_2 = (suhu_2 - a) / (b - a);
}
if ((suhu_2 >= b) && (suhu_2 < c))
{
member_suhu_2 = (c - suhu_2) / (c - b);
}
if ((suhu_2 < 0) || (suhu_2 > 70))
{
member_suhu_2 = 1;
}
if ((suhu_2 > c) || (suhu_2 < a))
{
member_suhu_2 = 0;
}
}
