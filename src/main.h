// NodeMCU
// #define MOTOR_1 25
// #define MOTOR_2 26

// Wemos Lolin32
#define MOTOR_1 14
#define MOTOR_2 12

#include "Arduino.h"
#include "BluetoothSerial.h"
#include <ArduinoJson.h>

#include "misc.h"
#include "motor.h"
#include "loadcell.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
    #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

hw_timer_t *My_timer_1 = NULL;
hw_timer_t *My_timer_2 = NULL;
hw_timer_t *My_timer_3 = NULL;
hw_timer_t *My_timer_4 = NULL;

Motor motor_1(MOTOR_1);
Motor motor_2(MOTOR_2);

void IRAM_ATTR motor_1_switch();
void IRAM_ATTR motor_2_switch();

void SwitchOnMotor_1(double time);
void SwitchOnMotor_2(double time);

void IRAM_ATTR readLoadCellValue();
void attachLoadCellInterrupt();

String readBTline();
void parseRequest(String json);
void resolveRequest(StaticJsonDocument<256> jsonDocument);
void requestResponse(String action, double time_1, double time_2);
void requestResponse(String action);