#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIGGER_PIN  13
#define ECHO_PIN     12


#include <Wire.h> 
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int btn_down = 3; 
int btn_up = 2;  
int btn_ok = 4; 
int pompa_1 = 5;
int pompa_2 = 6;
int pintu_air = 7;

int Menu = 0;       
int pilihan = 1;    
char* pilihan_menu[] = {"       ",
                       "Pintu Air",                   
                       "Pompa 1",
                       "Pompa 2",
                       "Jarak  ", 
                       "Pompa 1 On", "Pompa 1 Off",
                       "Pompa 2 On", "Pompa 2 Off",};

void setup(){
  lcd.begin();
  Serial.begin(9600);
  pinMode(btn_down, INPUT_PULLUP);
  pinMode(btn_ok, INPUT_PULLUP);
  pinMode(btn_up, INPUT_PULLUP);
  pinMode(pompa_1, OUTPUT);
  pinMode(pompa_2, OUTPUT);
  pinMode(pintu_air, INPUT_PULLUP);
}

void loop(){
  halaman_Menu_0();
  //Correr a função:
 halaman_1();
  //Correr a função:
  halaman_Menu_2();
  //Correr a função:
 halaman_2_1();
  //Correr a função:
 halaman_2_2();
  //Correr a função:
  halaman_Menu_3();
  //Correr a função:
 halaman_3_1();
  //Correr a função:
 halaman_3_2();
  //Correr a função:
 halaman_4();
  
  //Atraso para o próximo passo:
  delay(200);
}

//Página do Menu - 0:
void halaman_Menu_0(){
  //Condição para abrir página:
  if (Menu == 0){
    //Apaga a informação do LCD.
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(pilihan_menu[0]);

 if (digitalRead(btn_down) == LOW){
      pilihan = pilihan+1;
      delay(100);
    }
 
    //Condição para escrever no LCD os nomes das páginas:
    if (pilihan > 0 & pilihan < 3){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 0);
      //Escreve no LCD:
      lcd.print(pilihan_menu[1]);
            
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 1);
      //Escreve no LCD:
      lcd.print(pilihan_menu[2]);
    }
    //Condição para escrever no LCD os nomes das páginas:
    else if (pilihan > 2 & pilihan < 5){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 0);
      //Escreve no LCD:
      lcd.print(pilihan_menu[3]);
      
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 1);
      //Escreve no LCD:
      lcd.print(pilihan_menu[4]);
    }
        
    //Condição para selecionar página 1 e 3:
    if (pilihan == 1 || pilihan == 3){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(0, 0);
      //Escreve no LCD:
      lcd.print(">");
    }
    //Condição para selecionar página 2 e 4:
    else if (pilihan == 2 || pilihan == 4){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(0, 1);
      //Escreve no LCD:
      lcd.print(">");
    }
  
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW){
      //Condição para selecionar página:
      if (pilihan >= 1){
        //Altera o valor de selecção do título do Menu.
        pilihan = pilihan + 1;
      }
      //Condição para selecionar página:
      if (pilihan > 4){
        //Altera o valor de selecção do título do Menu.
        pilihan = 4;
      } 
    }  
      
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW){
      //Condição para selecionar página:
      if (pilihan < 1){
        //Altera o valor de selecção do título do Menu.
        pilihan = 1;
      }
      //Condição para selecionar página:
      if (pilihan <= 4){
        //Altera o valor de selecção do título do Menu.
        pilihan = pilihan - 1;
      }
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW){
      //Condição para selecionar página:
      if (pilihan == 1){
        //Altera o valor de selecção do Menu.
        Menu = 1;
      }
      //Condição para selecionar página:
      else if (pilihan == 2){
        //Altera o valor de selecção do título do Menu.
        pilihan = 5;
        //Altera o valor de selecção do Menu.
        Menu = 2;
      }
      //Condição para selecionar página:
      else if (pilihan == 3){
        //Altera o valor de selecção do título do Menu.
        pilihan = 7;
        //Altera o valor de selecção do Menu.
        Menu = 3;
      }
      //Condição para selecionar página:
      else if (pilihan == 4) {
        //Altera o valor de selecção do Menu.
        Menu = 4;
      }
    }
  }
}

//Pagina do Menu - 1:
void halaman_1(){
  //Condição para abrir página:
  if (Menu == 1){
    //Apaga a informação do LCD.
    lcd.clear();
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 0);
    //Escreve no LCD:
    lcd.print(pilihan_menu[pilihan]);
    lcd.print(":");
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 1);
    //Escreve no LCD:
    lcd.print("Status: ");
    pintu_air=digitalRead(7);
    if(pintu_air==HIGH){lcd.print("Terbuka");}
    if(pintu_air==LOW){lcd.print("Tertutup");}
    
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW){
      //Altera o valor de selecção do Menu.
      Menu = 0;
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW){
      //Não realiza nenhuma acção.
    }
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW){
      //Não realiza nenhuma acção.
    }
  }
}

//Página do Menu - 2:
void halaman_Menu_2(){
  //Condição para abrir página:
  if (Menu == 2){
    //Apaga a informação do LCD.
    lcd.clear();     
    //Condição para escrever no LCD os nomes das páginas:
    if (pilihan == 5 || pilihan == 6){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 0);
      //Escreve no LCD:
      lcd.print(pilihan_menu[5]);
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 1);
      //Escreve no LCD:
      lcd.print(pilihan_menu[6]);
    }
    //Condição para selecionar página 2.1:
    if (pilihan == 5){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(0, 0);
      //Escreve no LCD:
      lcd.print(">");
    }
    //Condição para selecionar página 2.2:
    else if (pilihan == 6){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(0, 1);
      //Escreve no LCD:
      lcd.print(">");
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW){      
      //Condição para selecionar página:
      if (pilihan == 5){
        //Altera o valor de selecção do título do Menu.
        pilihan = pilihan + 1;
      }
      //Condição para selecionar página:
      else if (pilihan == 6){
        //Altera o valor de selecção do título do Menu.
        pilihan = pilihan - 1;
      } 
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW){
      //Altera o valor de selecção do título do Menu.
      pilihan = 2;
      //Altera o valor de selecção do Menu.
      Menu = 0;
    }
        
    //Atraso para o próximo passo:
    delay(100);
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW){
      //Condição para selecionar página:
      if (pilihan == 5){
        //Altera o valor de selecção do Menu.
        Menu = 5;
      }
      //Condição para selecionar página:
      else if (pilihan == 6){
        //Altera o valor de selecção do Menu.
        Menu = 6;  
      }
    }
  }
}

//Página do Menu - 2.1:
void halaman_2_1(){
  //Condição para abrir página:
  if (Menu == 5){
    //Apaga a informação do LCD.
    lcd.clear();
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 0);
    //Escreve no LCD:
    lcd.print(pilihan_menu[5]);
    lcd.print(":");
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(1, 1);
    //Escreve no LCD:
     lcd.print("Pompa 1 ON...");
     digitalWrite(pompa_1, HIGH);
        
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW) {
      //Altera o valor de selecção do Menu.
      Menu = 2;
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW) {
      //Não realiza nenhuma acção.
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW) {
      //Não realiza nenhuma acção.
    }
  }
}

//Página do Menu - 2.2:
void halaman_2_2() {
  //Condição para abrir página:
  if (Menu == 6){
    //Apaga a informação do LCD.
    lcd.clear();
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 0);
    //Escreve no LCD:
    lcd.print(pilihan_menu[6]);
    lcd.print(":");
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(1, 1);
    //Escreve no LCD:
    lcd.print("Pompa 1 OFF...");
    digitalWrite(pompa_1, LOW);
    delay(500);
 
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW) {
      //Altera o valor de selecção do Menu
      Menu = 2;
    }
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW) {
      //Não realiza nenhuma acção.
    }
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW) {
      //Não realiza nenhuma acção.
    }
  }
}

//Página do Menu - 3:
void halaman_Menu_3(){
  //Condição para abrir página:
  if (Menu == 3){
    //Apaga a informação do LCD.
    lcd.clear();
    //Condição para escrever no LCD os nomes das páginas:
    if (pilihan > 6 & pilihan < 9){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 0);
      //Escreve no LCD:
      lcd.print(pilihan_menu[7]);
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 1);
      //Escreve no LCD:
      lcd.print(pilihan_menu[8]);
    }
    //Condição para escrever no LCD os nomes das páginas:
    else if (pilihan == 9){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(1, 0);
      //Escreve no LCD:
      lcd.print(pilihan_menu[9]);
    }
    //Condição para selecionarhalaman 3.1:
    if (pilihan == 7 || pilihan == 9){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(0, 0);
      //Escreve no LCD:
      lcd.print(">");
    }
    //Condição para selecionarhalaman 3.2:
    else if (pilihan == 8){
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(0, 1);
      //Escreve no LCD:
      lcd.print(">");
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW){
      //Condição para selecionar página:
      if (pilihan >= 7){
        //Altera o valor de selecção do título do Menu.
        pilihan = pilihan + 1;
      }
      //Condição para selecionar página:
      if (pilihan > 9){
        //Altera o valor de selecção do título do Menu.
        pilihan = 8;
      } 
    }  
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW){
      //Altera o valor de selecção do titulo do Menu.
      pilihan = 3;
      //Altera o valor de selecção do Menu.
      Menu = 0;
    }
    
    //Atraso para o próximo passo:
    delay(100);
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW){
      //Condição para selecionar página:
      if (pilihan == 7){
        //Altera o valor de selecção do Menu.
        Menu = 7;
      }
      //Condição para selecionar página:
      else if (pilihan == 8){
        //Altera o valor de selecção do Menu.
        Menu = 8;
      }
      //Condição para selecionarhalaman:
      else if (pilihan == 9){
        //Altera o valor de selecção do Menu.
        Menu = 9;
      }
    }
  }
}

//Página do Menu - 3.1:
void halaman_3_1() {
  //Condição para abrir página:
  if (Menu == 7){
    //Apaga a informação do LCD.
    lcd.clear();
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 0);
    //Escreve no LCD:
    lcd.print(pilihan_menu[7]);
    lcd.print(":");
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(1, 1);
    //Escreve no LCD:
    lcd.print("Pompa 2 ON...");
    digitalWrite(pompa_2,HIGH);
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW) {
      //Altera o valor de selecção do Menu.
      Menu = 3;
    }
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW) {
      //Não realiza nenhuma acção.
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW) {
      //Não realiza nenhuma acção.
    }
  }
}
//Página do Menu - 3.2:
void halaman_3_2() {
  //Condição para abrir página:
  if (Menu == 8){
    //Apaga a informação do LCD.
    lcd.clear();
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 0);
    //Escreve no LCD:
    lcd.print(pilihan_menu[8]);
    lcd.print(":");
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(1, 1);
    //Escreve no LCD:
    lcd.print("Pompa 2 OFF...");
    digitalWrite(pompa_2,LOW);
       
    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW) {
      //Altera o valor de selecção do Menu.
      Menu = 3;
    }
    
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW) {
      //Não realiza nenhuma acção.
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW) {
      //Não realiza nenhuma acção.
    }
  }
}

//Página do Menu - 4:
void halaman_4() {
  
  //Condição para abrir página:
  if (Menu == 4){
    //Apaga a informação do LCD.
    lcd.clear();
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 0);
    //Escreve no LCD:
    lcd.print(pilihan_menu[pilihan]);
    lcd.print(":");
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(1, 1);
    //Escreve no LCD:
   

    //Condição para acção dos botões na página:
    if (digitalRead(btn_up) == LOW){
      //Altera o valor de selecção do Menu.
      Menu = 0;
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_ok) == LOW){
      //Não realiza nenhuma acção.
    }
    //Condição para acção dos botões na página:
    if (digitalRead(btn_down) == LOW){
      //Não realiza nenhuma acção.
    }
  }
}
