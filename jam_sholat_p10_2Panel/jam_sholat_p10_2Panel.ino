/////////////////////////////diolah dari bergai sumber hasil browsing dan copas sana-sini/////////////////////////////
////////tutorial lebih lengkap kunjungi : www.rokhmad.com   //////

//////ini library nya/// jika belum punya kunjungi github.com //////
#include <DMD.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>
#include "PrayerTimes.h"
#include <SystemFont5x7.h>
#include <Arial_black_16.h>
#include <SPI.h>
char weekDay[][7] = {"SENIN" , "SELASA", "RABU", "KAMIS", "JUM'AT", "SABTU", "MINGGU",  }; // array hari, dihitung mulai dari senin, hari senin angka nya =0,
int langkah;
int lama_tampilnya;
boolean Waduh_Setting = false;
///////////////////////////////// Mendifinisikan Pin Tombol ////////////////////////////////////////
#define Jam_bUP A1                  // Tombol setting sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 3 Digital
#define tombol_bOK A2             // Tombol OK sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 5 Digital
#define Jam_bDN A3           //tombol setting   sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 4 Digital

///////////////////koding ini untuk setting rtc sesuai dengan laptop//// /////
//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
//DateTime dt(2017, 5, 4, 15, 11, 0, 4);

/*
 * PIN YANG DISAMBUNG
 * Arduino              P-10
 * -D6          ->      -A
 * -D7          ->      -B
 * -D8          ->      -SCLK
 * -D9          ->      -OE
 * -D11         ->      -DATA
 * -D13         ->      -CLK
 * -GND         ->      -GND
 */

DMD dmd(2, 1); //untuk mengatur jumlah panel yang kamu pakai

double times[sizeof(TimeName) / sizeof(char*)];

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}






void setup()
{

  pinMode(2, OUTPUT);//----pin 2 untuk alarm adzan maghrib ---hubungkan dengan buzzer

  langkah = 0;
  rtc.begin();
  //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above (ini untuk setting jam)
  Wire.begin();
  Timer1.initialize( 500 );
  Timer1.attachInterrupt( ScanDMD );
  dmd.clearScreen( true );

  pinMode(tombol_bOK, INPUT_PULLUP);                                 // Mode Pin Sebagai Input dengan Pull Up Internal
  pinMode(Jam_bUP, INPUT_PULLUP);                                    // Mode Pin Sebagai Input dengan Pull Up Internal
  pinMode(Jam_bDN, INPUT_PULLUP);                                    // Mode Pin Sebagai Input dengan Pull Up Internal

  attachInterrupt(0, Setting, FALLING);
}

////////////////////////////////// Fungsi Looping/Pengulangan ///////////////////////////////////////
void loop() {
  if (Waduh_Setting) {

    MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual
  }
  mulai();
}


/////////////////////////////////// Interrupt Button Setting ////////////////////////////////////////
void Setting() {
  Waduh_Setting = true;                                                // Aktifkan Menu Setting
}

////////////////////////////////////// Fungsi Menu Setting //////////
void MenuSetting() {
  DateTime now = rtc.now();
  String Teks;
  int tgl = now.date();         //Tanggal
  int bln = now.month();        //Bulan
  int thn = now.year();         //Tahun
  int hours, minutes;        //jam dan menit jawdwal sholat
  int jame = now.hour();     //jam
  int menit_e = now.minute(); //menit
  int harin_e = harin_e;  //hari
  detachInterrupt(0);// Matikan interrupt "0"
  delay(500);

  /////////////////////////////////////// Setting Jam & Menit ///////

  int kasus = 0;
setTimes:                                                           // Setting Times (Jam & Menit)
  if (digitalRead(Jam_bUP) == LOW ) {                               // Tombol Setting Menit
    delay(10); menit_e++;                                        // Delay Tombol & Tambah Nilai Menit
    if (menit_e >= 60) {
      menit_e = 0; // Batas Nilai Menit Lebih dari 59 Kembali ke 0
    }
    while (digitalRead(Jam_bUP) == LOW) {}                         // Kunci Tombol Setting Menit Sampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW) {                               // Tombol Setting Menit
    delay(10); jame++;                                          // Delay Tombol & Tambah Nilai Jam
    if (jame >= 24) {
      jame = 0; // Batas Nilai Jam Lebih dari 23 Kembali ke 0
    }
    while (digitalRead(Jam_bDN) == LOW) {}                         // Kunci Tombol Setting Jam Sampai Lepas Tombol
  }



setTgl:                                                           // Setting tanggal
  if (digitalRead(Jam_bUP) == LOW ) {                               // Tombol Setting tanggal
    delay(10); tgl++;
    if (tgl >= 32) {
      tgl = 1; // Batas Nilai tgl Lebih dari 31 Kembali ke 1
    }
    while (digitalRead(Jam_bUP) == LOW) {}                         // Kunci Tombol Setting tgl Sampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW) {                               // Tombol Setting bln
    delay(10); bln++;                                          // Delay Tombol & Tambah Nilai bln
    if (bln >= 13) {
      bln = 1; // Batas Nilai bln Lebih dari 12 Kembali ke 1
    }
    while (digitalRead(Jam_bDN) == LOW) {}                         // Kunci Tombol Setting blnSampai Lepas Tombol
  }




  // ///////=======setting thn==

setThn:                                                           // Setting thn
  if (digitalRead(Jam_bUP) == LOW ) {                               // Tombol Setting thn
    delay(10); thn++;                                        // Delay Tombol & Tambah Nilai thn
    if (thn >= 9999) {
      thn = 0; // Batas Nilai Menit Lebih dari 9999 Kembali ke 0
    }
    while (digitalRead(Jam_bUP) == LOW) {}                         // Kunci Tombol Setting thn Sampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW) {                               // Tombol Setting thn
    delay(10); thn--;                                          // Delay Tombol & Tambah Nilai thn
    if (thn <= 0) {
      thn = 0; // Batas Nilai kurang dari 0 Kembali ke 0
    }
    while (digitalRead(Jam_bDN) == LOW) {}                         // Kunci Tombol Setting thn Sampai Lepas Tombol
  }




  // =======setting Hari======
  //hitungan hari mulai dari 0,1,2,3,4,5,6 = senin, selasa, rabu, kamis, jumat, sabtu, minggu

setHari:                                                           // Setting Times hari
  if (digitalRead(Jam_bUP) == LOW ) {                               // Tombol Setting hari
    harin_e=harin_e;
    delay(10); 
    harin_e++;                                        // Delay Tombol & Tambah Nilai hari,
    if (harin_e >= 7) {
      harin_e = 0; // Batas Nilai Menit Lebih dari 6 Kembali ke 0
    }
    while (digitalRead(Jam_bUP) == LOW) {}                         // Kunci Tombol Setting hariSampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW) {                               // Tombol Setting hari
    harin_e=harin_e;
    delay(10); 
    harin_e--;                                          // Delay Tombol & Tambah Nilai hari
    if (harin_e <= 0) {
      harin_e = 0; // Batas Nilai kurang dari 0 Kembali ke 0
    }
    while (digitalRead(Jam_bDN) == LOW) {}                         // Kunci Tombol Setting hari Sampai Lepas Tombol
  }

  ///============Tombol OKE =======================
 

  if (digitalRead(tombol_bOK) == LOW) {                               // Tombol Selesai Setting

    kasus = kasus + 1;
    dmd.clearScreen(true);                                                                 // Hapus Tampilan P10
    DateTime dt(thn, bln, tgl, jame, menit_e, 0, harin_e);
    // DateTime dt(2017, 5, 4, 15, 11, 0, 4);
    rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above
    while (digitalRead(tombol_bOK) == LOW) {}
    delay(500);

    //  ============================================kasus-kasus===================

  } if (kasus == 1) {     //kasus seting jam

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(12, 0, "set-Jam", 7, 0);
    String xjam = Konversi(jame) + " : " + Konversi(menit_e) ;
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(9, 9, sjam, pjg, 0);
    delay (100);
    goto setTimes; //lari ke fungsi seting jam

  }
  if (kasus == 2) {  //kasus seting tanggal

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "set-Tgl/bl", 11, 0);
    String xjam = Konversi(tgl) + " : " + Konversi(bln) ;
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(9, 9, sjam, pjg, 0);
    delay (100);
    goto setTgl;//lari ke fungsi setting tanggal

  } if (kasus == 3) {  ///kasus seting tahun

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "set-Tahun", 9, 0);
    String xjam = Konversi(thn)  ;
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(9, 9, sjam, pjg, 0);
    delay (100);
    goto setThn; //lari ke fungsi setting tahun

  }

  if (kasus == 4) { //kasus seting hari

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "set-hari", 9, 0);
    String xjam = Konversi(harin_e)  ;
    if (harin_e == 0) {
      xjam = "SENIN";
      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(9, 9, sjam, pjg, 0);
    }  if (harin_e == 1) {
      xjam = "SELASA";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(9, 9, sjam, pjg, 0);
    }
     if (harin_e == 2) {
      xjam = "RABU";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(9, 9, sjam, pjg, 0);
    }if (harin_e == 3) {
      xjam = "KAMIS";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(9, 9, sjam, pjg, 0);
    }if (harin_e == 4) {
      xjam = "JUM'AT";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(9, 9, sjam, pjg, 0);
    }if (harin_e == 5) {
      xjam = "SABTU";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(9, 9, sjam, pjg, 0);
    }if (harin_e == 6) {
      xjam = "MINGGU";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(9, 9, sjam, pjg, 0);
    }
    delay (100);
    goto setHari; //lari ke seting hari

  }


  if (kasus == 5) {       //kasus wis Rampung
    dmd.clearScreen(true);
    goto endSetting;
    delay(500);

  }

endSetting:                                                         // Label End Setting
  attachInterrupt(0, Setting, FALLING);                               // Interrupt Tombol OK
  Waduh_Setting = false;

  kasus = 0;


}



//////////////////////==========================mulai program sholat JWS==============

int menite = 1; ///waktu iqomah lamanya
void mulai()
{
  long mulai = millis();


  while (millis() - mulai < 10000) {
    DateTime now = rtc.now();
    String Teks;


    ///================pengaturan koordinat-gunakan gps android untuk menentukan posisi anda (Malang)=================
    double lintang = -7.983908;  //Latitude
    double bujur = 112.621391;   //Longitude
    int Z = 7 ;                   //Zona Waktu GMT WIB biasanya 7


    int tgl = now.date();         //Tanggal
    int bln = now.month();        //Bulan
    int thn = now.year();         //Tahun
    int hours, minutes;     //jam sholat dan  menit sholat
    long mulai = millis();

    set_calc_method(ISNA);        //Methode perhitungan mengikuti ISNA
    set_asr_method(Shafii);       //Pendapat Imam Syafi'i
    set_high_lats_adjust_method(AngleBased);
    set_fajr_angle(20);           //Sudut Waktu Subuh
    set_isha_angle(18);           ////Sudut Waktu Isya
    get_prayer_times(thn, bln, tgl, lintang, bujur, Z, times);




    dmd.selectFont(SystemFont5x7);

    int menit_e = now.minute();
    int jame = now.hour();
    String xjam = Konversi(jame) + ":" + Konversi(menit_e) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 0, sjam, pjg, 0);

    String tanggal_ = Konversi(tgl) + "/" + Konversi(bln) + "/" + Konversi(thn) + " "; //tampilan tanggal
    int dowo = tanggal_.length() + 1;
    char tanggal_an[dowo];
    tanggal_.toCharArray(tanggal_an, dowo);
    dmd.drawString(2, 9, tanggal_an, dowo, 0);

    if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }


  }




  //----------------------------------------------====kode waktu adzan====------------------------
  //jam adzan Subuh----------------------------------------------------------
  int hours;
  int minutes;
  dmd.clearScreen( true );
  while (millis() - mulai < 11000) {
    ///////////////////////
   
    DateTime now = rtc.now();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 4, sjam, pjg, 0);
        ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }
    

    /////////////////////

    get_float_time_parts(times[0], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);
      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 9, "Subuh", 5, 0); //koordinat tampilan
      delay(150000);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);



          }
        }
      }
    }
  }





  //----------------------------------------------------------------------

  //jam adzan Dzuhur----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 20000) {
    ///////////////////////
    DateTime now = rtc.now();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 4, sjam, pjg, 0);

        ///////////////interupsi tombol menu-----------
    if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }

    /////////////////////
    get_float_time_parts(times[2], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);
      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(15, 9, "Sholat", 6, 0); //koordinat tampilan
      delay(150000);


      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }


  //----------------------------------------------------------------------

  //jam adzan Asar----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 31000) {
    ///////////////////////
    DateTime now = rtc.now();
    dmd.drawString(13, 4, (weekDay[now.dayOfWeek()]), 7, 0);   //tampilan hari
        ///////////////interupsi tombol menu-----------
if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }
    /////////////////////
    get_float_time_parts(times[3], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);
      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(19, 9, "Asar", 4, 0); //koordinat tampilan
      delay(150000);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }






  //----------------------------------------------------------------------

  //jam adzan Maghrib----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 40000) {
    ///////////////////////
    DateTime now = rtc.now();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 4, sjam, pjg, 0);

    ///////////////interupsi tombol menu-----------
if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }
    /////////////////////
    get_float_time_parts(times[4], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);
      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(15, 9, "Maghrib", 7, 0); //koordinat tampilan
      delay(150000);




      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }









  //jam adzan isya----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 50000) {
    ///////////////////////
    DateTime now = rtc.now();
    String hariku = Konversi(now.date()) + "/" + Konversi(now.month()) + "/" + Konversi(now.year()) + " "; //tampilan tanggal
    int dowo = hariku.length() + 1;
    char detikanku[dowo];
    hariku.toCharArray(detikanku, dowo);
    dmd.drawString(2, 4, detikanku, dowo, 0);
            ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }

    /////////////////////
    get_float_time_parts(times[6], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);
      digitalWrite(2, HIGH);//alarm sholat
      delay(1000);
      digitalWrite(2, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(15, 9, "Isya", 4, 0); //koordinat tampilan
      delay(150000);




      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }





  //----------------------------------------------------------------------



  //====================================runing teks======================

  String Teks;
  dmd.clearScreen(true);



  // Tampilkan Waktu Subuh
  int kecepatan;
  kecepatan = 40;                                   //kecepatan runing teks

  get_float_time_parts(times[0], hours, minutes);
  Teks = "SUBUH " + Konversi(hours) + ":" + Konversi(minutes);

  int pj = Teks.length() + 1;
  char tampil[pj];
  Teks.toCharArray(tampil, pj);
  dmd.drawMarquee(tampil, pj, 63, 4);

  long timer = millis();
  boolean ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();

        ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }

    }

  }

  //Tampilkan Waktu Dhuhur
  get_float_time_parts(times[2], hours, minutes);
  int suwene;
  int suwene_2;
  suwene = minutes;
  suwene_2 = hours;
  int sholatnya = 0;

  Teks = "DZUHUR " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
              ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }

    }


  }

  //Tampilkan Waktu Ashar
  get_float_time_parts(times[3], hours, minutes);
  Teks = "ASAR " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
              ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }
    }
  }

  //Tampilkan Waktu Magrib
  get_float_time_parts(times[4], hours, minutes);
  Teks = "MAGHRIB " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }
  //Tampilkan Waktu Isya
  get_float_time_parts(times[6], hours, minutes);
  Teks = "ISYA  " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
              ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }
    }
  }

  //Tampilkan Waktu Imsak, 10 Menit sebelum waktu subuh

  get_float_time_parts(times[0], hours, minutes);
  if (minutes < 11)
  {
    minutes = 60 - minutes;
    hours --;
  }
  else {
    minutes = minutes - 10 ;
  }

  Teks = "IMSAK " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
              ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }
    }
  }

  //Tampilkan PENGUMUMAN


  Teks = " MASJID AL IKHLAS SMK N 2 SRAGEN";

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
              ///////////////interupsi tombol menu-----------
 
 if (digitalRead(tombol_bOK) == LOW) {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual

    }
    }
  }




  //tampilkan tulisan diam-statis-nama masjid 5 detik

  Teks = "MASJID";                    //baris atas
  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(16, 0, tampil, pj, 0); //koordinat tampilan


  Teks = "AL-IKHLAS";                   //baris kedua
  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.selectFont(SystemFont5x7);
  dmd.drawString(5, 9, tampil, pj, 0); //koordinat tampilan
  delay (1000);
  dmd.clearScreen( true );

  //-----------------------------------------tampilkan tulisan waktu sholat statis--------------------------------------------------------------------

  dmd.clearScreen( true );


  int tunda;
  tunda = 0;

  while (millis() - mulai < 120000) {
    tunda = tunda + 1;
    DateTime now = rtc.now();
    int tgl = now.date();         //Tanggal
    int bln = now.month();        //Bulan
    int thn = now.year();         //Tahun
    int hours, minutes;
    long mulai = millis();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute())  ; //tampilan jam dan menit
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);

    dmd.drawString(0, 0, sjam, pjg, 0);//tampilan jam dan menit

    String det = Konversi(now.second()) + " ";  //detik
    int pjgn = det.length() + 1;
    char sjamm[pjgn];
    det.toCharArray(sjamm, pjgn);
    dmd.drawLine(  31,  0, 30, 150, GRAPHICS_NORMAL );
    dmd.drawString(10, 9, sjamm, pjgn, 0);     //tampilan detik

    delay(1000);


    if (tunda < 3) {

      get_float_time_parts(times[0], hours, minutes);
      Teks = "Subuh   ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);              //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan

    } else if (tunda < 6) {

      get_float_time_parts(times[2], hours, minutes);
      Teks = "Zuhur   ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);             //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan

    } else if (tunda < 9) {

      get_float_time_parts(times[3], hours, minutes);
      Teks = "Asar    ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan
    }  else if (tunda < 12) {

      get_float_time_parts(times[4], hours, minutes);
      Teks = "Mgrib    ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan
    }  else if (tunda < 15) {

      get_float_time_parts(times[6], hours, minutes);

      Teks = "Isya     ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan







    }  else if (tunda < 18) {

      get_float_time_parts(times[0], hours, minutes);
      if (minutes < 11)
      {
        minutes = 60 - minutes;
        hours --;
      }
      else {
        minutes = minutes - 10 ;
      }
      Teks = "Imsak   ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan
      delay(1000);
      tunda = 0;
    }
  }





  dmd.clearScreen( true );

}





String Konversi(int sInput) {
  if (sInput < 10)
  {
    return"0" + String(sInput);
  }
  else
  {
    return String(sInput);
  }
}
