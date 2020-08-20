#include<Servo.h>
Servo servo;

#define EN        8  

//Direction pin
#define X_DIR     5 
#define Y_DIR     6
#define Z_DIR     7

//Step pin
#define X_STP     2
#define Y_STP     3 
#define Z_STP     4

//limit switch
#define set_1 52


//batas gripper ambil susu bear brand
int maxbb = 67;
int minbb = 0;

//batas gripper ambil mie 
int maxmie = 45;
int minmie = 10;

//batas gripper ambil sabun
int maxsabun = 50;
int minsabun = 5;

//batas gripper ambil gelas
int maxgelas = 60;
int mingelas = 50;

int setPin = 22;
int bearbrand = 10;
int mie = 11;
int sabun = 12;
int gelas = 13;
int val;


//DRV8825
int delayTime=100; //Delay between each pause (uS)
int level_1 = 60;
int stps=15000;// Steps to move
int z_stps = 1800;
int stopp = 0; // steps to stop


void step(boolean dir, byte dirPin,  byte stepperPin2, byte stepperPin3, int steps){
  digitalWrite(dirPin, dir);
  delay(100);
  
  for (int i = 0; i < steps; i++) {
   // digitalWrite(stepperPin1, HIGH);
    digitalWrite(stepperPin2, HIGH);
    digitalWrite(stepperPin3, HIGH);
    delayMicroseconds(delayTime); 
   //digitalWrite(stepperPin1, LOW);
    digitalWrite(stepperPin2, LOW);
    digitalWrite(stepperPin3, LOW);   
    delayMicroseconds(level_1);  
  }
  
}

void set_awal(){
   digitalWrite(5, true);
    delay(100);
    
    for (int i = 0; i < 20000; i++) {
   // digitalWrite(stepperPin1, HIGH);
    digitalWrite(2, HIGH);
    delayMicroseconds(delayTime); 
   //digitalWrite(stepperPin1, LOW);
    digitalWrite(2, LOW);  
    delayMicroseconds(delayTime); 
    if (digitalRead(52)==0)
    break;
  }
  }

void setup(){
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(52, INPUT_PULLUP); 
  digitalWrite(EN, LOW);
 
  servo.attach(setPin);
  servo.write(0);
  //set_awal();
}

void loop(){
   jalan();
} 

void jalan(){
  bukagripper();
   step(false, X_DIR, X_STP, X_STP, stps); 
  
//   step(false, Z_DIR, Z_STP, Z_STP, z_stps);
   tutupgripper();
   step(true, X_DIR, X_STP, X_STP, stps);
//   step(true, Z_DIR, Z_STP, Z_STP, z_stps);
   step(false, Y_DIR, Y_STP, Y_STP, stps);
   step(false, X_DIR, X_STP, X_STP, stps);
   bukagripper();
//   step(true, Z_DIR, Z_STP, Z_STP, z_stps);  
   step(true, X_DIR, X_STP, X_STP, stps);
   tutupgripper(); 
   step(true, Y_DIR, Y_STP, Y_STP, stps); 
}

void bukagripper(){
    for(val = 0; val <= 67; val++){
        servo.write(val);
        delay(20);
        if(val == 67){
            val = 67;
          }
      }
  }

  void tutupgripper(){
      for(val = 67; val>=0; val--){
          servo.write(val);
          delay(20);
          if(val == 0){
              val = 0;
            }
        }
    }
