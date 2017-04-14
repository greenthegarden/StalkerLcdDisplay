#ifndef STALKERLCDDISPLAY_STALKERCONFIG_H_
#define STALKERLCDDISPLAY_STALKERCONFIG_H_


#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>


// Stalker v2.1 pin definitions

const int  BAT_READ   = A7; // Allows the battery voltage to be read via Analog Pin 7
const int  CH_READ    = 6;  // Digital Pin 6 remains low (logic 0) while the battery is being charged
const int  OK_READ    = 7;  // Digital Pin 7 goes low (logic 0) after the battery has been charged
const int  STATUS_LED = 8;
const int  RF_CTS     = 9;
const int  RF_RTS     = A0;

// (the following three are predefined)
// #define MOSI        11
// #define MISO        12
// #define SCK         13


// initialisation of sensor objects
RtcDS3231<TwoWire> Rtc(Wire); //Create the DS3231 object


void rfpins_init() {
  // configure CTS and RTS pins as inputs
  pinMode(RF_CTS, INPUT);
  pinMode(RF_RTS, INPUT);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

#if DEBUG_LEVEL > 0
void printDateTime(const RtcDateTime& dt) {
  char datestring[20];
  snprintf_P(datestring,
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
  DEBUG_LOG(1, datestring);
}
#endif

byte rtcinit() {
  Rtc.Begin();

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  DEBUG_LOG(1, "Comilation date:");
  printDateTime(compiled);

  if (!Rtc.IsDateTimeValid()) {
    // Common Cuases:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing
    DEBUG_LOG(1, "RTC lost confidence in the DateTime!");
    // following line sets the RTC to the date & time this sketch was compiled
    // it will also reset the valid flag internally unless the Rtc device is
    // having an issue
    Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning()) {
    DEBUG_LOG(1, "RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();

  if (now < compiled) {
    DEBUG_LOG(1, "RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
    return 0;
  }
  else if (now > compiled) {
    DEBUG_LOG(1, "RTC is newer than compile time. (this is expected)");
    return 1;
  }
  else if (now == compiled) {
    DEBUG_LOG(1, "RTC is the same as compile time! (not expected but all is fine)");
    return 0;
  }
  return 0;
}


#endif  /* STALKERLCDDISPLAY_STALKERCONFIG_H_ */
