int pbuttonPin = A1;// connect output to push button
int pbuttonPin1 = A2;
int relayPin = 13;// Connected to relay (LED)

int val = 0; // push value from pin 2
int val1 = 0;
int lightON = 0;//light status
int pushed = 0;//push status
int pushed1 = 0;

void setup() {
  // Robojax.com code and video tutorial for push button ON and OFF
  Serial.begin(9600);
  pinMode(pbuttonPin, INPUT_PULLUP); 
  pinMode(relayPin, OUTPUT);
 digitalWrite(relayPin, HIGH);// keep the load OFF at the begining. If you wanted to be ON, change the HIGH to LOW
}

void loop() {
// Robojax.com code and video tutorial for push button ON and OFF
  val = digitalRead(pbuttonPin);// read the push button value
  val1 = digitalRead(pbuttonPin1);
  if(val == HIGH && lightON == LOW){

    pushed = 1-pushed;
    delay(100);
  }    

  lightON = val;

      if(pushed == HIGH){
        Serial.println("Light ON");
        digitalWrite(relayPin, LOW); 
       
      }else{
        Serial.println("Light OFF");
        digitalWrite(relayPin, HIGH);
   
      }     

// Robojax.com code and video tutorial for push button ON and OFF

  delay(100);
}
