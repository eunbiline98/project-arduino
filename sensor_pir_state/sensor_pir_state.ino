int indicator = 9;               
int inputPin = 8;               
int pirState = LOW;             
int val = 0;                    
int x = HIGH;
int pirReading;
//int pulseSpeed = 100 ; 

void setup() {
  pinMode(indicator, OUTPUT);   
  pinMode(inputPin, INPUT);     
 
  Serial.begin(9600);
}
 
void loop(){

  val = digitalRead(inputPin);                      
    if (val == HIGH) {  
      if (x == HIGH) {                          
      digitalWrite(indicator, HIGH); 
      Serial.println("Motion 2");
      }
      else{
       digitalWrite(indicator, LOW);
       Serial.println("Motion 1");  
      }
      if (pirState == LOW) {
        Serial.println("Motion detected!");
        
        pirState = HIGH;
        }
    }   
    else {   
      if (pirState == HIGH){
        if (x==HIGH){
          x=LOW;
        }
        else {
          x=HIGH;
        }
        Serial.println("Motion ended!");
        pirState = LOW;
        
      }
      //delay(pulseSpeed);
    }
}
