#define SensorSuara 12

#define SensorPir 2 // sensor pin
int count_pir =0;
unsigned long timer_now=millis();
boolean start_timer=false;
unsigned long last_activ=0;

#define relay_lamp_1 8
#define relay_lamp_2 9
#define tombol_1  3 // saklar lampu
int lampu = 0; 
int lightON = 0;
int st = 0;
int count = 0;

#define relay_ac 11
#define tombol_2  4 // saklar air conditioner
int ac = 0; 
int acON = 0;
int st1 = 0;

#define relay_lcd 10
#define tombol_3  5 // saklar proyektor lcd
int proyektor = 0; 
int proyektorON = 0;
int st2 = 0;

#define led_on 6
#define led_off 7

int state=0;

void setup() {
  Serial.begin(9600); 
  pinMode(SensorSuara,INPUT);
  pinMode(SensorPir, INPUT_PULLUP);
  pinMode(tombol_1, INPUT_PULLUP);
  pinMode(tombol_2, INPUT_PULLUP);
  pinMode(tombol_3, INPUT_PULLUP);
  pinMode(relay_lamp_1, OUTPUT);
  pinMode(relay_lamp_2, OUTPUT);
  pinMode(relay_ac, OUTPUT);
  pinMode(relay_lcd, OUTPUT);
  pinMode(led_on, OUTPUT);
  pinMode(led_off, OUTPUT);
  digitalWrite(relay_lamp_1, HIGH);
  digitalWrite(relay_lamp_2, HIGH);
  digitalWrite(relay_ac, HIGH);
  digitalWrite(relay_lcd, HIGH);

  digitalWrite(led_on, HIGH);
  digitalWrite(led_off, LOW);
  delay(10000);
}
 
void loop() {
  if((digitalRead(tombol_1)==LOW || digitalRead(tombol_2)==LOW || digitalRead(tombol_3)==LOW) && state == 0){
    digitalWrite(relay_lamp_1, LOW);
    digitalWrite(relay_lamp_2, LOW);
    digitalWrite(relay_lcd, LOW);
    digitalWrite(relay_ac, LOW);
    delay(1500);
    digitalWrite(relay_ac, HIGH);
    state=1;
  }
  if(state==0){
    digitalWrite(relay_lamp_1, HIGH);
    digitalWrite(relay_lamp_2, HIGH);
    digitalWrite(relay_ac, HIGH);
    digitalWrite(relay_lcd, HIGH);
    digitalWrite(led_on, HIGH);
    digitalWrite(led_off, LOW);

   digitalWrite(led_on, HIGH);
   digitalWrite(led_off, LOW);
  }else{
    deteksigerakan();
    baca_pir();
  }
  /*
  int kondisi_pir = digitalRead(SensorPir);
  if(kondisi_pir == HIGH) {
   deteksigerakan();
   
  }
  else{
  digitalWrite(relay_lamp_1, HIGH);
  digitalWrite(relay_lamp_2, HIGH);
  digitalWrite(relay_ac, HIGH);
  digitalWrite(relay_lcd, HIGH);

  // kembali status aktif LOW pada lampu dan ac jika sensor == 1 maka lampu / ac == 1 
  st =1; st1 =1;
  // kembali status aktif HIGH pada lcd proyektor jika sensor == 1 maka lcd proyektor == 0 
  st2 =0;
  
  digitalWrite(led_on, HIGH);
  digitalWrite(led_off, LOW);
  }*/
}
void baca_pir() {
//  int kondisi_suara = digitalRead(SensorSuara);
  timer_now=millis();
  if (timer_now - last_activ > (5*1000)){
      int kondisi_pir = digitalRead(SensorPir);
      if (kondisi_pir == HIGH){
          state = 1;
          count_pir=0;
       }else{
        count_pir ++;  
       }
       
      if (count_pir == 20){
        state = 0;
        count_pir=0;

        digitalWrite(relay_ac, LOW);
        delay(1500);
        digitalWrite(relay_ac, HIGH);
      }
      last_activ=millis();
  }
  
  
}
void deteksigerakan() {
  digitalWrite(led_on, LOW);
  digitalWrite(led_off, HIGH);
//  lampu = digitalRead(tombol_1);
//  if(lampu == HIGH && lightON == LOW){
//    st = 1-st;
//    delay(100);
//  }    
//  lightON = lampu;
//    if(st == HIGH){
//      digitalWrite(relay_lamp, LOW);   
//    }
//    else{
//      digitalWrite(relay_lamp, HIGH);
//    }     

  lampu = digitalRead(tombol_1); 
  if(lampu == LOW){
    delay(1000);
    count++; 
  }
    if(count == 1){
        digitalWrite(relay_lamp_1, LOW);  
        digitalWrite(relay_lamp_2, HIGH);  
    }
      if(count == 2){
        digitalWrite(relay_lamp_1, HIGH);  
        digitalWrite(relay_lamp_2, LOW);
      }
        if(count == 3){
        digitalWrite(relay_lamp_1, LOW);  
        digitalWrite(relay_lamp_2, LOW);
        }
        if(count == 4){
        digitalWrite(relay_lamp_1, HIGH);  
        digitalWrite(relay_lamp_2, HIGH);
        count = 0;
        }
//  ac = digitalRead(tombol_2);
//  if(ac == LOW && acON == LOW){
//    st1 = 1-st1;
//    delay(300);
//      
//  //acON = ac;
//    if(st1 == HIGH){
//      digitalWrite(relay_ac, LOW); 
//      delay(200);
//      digitalWrite(relay_ac, HIGH);
//    }
//    else{
//      digitalWrite(relay_ac, HIGH); 
//      delay(200);
//      digitalWrite(relay_ac, LOW);
//    }     
//  }

  ac = digitalRead(tombol_3);
  if(ac == LOW){
    digitalWrite(relay_ac, LOW);
    delay(1500);
    digitalWrite(relay_ac, HIGH);
  }
    
  proyektor = digitalRead(tombol_2);
  if(proyektor == LOW && proyektorON == LOW){
    st2 = 1-st2;
    delay(1000);    
  proyektorON = proyektor;
    if(st2 == HIGH){
      digitalWrite(relay_lcd, LOW);  
    }
    else{
      digitalWrite(relay_lcd, HIGH);
    }  
  }   
}
 
