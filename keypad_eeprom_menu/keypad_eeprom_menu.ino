#include <Keypad.h>
#include <Wire.h> 
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <EEPROM.h>
unsigned char key,menu,urut;
unsigned char Pass1,Pass2,Pass3,Pass4,Pass5,Pass6,i;
unsigned char Pass_kpd1,Pass_kpd2,Pass_kpd3,Pass_kpd4,Pass_kpd5,Pass_kpd6;
unsigned char cek,cnt;
unsigned char fkpd;
char Pass[7],Pass_kpd[7];
char s,t;

const byte ROWS=4;
const byte COLS=4;
char keys[ROWS][COLS]={
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS]={A0,A1,A2,A3};
byte colPins[COLS]={10,11,12,13};
Keypad keypad = Keypad(makeKeymap(keys),rowPins, colPins, ROWS, COLS);
void setup()
{
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
digitalWrite(8,0);
digitalWrite(9,0);
lcd.begin();
lcd.print("     Welcome  ");
lcd.setCursor(0,1);
lcd.print("AtmDuino Product");
delay(2000);
//---------------------cek alamat 0
cek=EEPROM.read(0);
if(cek==0xFF){
cek=1;
EEPROM.write(0,'1');
Pass1='1'; EEPROM.write(1,'1');
Pass2='1'; EEPROM.write(2,'1');
Pass3='1'; EEPROM.write(3,'1');
Pass4='1'; EEPROM.write(4,'1');
Pass5='1'; EEPROM.write(5,'1');
Pass6='1'; EEPROM.write(6,'1');
}
else{
Pass1=EEPROM.read(1);
Pass2=EEPROM.read(2);
Pass3=EEPROM.read(3);
Pass4=EEPROM.read(4);
Pass5=EEPROM.read(5);
Pass6=EEPROM.read(6);
}
fkpd=0;
delay(200);
}
void loop()
{
//----------------------------- display time
digitalWrite(8,1);
lcd.clear();
lcd.print("Silakan kunci...");
lcd.setCursor(0,1);
lcd.print("Tekan #");
fkpd=2;
goKeypad();
lcd.clear();
lcd.print("Pintu Terkunci");
delay(3000);
pass:
lcd.clear();
lcd.print("Masukan Password");
lcd.setCursor(0,1);
fkpd=1;
menu=0;
goKeypad();
if(menu==4) goto pass;
}
//---------------------------- keypad
void goKeypad(){
do{
key=keypad.getKey();
if(key != NO_KEY){
if(key=='*' && fkpd==1){
lcd.clear();
lcd.print("Ganti Password");
lcd.setCursor(0,1);
lcd.print("Pass lama=");
menu=1;
delay(300);
}
else if(key=='#' && fkpd==2){
fkpd=0;
digitalWrite(8,0);
delay(250);
}
else if ((key=='C')&&(fkpd == 1)){ //C
if(menu==3){
lcd.clear();
lcd.print("Pass baru:");
menu=2;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
cnt=0;
}
else if(menu==2){
menu=4;
fkpd=0;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
cnt=0;
}
else if((menu==1)||(menu==0)){
menu=4;
fkpd=0;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
cnt=0;
}
delay(250);
}
else if (key=='D'&& menu==3){
lcd.clear();
lcd.print("Simpan Pass Baru..");
EEPROM.write(1,Pass_kpd[0]);
EEPROM.write(2,Pass_kpd[1]);
EEPROM.write(3,Pass_kpd[2]);
EEPROM.write(4,Pass_kpd[3]);
EEPROM.write(5,Pass_kpd[4]);
EEPROM.write(6,Pass_kpd[5]);
EEPROM.write(0,1);
Pass1 = Pass_kpd[0];
Pass2 = Pass_kpd[1];
Pass3 = Pass_kpd[2];
Pass4 = Pass_kpd[3];
Pass5 = Pass_kpd[4];
Pass6 = Pass_kpd[5];
fkpd = 0;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
delay(3000);
menu = 4;
}
else if(menu<2 && fkpd==1 && key!='A' && key!='B'
&& key!='C' && key!='D' && key!='*' && key!='#'){
cek_password();
}
else if(menu==2 && fkpd==1&& key!='A' && key!='B'
&& key!='C' && key!='D' && key!='*' && key!='#'){
entri_password();
}
}
}
while(fkpd);
}
void cek_password(){
Pass_kpd[cnt]=key;
cnt++;
lcd.print('*');
// lcd.print(char(key));
if((cnt==6)&&(menu==0)){
delay(500);
if((Pass1==Pass_kpd[0])&&(Pass2==Pass_kpd[1])&&(Pass3==Pass_kpd[2])&&(Pass4==Pass_kpd[3])&&(Pass5==Pass_kpd[4])&&(Pass6==Pass_kpd[5])){
lcd.clear();
lcd.print("Password Benar");
lcd.setCursor(0,1);
lcd.print("Silakan Masuk");
digitalWrite(8,1);
digitalWrite(9,1);
delay(200);
digitalWrite(9,0);
delay(200);
digitalWrite(9,1);
delay(200);
digitalWrite(9,0);
cnt=0;
fkpd=0;
s=0;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
delay(3000);
}
else {
lcd.clear();
lcd.print("Password Salah");
s++;
if(s < 3){
digitalWrite(9,1);
delay(2000);
digitalWrite(9,0);
lcd.clear();
lcd.print("Masukan Password");
lcd.setCursor(0,1);
cnt=0;
for(t=0;t<6;t++)
Pass_kpd[t]=' ';
}
else if (s==3){
lcd.clear();
lcd.print("Anda 3x Salah");
lcd.setCursor(0,1);
lcd.print("Tunggu 5 Menit");
for(i=0;i<75;i++){
digitalWrite(9,1);
delay(2000);
digitalWrite(9,0);
delay(2000);
}
lcd.clear();
lcd.print("Masukan Password");
lcd.setCursor(0,1);
cnt=0;
s=0;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
}
}
}
else if((cnt==6)&&(menu==1)){
if((Pass1==Pass_kpd[0])&&(Pass2==Pass_kpd[1])&&(Pass3==Pass_kpd[2])&&(Pass4==Pass_kpd[3])&&(Pass5==Pass_kpd[4])&&(Pass6==Pass_kpd[5])){
lcd.clear();
lcd.print("Password Benar");
lcd.setCursor(0,1);
delay(2000);
lcd.clear();
lcd.print("Pass baru:");
cnt=0;
menu=2;
s=0;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
}
else {
s++;
if(s < 3){
lcd.clear();
lcd.print("Password Salah");
cnt=0;
delay(2000);
lcd.clear();
lcd.print("Ganti Password");
lcd.setCursor(0,1);
lcd.print("Pass lama=");
for(t=0;t<6;t++)Pass_kpd[t]=' ';
}
else if (s==3){
lcd.clear();
lcd.print("Anda 3x Salah");
lcd.setCursor(0,1);
lcd.print("Tunggu 5 Menit");
for(i=0;i<75;i++){
digitalWrite(9,1);
delay(2000);
digitalWrite(9,0);
delay(2000);
}
lcd.clear();
lcd.print("Ganti Password");
lcd.setCursor(0,1);
lcd.print("Pass lama=");
cnt=0;
s=0;
for(t=0;t<6;t++)Pass_kpd[t]=' ';
}
}
}
}
void entri_password(){
lcd.print(char(key));
Pass_kpd[cnt]=key;
cnt++;
if(cnt==6){
lcd.setCursor(0,1);
lcd.print("D=save,C=cancel");
menu=3;
cnt=0;
}
}

