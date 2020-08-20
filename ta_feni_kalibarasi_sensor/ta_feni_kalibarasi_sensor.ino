//sesuikan posisi pin select
int s0 = D2;
int s1 = D1;
int s2 = D0;
  
//gunakan A0 sebagai input
int analogPin = A0;
  
//variabel untuk menyimpan nilai input
int nilaiInput = 0;

//variabel untuk menyimpan nilai input
int sensor_soil_1 = 0;
int sensor_soil_2 = 0;
float kelembaban_tanah_1, kelembaban_tanah_2;

//setup nilai ambang kelembaban tanah
int nilai_lembab = 70.00;
int nilai_kering = 40.00;

//solenoid water valve
#define keran D5

void setup() {
  //aktifkan komunikasi serial
  Serial.begin(9600);
  //jadikan pin select sebagai output
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(keran,OUTPUT);
}
  
void loop() {
  //sensor soil moisture 1
  digitalWrite(s0,LOW);
  digitalWrite(s1,LOW);
  digitalWrite(s2,LOW);
  sensor_soil_1 = analogRead(analogPin);
  Serial.print(sensor_soil_1);
  Serial.print(" ");

  kelembaban_tanah_1 = ( 100 - ( (sensor_soil_1/1023.00) * 100 ) );
  Serial.print("Sensor 1 Kelembaban tanah = ");
  Serial.print(kelembaban_tanah_1);
  Serial.print("%\n\n");
  delay(1000);
     
  //sensor soil moisture 2
  digitalWrite(s0,HIGH);
  digitalWrite(s1,LOW);
  digitalWrite(s2,LOW);
  sensor_soil_2 = analogRead(analogPin);
  Serial.println(sensor_soil_2);
  
  kelembaban_tanah_2 = ( 100 - ( (sensor_soil_2/1023.00) * 100 ) );
  Serial.print("Sensor 2 Kelembaban tanah = ");
  Serial.print(kelembaban_tanah_2);
  Serial.print("%\n\n");
  delay(1000);
  
// water valve logic kedua sensor data sesuai
  if ((kelembaban_tanah_1 >= nilai_lembab)&&(kelembaban_tanah_2 >= nilai_lembab)){
    digitalWrite(keran,HIGH);
  }
  if ((kelembaban_tanah_1 <= nilai_kering)&&(kelembaban_tanah_2 <= nilai_kering)){
    digitalWrite(keran,LOW);
  }  
// water valve logic salah satu sensor data sesuai
    if ((kelembaban_tanah_1 >= nilai_lembab)||(kelembaban_tanah_2 >= nilai_lembab)){
    digitalWrite(keran,HIGH);
  }
  if ((kelembaban_tanah_1 <= nilai_kering)||(kelembaban_tanah_2 <= nilai_kering)){
    digitalWrite(keran,LOW);
  }  
}
