
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

void printNumberDec( long n );




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
long pos;

void loop() 
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor( 0, 1 );

  pos=myEnc.read();

  if ( oldPos != pos )
  {
    lcd.print("                ");
    lcd.setCursor( 0, 1 );
  }

  //printNumberFast( 488 );
  lcd.print( pos );

  oldPos = pos;
}




void inline divmod10_v2(uint32_t n,uint32_t *div,uint32_t *mod) {
  uint32_t p,q;
  /* Using 32.16 fixed point representation p.q */
  /* p.q = (n+1)/512 */
  q = (n&0xFFFF) + 1;
  p = (n>>16);
  /* p.q = 51*(n+1)/512 */
  q = 13107*q;
  p = 13107*p;
  /* p.q = (1+1/2^8+1/2^16+1/2^24)*51*(n+1)/512 */
  q = q + (q>>16) + (p&0xFFFF);
  p = p + (p>>16) + (q>>16);
  /* divide by 2 */
  p = p>>1;
  *div = p;
  *mod = n-10*p;
}






void printNumberDec( long n )
{
        uint8_t buf[11], *p;
        uint32_t digit;
        uint32_t m = n;
  //uint32_t t1, t2, c3333=0x3333;

        p = buf + (sizeof(buf));
        do {
    divmod10_v2( m, &m, &digit);
    //divmod10_asm(n, digit, t1, t2, c3333);
                *--p = digit + '0';
        } while (n);


        lcd.print(*p);
}