#include <SPI.h>       
#include <DMD.h>        
#include <TimerOne.h>  
#include "SystemFont5x7.h"
#include "Arial_Black_16_ISO_8859_1.h"
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

void setup(void)
{
   Timer1.initialize( 3000 );          
   Timer1.attachInterrupt( ScanDMD );   
   dmd.clearScreen( true );   
  Serial.begin(115200);
}

void loop(void)
{
   dmd.clearScreen( true );
   dmd.selectFont(Arial_Black_16_ISO_8859_1);

   const char *MSG = "SELAMAT DATANG DI ATMDUINO ROBOTIC SHOP, MENERIMA PESANAN PROJECT PEMBUATAN ALAT / JASA KONSULTASI UNTUK TUGAS AKHIR / SKRIPSI ,BERBASIS MICROCONTROLER BERBASIS ARDUINO, ESP8266, ESP32, STM32, HUB:08973234445";
   dmd.drawMarquee(MSG,strlen(MSG),(200*DISPLAYS_ACROSS)-1,0);
   long start=millis();
   long timer=start;
   
   while(1){
     if ((timer+40) < millis()) {
       dmd.stepMarquee(-1,0);
       timer=millis();
     }
   }
   
}
