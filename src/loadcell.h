#include "Arduino.h"
#include "HX711.h"

#define CALIBRATION_FACTOR -4500

// NodeMCU
// const int LOADCELL_DOUT_PIN = 22;
// const int LOADCELL_SCK_PIN = 23;

// Wemos Lolin32
const int LOADCELL_DOUT_PIN = 21;
const int LOADCELL_SCK_PIN = 22;

HX711 scale;

bool glassDetected = false;
float glassWeight = 0;