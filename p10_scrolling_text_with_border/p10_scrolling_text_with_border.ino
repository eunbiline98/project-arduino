
/********************************************************************************************************************************
 ***********************************code ini dibuat olehttps://github.com/fahrudinafif89 ****************************************
 *******************************https://github.com/fahrudinafif89/arduino-Running-text-on-p10************************************
  *******************************************************************************************************************************/
  
#include <SPI.h>        
#include <DMD.h>        
#include <TimerOne.h>   
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

void ScanDMD()
{ 
 dmd.scanDisplayBySPI();
}


void setup(void)
{  
  Timer1.initialize( 5000 );           
  Timer1.attachInterrupt( ScanDMD );     
  dmd.clearScreen( true );  
}


void loop(void)
{ 
  dmd.clearScreen( true );
 // dmd.selectFont(Arial_Black_16);
  dmd.selectFont(System5x7);

char teks[] = "SELAMAT DATANG DI ATMDUINO ROBOTIC SHOP, MENERIMA PESANAN PROJECT PEMBUATAN ALAT / JASA KONSULTASI UNTUK TUGAS AKHIR / SKRIPSI ,BERBASIS MICROCONTROLER BERBASIS ARDUINO, ESP8266, ESP32, STM32, HUB:08973234445    ";   //tulisan yang akan ditampilkan
int lebarTeks = 0;
for (int i = 0; i < sizeof(teks)+1; i++) {
  lebarTeks += dmd.charWidth(teks[i]) + 1;
}
dmd.drawBox(  0,  0, (32*DISPLAYS_ACROSS)-1, (16*DISPLAYS_DOWN)-1, GRAPHICS_NORMAL );
for (int i = 0; i<lebarTeks; i++) {
  dmd.drawString (63-i, 3, teks, sizeof(teks), GRAPHICS_NORMAL);
  dmd.drawBox(  0,  0, (64  *DISPLAYS_ACROSS)-1, (16*DISPLAYS_DOWN)-1, GRAPHICS_NORMAL );
  delay(100);
} 
  
}
