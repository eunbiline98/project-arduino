#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
/* Modify the pin number below to meet your board
*/
#define IN_LEFT    A0   // analog input for left channel
#define IN_RIGHT   A1  // analog input for right channel


/* Other minor configurable value
*/
#define T_REFRESH    100            // msec bar refresh rate
#define T_PEAKHOLD   3*T_REFRESH    // msec peak hold time before return

/* local variable
*/
byte  fill[6]={ 0x20,0x00,0x01,0x02,0x03,0xFF };      // character used to fill (0=empty  5=full)
byte  peak[7]={ 0x20,0x00,0x04,0x05,0x06,0x07,0x20 }; // character used to peak indicator
int   lmax[2];                                        // level max memory
int   dly[2];                                         // delay & speed for peak return

void  bar  ( int row,int lev )
{
  lcd.setCursor( 0,row );
  lcd.write( row ? 'R' : 'L' );
  for( int i=1 ; i<16 ; i++ )
  {
    int f=constrain( lev      -i*5,0,5 );
    int p=constrain( lmax[row]-i*5,0,6 );
    if( f )
      lcd.write( fill[ f ] );
    else
      lcd.write( peak[ p ] );
  }
  if( lev>lmax[row] )
  {
    lmax[row] = lev;
    dly[row]  = -(T_PEAKHOLD)/T_REFRESH;                // Starting delay value. Negative=peak don't move
  }
  else
  {
    if( dly[row]>0 )
      lmax[row] -= dly[row];

    if( lmax[row]<0 )
      lmax[row]=0;
    else
      dly[row]++;
  }
}

byte block[8][8]=
{
  { 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },  // define character for fill the bar
  { 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
  { 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
  { 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },

  { 0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08 },  // define character for peak level
  { 0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04 },
  { 0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02 },
  { 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 },
};

void  setup  ( void )
{
  lcd.begin();
  for( int i=0 ; i<8 ; i++ )
    lcd.createChar( i,block[i] );
}

long  lastT=0;

void  loop  ( void )
{
  if( millis()<lastT )
    return;
  lastT += T_REFRESH;   
  int anL = map( sqrt( analogRead( IN_LEFT  )*16 ),0,128,0,80 );  // sqrt to have non linear scale (better was log)
  int anR = map( sqrt( analogRead( IN_RIGHT )*16 ),0,128,0,80 );
  bar( 0,anL );
  bar( 1,anR );
}
