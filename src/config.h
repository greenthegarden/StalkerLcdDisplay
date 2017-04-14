#ifndef STALKERLCDDISPLAY_CONFIG_H_
#define STALKERLCDDISPLAY_CONFIG_H_


#include "debug.h"


#if DEBUG_LEVEL > 0
const unsigned long BAUD_RATE          = 9600UL;
#endif

const unsigned long UPDATE_INTERVAL    = 1UL * 60UL * 1000UL;   // 1 minute = 1 * 60 * 1000 milliseconds

unsigned long previousMillis           = 0UL;

const unsigned long BACKLIGHT_DURATION = 10UL * 1000UL;  // 10 seconds = 10 * 1000 milliseconds

unsigned long backlightOnMillis        = 0UL;


#include "stalkerConfig.h"

#include "freetronicsLcdConfig.h"


#endif  /* STALKERLCDDISPLAY_CONFIG_H_ */
