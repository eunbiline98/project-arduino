#include <GY6050.h>          
#include <Wire.h>
#include <Servo.h>

#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;  


int X = 0;
int Y = 0;
GY6050 gyro(0x68);             


void setup() {
  Serial.begin(9600);
  lcd.begin();
  Wire.begin();            
  gyro.initialisation();
  delay(100);
  myservo.attach(9);
}

void loop() {
  lcd_kordinat_show();
  X = map(gyro.refresh('A', 'X'), -90, 90, 0, 180);                
  Y = map(gyro.refresh('A', 'Y'), -90, 90, 0, 180);
   
  myservo.write(Y);   

  Serial.print("AcX = ");
  Serial.print(gyro.refresh('A', 'X'));           
  Serial.print(" | AcY = ");
  Serial.print(gyro.refresh('A', 'Y'));           
  Serial.print(" | AcZ = ");
  Serial.println(gyro.refresh('A', 'Z'));           

}

void lcd_kordinat_show(){
//  lcd.setCursor(0,0);
//  lcd.print(" X     Y     Z");
  lcd.setCursor(0,1);
  lcd.print(gyro.refresh('A', 'X'));

  lcd.setCursor(7,1);
  lcd.print(gyro.refresh('A', 'Y'));
  
  lcd.setCursor(12,1);
  lcd.print(gyro.refresh('A', 'Z'));
  delay(70);
  lcd.clear();

if (gyro.refresh('A', 'Y') <=5 && gyro.refresh('A', 'Y') <= -5 ){

    lcd.setCursor(0,0);
    lcd.print("Pos:Miring Kanan   ");
  }
  else if (gyro.refresh('A', 'Y') >= 5){

    lcd.setCursor(0,0);
    lcd.print("Pos:Miring Kiri   ");
  }
    else if (gyro.refresh('A', 'Y') >= -5){
      
    lcd.setCursor(0,0);
    lcd.print("Pos:Center      ");
  }
}
