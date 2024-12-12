#ifndef PINS_H
#define PINS_H
#include "valve.h"
#define VALVE1_PIN  19  // Pin de la válvula (ejemplo: GPIO5)
#define VALVE2_PIN  18
#define VALVE3_PIN  5 
#define VALVE4_PIN  17
#define VALVE_COUNT 4

extern Valve valve1;
extern Valve valve2;
extern Valve valve3;
extern Valve valve4;
extern Valve* valves[4];
#endif