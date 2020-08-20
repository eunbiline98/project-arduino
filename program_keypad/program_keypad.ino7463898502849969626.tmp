/* --- www.nyebarilmu.com --- */

#include <Keypad.h>

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

void setup(){ 
//penggunaan serial komunikasi dengan kecepatan 9600 bps 
Serial.begin(9600); 
}

void loop(){
char tombol = keypad.getKey();
if (tombol!= NO_KEY){
Serial.println(tombol);}
}
