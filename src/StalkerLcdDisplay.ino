#include <Arduino.h>

/*
  To compile Set Tools/Board in the Arduino IDE to
  Board: "Arduino Pro or Pro Mini"
  Processor: ATmega328 (3.3V 8MHz)"
*/


#include "config.h"


/*--------------------------------------------------------------------------------------
 setup()
 Called by the Arduino framework once, before the main loop begins
 --------------------------------------------------------------------------------------*/
void setup() {
#if DEBUG_LEVEL > 0
  Serial.begin(BAUD_RATE);
#endif

  lcdinit();

  if (rtcinit()) {
    updateDisplay();
  } else {
    rtcError();
    while(1) {}
  }
}
/*--------------------------------------------------------------------------------------
 end setup()
 --------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------
 loop()
 Arduino main loop
 --------------------------------------------------------------------------------------*/
void loop()
{
  unsigned long currentMillis = millis();

  //get the latest button pressed, also the buttonJustPressed, buttonJustReleased flags
  byte button = ReadButtons();

  switch( button )
  {
    case BUTTON_NONE:
    {
       break;
    }
    case BUTTON_RIGHT:
    {
      LCD_BACKLIGHT_ON();
      break;
    }
    case BUTTON_UP:
    {
      LCD_BACKLIGHT_ON();
      break;
    }
    case BUTTON_DOWN:
    {
      LCD_BACKLIGHT_ON();
      break;
    }
    case BUTTON_LEFT:
    {
      LCD_BACKLIGHT_ON();
      break;
    }
    case BUTTON_SELECT:
    {
      LCD_BACKLIGHT_ON();
      break;
    }
    default:
    {
      break;
    }
  }

  if (backlightOn && (currentMillis - backlightOnMillis >= BACKLIGHT_DURATION)) {
    LCD_BACKLIGHT_OFF();
  }

  if (currentMillis - previousMillis >= UPDATE_INTERVAL) {
    previousMillis = currentMillis;
    if (!Rtc.IsDateTimeValid()) {
      // Common Cuases:
      //    1) the battery on the device is low or even missing and the power line was disconnected
      rtcError();
    } else {
      updateDisplay();
    }
  }
}
/*--------------------------------------------------------------------------------------
 end loop()
 --------------------------------------------------------------------------------------*/
