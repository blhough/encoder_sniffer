
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
#include <elapsedMillis.h>



void printNumber( long n );
void controlBacklight( long pos );
void checkButtonPressAndFlipEncoder();


const size_t NUM_LONG_DIGITS = 10;
const byte INVALID_DIGIT = 10;
const byte BACKLIGHT_PIN = 20;
const byte BUTTON_PIN = 14;

long oldPos = 0; //previous encoder position
byte pos[10]; //current encoder position
bool isNegative = false; //if encoder position is negaitve
bool isEncoderFlip = false; // if the encoder counts are flipped;
bool isButtonReleased = true; // if the button is released
byte stopIndex = 0; //the index to stop printing on
elapsedMillis timer = 0; // timer for encoder settling
bool isBacklightOn = false; // if backlight is on


Encoder myEnc( 15, 16 );

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidTWI lcd( 0 );



void setup() 
{
  // set up the LCD's number of rows and columns: 
  lcd.begin( 16, 2 );
  
  // Print a message to the LCD.
  lcd.print( "Encoder Pos:" );

  pinMode(BACKLIGHT_PIN, OUTPUT); // initialize backlight pin for output
  pinMode(BUTTON_PIN, INPUT_PULLUP ); // initialize button pin for input with pullup
  delayMicroseconds( 100 ); //just to make sure everything is ready
}



void loop() 
{
  long pos = myEnc.read();

  if ( isEncoderFlip )
  {
    pos = -pos;
  }

  controlBacklight( pos );

  checkButtonPressAndFlipEncoder();

  printNumber( pos );

}



void printNumber( long n )
{
  if ( oldPos == n ) //no change needed
  {
    return;
  }

  oldPos = n;


  bool  signChange = false;

  if( ( n < 0 ) != isNegative ) //if sign change
  {
    isNegative = !isNegative; //flip sign
    signChange = true; // remember change
  }

  if ( isNegative ) //if n is negative, make it positve
  {             
    n = -n;
  }


  for ( size_t i = 0 ; i < NUM_LONG_DIGITS ; i++ ) // for each digit upto maximum number of digits
  {
    if (n == 0 ) // if all digits have been printed
    {
      if ( signChange || ( i != stopIndex) ) // if sign changed or number of digits changed
      {

        pos[ i ] = INVALID_DIGIT; //invalidates the digit to ensure it is printed next time it is needed
        lcd.setCursor( NUM_LONG_DIGITS - i  , 1 );

        if ( isNegative )
        {
          lcd.print('-');
        }
        else
        {
          lcd.print(' ');
        }
      }

      for ( size_t j = i + 1 ; j <= stopIndex ; j++ ) // if oldPos was longer, clear upto last digit of oldPos
      {
        pos[ j ] = INVALID_DIGIT; //invalidates the digit to ensure it is printed next time it is needed
        lcd.setCursor( NUM_LONG_DIGITS - j  , 1 );
        lcd.print(' ');
      }

      stopIndex = i; // remember where current position ends
      break; // break for loop
    }

    byte m = n % 10; //extract least significant digit

    if (pos[ i ] != m ) // if the digit has changed, print new digit
    {
      pos[ i ] = m;
      lcd.setCursor( NUM_LONG_DIGITS - i  , 1 );
      lcd.print(m);
    }

    n /= 10; // truncate least significant digit
  }
}



void controlBacklight( long pos )
{
  if ( oldPos == pos )
  {
    if ( !isBacklightOn && timer > 1000 )// if stable for over 1 second and not already on
    {
      digitalWrite( BACKLIGHT_PIN, LOW ); // turn on led
      isBacklightOn = true;
    }
  }
  else
  {
    if ( isBacklightOn )
    {
      digitalWrite( BACKLIGHT_PIN, HIGH ); 
      isBacklightOn = false;
    }
    
    timer = 0;
  }
}



void checkButtonPressAndFlipEncoder()
{

  if ( digitalRead( BUTTON_PIN ) )
  {
   if ( isButtonReleased )
   {
    isEncoderFlip = !isEncoderFlip;
    isButtonReleased = false;
   }
  }
  else
  {
    isButtonReleased = true;
  }
}