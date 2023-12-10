#define MOTOR_1  25
#define MOTOR_2  26

#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include "misc.h"
#include "motor.h"
#include "credentials.h"

hw_timer_t *My_timer_1 = NULL;
hw_timer_t *My_timer_2 = NULL;

Motor motor_1(MOTOR_1);
Motor motor_2(MOTOR_2);

void IRAM_ATTR motor_1_switch();
void IRAM_ATTR motor_2_switch();

void SwitchOnMotor_1(int time);
void SwitchOnMotor_2(int time);

void clearTimer(hw_timer_t *timer);
