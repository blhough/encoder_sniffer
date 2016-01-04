
/*
 Demonstration sketch for Adafruit i2c/SPI LCD backpack
 using MCP23008 I2C expander
 ( http://www.ladyada.net/products/i2cspilcdbackpack/index.html )


 
  The circuit:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * CLK to Analog #5 (pin 19)
 * DAT to Analog #4 (pin 18)
*/

// include the library code:
#include <Wire.h>
#include <LiquidTWI.h>
#include <Encoder.h>

Encoder myEnc( 11, 12 );

void printNumber( long n );




// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidTWI lcd( 0 );

void setup() 
{
  // set up the LCD's number of rows and columns: 
  lcd.begin( 16, 2 );
  
  // Print a message to the LCD.
  lcd.print( "Lin Encrd Pos:" );
}

long oldPos = 0;
//long pos;
byte pos[10];
bool wasNegative = false;
byte stopPos = 0;

void loop() 
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor( 0, 1 );

  long pos=myEnc.read();

  // if ( oldPos != pos )
  // {
  //   lcd.print("                ");
  //   lcd.setCursor( 0, 1 );
  // }

  //printNumberFast( 488 );

  //lcd.print( pos );
  printNumber( pos );

  oldPos = pos;
}






void printNumber( long n )
{
  //byte count = 0;

  if( !( ( n < 0 ) == wasNegative ) )
  {
    lcd.setCursor( 0  , 1 );

    if ( wasNegative )
    {
      lcd.print('-');
    }
    else
    {
      lcd.print(' ');
    }

    wasNegative = !wasNegative;
  }

  if (n < 0)
  {             
    n = -n;
  }

  for ( int i = 0; i < 10; i++ )
  {
    byte m = n % 10;

    if (pos[ i ] != m )
    {
      pos[ i ] = m;
      lcd.setCursor( 10 - i  , 1 );
      lcd.print(m);
    }
    else
    {
      pos[ i ] = 10;
    }

    n /= 10;

    if (n == 0 )
    {
      byte stopTemp = i;

      while( i < stopPos )
      {
        i++;
        lcd.setCursor( 10 - i  , 1 );
        lcd.print(' ');
      }

      stopPos = stopTemp;
      break;
    }
  }


}