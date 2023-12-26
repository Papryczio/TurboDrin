#include "Arduino.h"
#include "HX711.h"

#define CALIBRATION_FACTOR -4500

const int LOADCELL_DOUT_PIN = 22;
const int LOADCELL_SCK_PIN = 23;

HX711 scale;

bool glassDetected = false;
float glassWeight = 0;