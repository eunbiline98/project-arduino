#define EN        8  
//Direction pin
#define Y_DIR     6
//Step pin
#define Y_STP     3 

#define X_DIR     5
//Step pin
#define X_STP     2
//Button
#define btn_1     52
#define btn_2     50

//DRV8825
int delayTime=150; //Delay between each pause (uS)
int stps=800;// Steps to move


void step(boolean dir, byte dirPin, byte stepperPin, int steps){
  digitalWrite(dirPin, dir);
  delay(100);
  
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(delayTime); 
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(delayTime); 
  }
}

void setup(){
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
}

void loop(){
    step(false, Y_DIR, Y_STP, stps); //Y, Clockwise
    step(true, Y_DIR, Y_STP, stps); //Y, Counterclockwise

    step(false, X_DIR, X_STP, stps); //Y, Clockwise
    step(true, X_DIR, X_STP, stps); //Y, Counterclockwise
  }

