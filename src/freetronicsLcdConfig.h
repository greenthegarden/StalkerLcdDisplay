#ifndef STALKERLCDDISPLAY_FREETRONICSLCDCONFIG_H_
#define STALKERLCDDISPLAY_FREETRONICSLCDCONFIG_H_

/*
  Freetronics LCD & Keypad Shield:

    http://www.freetronics.com/products/lcd-keypad-shield

  Pins used by LCD & Keypad Shield:

    A0: Buttons, analog input from voltage ladder
    D4: LCD bit 4
    D5: LCD bit 5
    D6: LCD bit 6
    D7: LCD bit 7
    D8: LCD RS
    D9: LCD E
    D3: LCD Backlight (high = on, also has pullup high so default is on)

  ADC voltages for the 5 buttons on analog input pin A0:

    RIGHT:  0.00V :   0 @ 8bit ;   0 @ 10 bit
    UP:     0.71V :  36 @ 8bit ; 145 @ 10 bit
    DOWN:   1.61V :  82 @ 8bit ; 329 @ 10 bit
    LEFT:   2.47V : 126 @ 8bit ; 505 @ 10 bit
    SELECT: 3.62V : 185 @ 8bit ; 741 @ 10 bit

  */


#include <Wire.h>
#include <LiquidCrystal.h>


byte backlightOn = false;

/*--------------------------------------------------------------------------------------
  Defines
--------------------------------------------------------------------------------------*/
// Pins in use
#define BUTTON_ADC_PIN           A0  // A0 is the button ADC input
#define LCD_BACKLIGHT_PIN         3  // D3 controls LCD backlight

// ADC readings expected for the 5 buttons on the ADC input
#define RIGHT_10BIT_ADC           0  // right
#define UP_10BIT_ADC            145  // up
#define DOWN_10BIT_ADC          329  // down
#define LEFT_10BIT_ADC          505  // left
#define SELECT_10BIT_ADC        741  // right
#define BUTTONHYSTERESIS         10  // hysteresis for valid button sensing window

//return values for ReadButtons()
#define BUTTON_NONE               0  //
#define BUTTON_RIGHT              1  //
#define BUTTON_UP                 2  //
#define BUTTON_DOWN               3  //
#define BUTTON_LEFT               4  //
#define BUTTON_SELECT             5  //

//some example macros with friendly labels for LCD backlight/pin control, tested and can be swapped into the example code as you like
#define LCD_BACKLIGHT_OFF()     { digitalWrite( LCD_BACKLIGHT_PIN, LOW ); backlightOn = 0; }
#define LCD_BACKLIGHT_ON()      { digitalWrite( LCD_BACKLIGHT_PIN, HIGH ); backlightOn = 1; backlightOnMillis = millis(); }
#define LCD_BACKLIGHT(state)    { if( state ){digitalWrite( LCD_BACKLIGHT_PIN, HIGH );}else{digitalWrite( LCD_BACKLIGHT_PIN, LOW );} }

/*--------------------------------------------------------------------------------------
  Variables
--------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events


// initialize the lcd library with the numbers of the interface pins
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7, BACKLIGHTPIN);
LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );  //Pins for the Freetronics 16x2 LCD shield. LCD: ( RS, E, LCD-D4, LCD-D5, LCD-D6, LCD-D7 )


void lcdinit() {
  //button adc input
  pinMode( BUTTON_ADC_PIN, INPUT );         //ensure A0 is an input
  digitalWrite( BUTTON_ADC_PIN, LOW );      //ensure pullup is off on A0
  //lcd backlight control
  pinMode( LCD_BACKLIGHT_PIN, OUTPUT );     //D3 is an output
  LCD_BACKLIGHT_ON();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

/*--------------------------------------------------------------------------------------
  ReadButtons()
  Detect the button pressed and return the value
  Uses global values buttonWas, buttonJustPressed, buttonJustReleased.
--------------------------------------------------------------------------------------*/
byte ReadButtons()
{
   byte button = BUTTON_NONE;   // return no button pressed if the below checks don't write to btn

   //read the button ADC pin voltage
   unsigned int buttonVoltage = analogRead( BUTTON_ADC_PIN );
   //sense if the voltage falls within valid voltage windows
   if( buttonVoltage < ( RIGHT_10BIT_ADC + BUTTONHYSTERESIS ) ) {
      button = BUTTON_RIGHT;
   } else if(   buttonVoltage >= ( UP_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( UP_10BIT_ADC + BUTTONHYSTERESIS ) ) {
      button = BUTTON_UP;
   } else if(   buttonVoltage >= ( DOWN_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( DOWN_10BIT_ADC + BUTTONHYSTERESIS ) ) {
      button = BUTTON_DOWN;
   } else if(   buttonVoltage >= ( LEFT_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( LEFT_10BIT_ADC + BUTTONHYSTERESIS ) ) {
      button = BUTTON_LEFT;
   } else if(   buttonVoltage >= ( SELECT_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( SELECT_10BIT_ADC + BUTTONHYSTERESIS ) ) {
      button = BUTTON_SELECT;
   }
   //handle button flags for just pressed and just released events
   if( (buttonWas == BUTTON_NONE ) && ( button != BUTTON_NONE ) ) {
      //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
      //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
      buttonJustPressed  = true;
      buttonJustReleased = false;
   }
   if( ( buttonWas != BUTTON_NONE ) && ( button == BUTTON_NONE ) ) {
      buttonJustPressed  = false;
      buttonJustReleased = true;
   }

   //save the latest button value, for change event detection next time round
   buttonWas = button;

   return( button );
}

void lcdPrintDateTime(const RtcDateTime& dt) {
  char datestring[16+1];
  snprintf_P(datestring,
            countof(datestring),
            PSTR("%02u:%02u %02u/%02u/%04u"),
            dt.Hour(),
            dt.Minute(),
            dt.Day(),
            dt.Month(),
            dt.Year()
            );
  lcd.print(datestring);
}

void updateDisplay() {
  lcd.home();
  lcdPrintDateTime(Rtc.GetDateTime());
  lcd.setCursor(0, 1);  // move cursor to second row
  lcd.print(" Temp:  ");
  lcd.print(Rtc.GetTemperature().AsFloat());
  lcd.print("oC");
}

void rtcError() {
  lcd.home();
  lcd.print("   RTC ERROR   ");
}


#endif  /* STALKERLCDDISPLAY_FREETRONICSLCDCONFIG_H_ */
