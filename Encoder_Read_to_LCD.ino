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

Encoder myEnc(11, 12);

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidTWI lcd(0);

void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  lcd.print("Lin Encrd Pos:");
}
long oldPos=0;

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  
  long Pos=myEnc.read();
  
  if(oldPos!=Pos)
  lcd.print("                ");

  
  lcd.print(Pos);
  oldPos=Pos;


}

