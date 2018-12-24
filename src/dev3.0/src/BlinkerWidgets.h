#ifndef BLINKER_WIDGETS_H
#define BLINKER_WIDGETS_H

#if defined(BLINKER_MQTT)
    #include "Functions/BlinkerBridge.h"
#endif

#include "Functions/BlinkerButton.h"
#include "Functions/BlinkerJoystick.h"
#include "Functions/BlinkerNumber.h"
#include "Functions/BlinkerRGB.h"
#if defined(BLINKER_BLE)
    #include "Functions/BlinkerSlider.h"
#endif
#include "Functions/BlinkerSwitch.h"
#include "Functions/BlinkerText.h"

#if defined(BLINKER_ALIGENIE)
    #include "Functions/BlinkerAliGenie.h"

    BLINKERALIGENIE BlinkerAliGenie;    
#endif

#if defined(BLINKER_DUEROS)
    #include "Functions/BlinkerDuerOS.h"

    BLINKERDUEROS BlinkerDuerOS;    
#endif

#if defined(BLINKER_BLE) || defined(BLINKER_WIFI) || \
    defined(BLINKER_MQTT) || defined(BLINKER_PRO) || \
    defined(BLINKER_NBIOT)

    BlinkerSwitch BUILTIN_SWITCH;
#endif

#endif