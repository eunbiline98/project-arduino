#include <Wire.h> 
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "DHT.h"
 
DHT dht(4, DHT11);
int sen_in, sen_out, offset_sen_in, offset_sen_out; // variabel sensor infrared
boolean detect_in = false, detect_out = false;
 
int count;
float  Min[25];
float suhu,
      uMember,
      uDingin, uSejuk, uNormal, uPanas, uSangatPanas,
      uSangatSedikit, uSedikit, uSedang, uBanyak, uSangatBanyak,
      N_suhu[5], N_orang[5], kondisiSuhu, kondisiOrang,
      Keluaran, Kecepatan;
String OutputFuzzy;
 
const int PWM_KIPAS   = 5,
          P1_KIPAS    = 6,
          P2_KIPAS    = 7,
          PWM_EXHAUST = 9,
          P1_EXHAUST  = 8,
          P2_EXHAUST  = 10;
 
long lastTime_readSen = 0, lastTime_LCD = 0;
int state_LCD = 1;
 
void setup() {
  Serial.begin(9600);
  lcd.begin();
  dht.begin();
  for (int i = 0; i <= 1; i++) {
    pinMode(i, INPUT);
  }
  for (int i = 5; i <= 10; i++) {
    pinMode(i, OUTPUT);
  }
 
  sen_in = analogRead(A0);
  sen_out = analogRead(A1);
  offset_sen_in = sen_in - 70;   // sample sensor masuk
  offset_sen_out = sen_out - 70; // sample sensor keluar
  count = 0;
  bacaSensor_suhu();
}
 
void loop() {
  sensor_count();
 
  if (millis() - lastTime_readSen > 2000) {
    bacaSensor_suhu();
    lastTime_readSen = millis();
  }
  //Serial.print(sen_in);
  //Serial.print(" | ");
  //Serial.print(sen_out);
  //Serial.print(" | ");
  //Serial.print(count);
 
  fuzzifikasi();
  defuzzifikasi();
  Output();
  Kecepatan = map(Keluaran, 0, 100, 0, 255);
  driveKipas_N_Exhaust(Kecepatan);
}
 
void bacaSensor_suhu() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  suhu = dht.readTemperature();
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(suhu)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}
 
void sensor_count() {
  sen_in = analogRead(A0);
  sen_out = analogRead(A1);
 
  if (sen_in > offset_sen_in) {
    detect_in = false;
  }
  else if (sen_in < offset_sen_in && detect_in == false) {
    count++;
    detect_in = true;
    if (count >= 40)count = 40;
  }
 
  if (sen_out > offset_sen_out) {
    detect_out = false;
  }
  else if (sen_out < offset_sen_out && detect_out == false) {
    count--;
    detect_out = true;
    if (count < 0)count = 0;
  }
}
 
void driveKipas_N_Exhaust(float Kecepatan) {
  digitalWrite(P1_KIPAS, LOW);
  digitalWrite(P2_KIPAS, HIGH);
  analogWrite(PWM_KIPAS, Kecepatan);
  digitalWrite(P1_EXHAUST, LOW);
  digitalWrite(P2_EXHAUST, HIGH);
  analogWrite(PWM_EXHAUST, Kecepatan);
}
 
void hitung_member(int pilih, float Nilai, float A, float B, float C) {
  switch (pilih) {
    case 1:
      if ((Nilai >= A) && (Nilai <= B)) uMember = 1;
      if ((Nilai > B) && (Nilai < C)) uMember = (C - Nilai) / (C - B);
      if (Nilai >= C) uMember = 0;
      break;
    case 2:
      if ((Nilai <= A) || (Nilai >= C)) uMember = 0;
      if ((Nilai > A) && (Nilai < B)) uMember = (Nilai - A) / (B - A);
      if ((Nilai > B) && (Nilai < C)) uMember = (C - Nilai) / (C - B);
      if (Nilai == B) uMember = 1;
      break;
    case 3:
      if (Nilai <= A) uMember = 0;
      if ((Nilai > A) && (Nilai < B)) uMember = (Nilai - A) / (B - A);
      if (Nilai >= B) uMember = 1;
      break;
  }
}
 
void fuzzifikasi() {
  // Suhu
  uMember = 0;
  hitung_member(1, suhu, 0, 24, 26);
  uDingin = uMember;
  hitung_member(2, suhu, 24, 26, 29);
  uSejuk = uMember;
  hitung_member(2, suhu, 26, 29, 32);
  uNormal = uMember;
  hitung_member(2, suhu, 29, 32, 35);
  uPanas = uMember;
  hitung_member(3, suhu, 32, 35, 35);
  uSangatPanas = uMember;
 
  // Banyak Orang
  uMember = 0;
  hitung_member(1, count, 0, 5, 10);
  uSangatSedikit = uMember;
  hitung_member(2, count, 5, 10, 20);
  uSedikit = uMember;
  hitung_member(2, count, 10, 20, 30);
  uSedang = uMember;
  hitung_member(2, count, 20, 30, 40);
  uBanyak = uMember;
  hitung_member(3, count, 30, 40, 40);
  uSangatBanyak = uMember;
}
 
//int fuzzy_set[5][5] = {
//  //Cold|Cool|Nor|Hot|Vhot
//  {20, 20, 20, 60, 80},   //| Vlow
//  {20, 40, 20, 60, 80},   //| Low
//  {40, 40, 60, 80, 100},  //| Medium
//  {40, 60, 80, 100, 100}, //| High
//  {60, 80, 100, 100, 100} //| Very high
//};
 
//20 = Vslow, 40 = Slow, 60 = Med, 80 = Fast, 100 = Vfast
int fuzzy_set[5][5] = {
  //|vLow|Low|Med|High|vHigh
  {20, 20, 40,  40,  60},  //| Cold
  {20, 40, 40,  60,  80},  //| Cool
  {40, 40, 60,  80,  100}, //| Normal
  {60, 60, 80,  100, 100}, //| Hot
  {80, 80, 100, 100, 100}  //| Very hot
};
 
void defuzzifikasi() {
  float pembil = 0, penyeb = 0, centre_of_area = 0;
  N_suhu[5] = {};
  N_orang[5] = {};
 
  for (int set = 0; set < 25;) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        float data_uSuhu[5] = {uDingin, uSejuk, uNormal, uPanas, uSangatPanas};
        N_suhu[i] = data_uSuhu[i];
        float data_uOrang[5] = {uSangatSedikit, uSedikit, uSedang, uBanyak, uSangatBanyak};
        N_orang[j] = data_uOrang[j];
 
        kondisiSuhu = max(N_suhu[i], kondisiSuhu);
        kondisiOrang = max(N_orang[j], kondisiOrang);
 
        /* Metode COA (Centre Of Area)*/
        min[set] = min(N_suhu[i], N_orang[j]);
        pembil  += Min[set] * fuzzy_set[i][j];
        penyeb  += Min[set];
        delay(5);
        set ++;
      }
    }
  }
  centre_of_area = pembil / penyeb;
  Keluaran = centre_of_area;
}
 
void basis_Aturan_output() {
  if (kondisiSuhu == uDingin && kondisiOrang == uSangatSedikit)OutputFuzzy = "Vslow";
  else if (kondisiSuhu == uDingin && kondisiOrang == uSedikit)OutputFuzzy = "Vslow";
  else if (kondisiSuhu == uDingin && kondisiOrang == uSedang)OutputFuzzy = "Slow";
  else if (kondisiSuhu == uDingin && kondisiOrang == uBanyak)OutputFuzzy = "Slow";
  else if (kondisiSuhu == uDingin && kondisiOrang == uSangatBanyak)OutputFuzzy = "Med";
 
  else if (kondisiSuhu == uSejuk && kondisiOrang == uSangatSedikit)OutputFuzzy = "Vslow";
  else if (kondisiSuhu == uSejuk && kondisiOrang == uSedikit)OutputFuzzy = "Slow";
  else if (kondisiSuhu == uSejuk && kondisiOrang == uSedang)OutputFuzzy = "Slow";
  else if (kondisiSuhu == uSejuk && kondisiOrang == uBanyak)OutputFuzzy = "Med";
  else if (kondisiSuhu == uSejuk && kondisiOrang == uSangatBanyak)OutputFuzzy = "Fast";
 
  else if (kondisiSuhu == uNormal && kondisiOrang == uSangatSedikit)OutputFuzzy = "Slow";
  else if (kondisiSuhu == uNormal && kondisiOrang == uSedikit)OutputFuzzy = "Slow";
  else if (kondisiSuhu == uNormal && kondisiOrang == uSedang)OutputFuzzy = "Med";
  else if (kondisiSuhu == uNormal && kondisiOrang == uBanyak)OutputFuzzy = "Fast";
  else if (kondisiSuhu == uNormal && kondisiOrang == uSangatBanyak)OutputFuzzy = "Vfast";
 
  else if (kondisiSuhu == uPanas && kondisiOrang == uSangatSedikit)OutputFuzzy = "Med";
  else if (kondisiSuhu == uPanas && kondisiOrang == uSedikit)OutputFuzzy = "Med";
  else if (kondisiSuhu == uPanas && kondisiOrang == uSedang)OutputFuzzy = "Fast";
  else if (kondisiSuhu == uPanas && kondisiOrang == uBanyak)OutputFuzzy = "Vfast";
  else if (kondisiSuhu == uPanas && kondisiOrang == uSangatBanyak)OutputFuzzy = "Vfast";
 
  else if (kondisiSuhu == uSangatPanas && kondisiOrang == uSangatSedikit)OutputFuzzy = "Fast";
  else if (kondisiSuhu == uSangatPanas && kondisiOrang == uSedikit)OutputFuzzy = "Fast";
  else if (kondisiSuhu == uSangatPanas && kondisiOrang == uSedang)OutputFuzzy = "Vfast";
  else if (kondisiSuhu == uSangatPanas && kondisiOrang == uBanyak)OutputFuzzy = "Vfast";
  else if (kondisiSuhu == uSangatPanas && kondisiOrang == uSangatBanyak)OutputFuzzy = "Vfast";
}
 
void Output() {
  //uMember_suhu_count();
  basis_Aturan_output();
  Tampil_Serial();
  Tampil_LCD();
}
 
void Tampil_Serial() {
  //Tampil Serial
  Serial.print(" Temp : ");
  Serial.print(suhu);
  Serial.print(" *C ");
  Serial.print(" ");
  tampil_member_suhu();
  if (kondisiSuhu == N_suhu[0])Serial.print("Cold");
  else if (kondisiSuhu == N_suhu[1])Serial.print("Cool");
  else if (kondisiSuhu == N_suhu[2])Serial.print("Normal");
  else if (kondisiSuhu == N_suhu[3])Serial.print("Hot");
  else if (kondisiSuhu == N_suhu[4])Serial.print("Very Hot");
 
  Serial.print(" | Count : ");
  Serial.print(count);
  Serial.print(" ");
  tampil_member_count();
  if (kondisiOrang == N_orang[0])Serial.print("Very Low");
  else if (kondisiOrang == N_orang[1])Serial.print("Low");
  else if (kondisiOrang == N_orang[2])Serial.print("Medium");
  else if (kondisiOrang == N_orang[3])Serial.print("High");
  else if (kondisiOrang == N_orang[4])Serial.print("Very High");
 
  Serial.print(" | ");
  if (OutputFuzzy == "Vslow")     Serial.print("Fan : Very Slow");
  else if (OutputFuzzy == "Slow") Serial.print("Fan : Slow");
  else if (OutputFuzzy == "Med")  Serial.print("Fan : Medium");
  else if (OutputFuzzy == "Fast") Serial.print("Fan : Fast");
  else if (OutputFuzzy == "Vfast")Serial.print("Fan : Very Fast");
  Serial.print(" LV : ");
  Serial.print(Keluaran);
  Serial.print("%");
  Serial.print(" PWM : ");
  Serial.print(Kecepatan);
  Serial.println();
}
 
void Tampil_LCD() {
  lcd.backlight();
  if (millis() - lastTime_LCD > 5000) {
    state_LCD = 1;
    lastTime_LCD = millis();
  }
  if (millis() > lastTime_LCD + 2500) {
    state_LCD = 2;
  }
 
  if (state_LCD == 1) {
    lcd.setCursor (0, 0);
    lcd.print("Temp:");
    lcd.print((int)suhu);
    lcd.print("C ");
    if (kondisiSuhu == N_suhu[0])lcd.print("   Cold");
    if (kondisiSuhu == N_suhu[1])lcd.print("   Cool");
    if (kondisiSuhu == N_suhu[2])lcd.print(" Normal");
    if (kondisiSuhu == N_suhu[3])lcd.print("    Hot");
    if (kondisiSuhu == N_suhu[4])lcd.print("VeryHot");
 
    lcd.setCursor (0, 1);
    lcd.print("Cont:");
    lcd.print((int)count);
    lcd.print(" ");
    lcd.setCursor (7, 1);
    if (kondisiOrang == N_orang[0])lcd.print("  VeryLow");
    if (kondisiOrang == N_orang[1])lcd.print("      Low");
    if (kondisiOrang == N_orang[2])lcd.print("   Medium");
    if (kondisiOrang == N_orang[3])lcd.print("     High");
    if (kondisiOrang == N_orang[4])lcd.print(" VeryHigh");
  }
  else if (state_LCD == 2) {
    lcd.setCursor (0, 0);
    lcd.print("  PWM: ");
    lcd.print((float)Keluaran);
    lcd.print("%   ");
    lcd.setCursor (0, 1);
    if (OutputFuzzy == "Vslow")     lcd.print("   Very Slow    ");
    else if (OutputFuzzy == "Slow") lcd.print("      Slow      ");
    else if (OutputFuzzy == "Med")  lcd.print("     Medium     ");
    else if (OutputFuzzy == "Fast") lcd.print("      Fast      ");
    else if (OutputFuzzy == "Vfast")lcd.print("   Very Fast    ");
  }
}
void uMember_suhu_count() {
  Serial.print(" Temp : ");
  Serial.print(suhu);
  Serial.print(" *C ");
  Serial.print(" ");
  Serial.print("Cold:");
  Serial.print(uDingin);
  Serial.print(" Cool:");
  Serial.print(uSejuk);
  Serial.print(" Nor:");
  Serial.print(uNormal);
  Serial.print(" Hot:");
  Serial.print(uPanas);
  Serial.print(" VHot:");
  Serial.print(uSangatPanas);
  Serial.print(" ");
 
  Serial.print(" | Count : ");
  Serial.print(count);
  Serial.print(" ");
  Serial.print("VLow:");
  Serial.print(uSangatSedikit);
  Serial.print(" Low:");
  Serial.print(uSedikit);
  Serial.print(" Med:");
  Serial.print(uSedang);
  Serial.print(" High:");
  Serial.print(uBanyak);
  Serial.print(" VHigh:");
  Serial.println(uSangatBanyak);
}
 
void tampil_member_suhu() {
  //if (uDingin != 0.00) {
    Serial.print("Cold:");
    Serial.print(uDingin);
  //}
  //if (uSejuk != 0.00) {
    Serial.print(" Cool:");
    Serial.print(uSejuk);
  //}
  //if (uNormal != 0.00) {
    Serial.print(" Nor:");
    Serial.print(uNormal);
  //}
  //if (uPanas != 0.00) {
    Serial.print(" Hot:");
    Serial.print(uPanas);
  //}
  //if (uSangatPanas != 0.00) {
    Serial.print(" VHot:");
    Serial.print(uSangatPanas);
  //}
  Serial.print(" ");
}
 
void tampil_member_count() {
  //if (uSangatSedikit != 0.00) {
    Serial.print("VLow:");
    Serial.print(uSangatSedikit);
  //}
  //if (uSedikit != 0.00) {
    Serial.print(" Low:");
    Serial.print(uSedikit);
  //}
  //if (uSedang != 0.00) {
    Serial.print(" Med:");
    Serial.print(uSedang);
  //}
  //if (uBanyak != 0.00) {
    Serial.print(" High:");
    Serial.print(uBanyak);
  //}
  //if (uSangatBanyak != 0.00) {
    Serial.print(" VHigh:");
    Serial.print(uSangatBanyak);
  //}
  Serial.print(" ");
}
