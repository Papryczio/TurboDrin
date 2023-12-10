

#include "main.h"

void setup() {
    Serial.begin(9600);

    // Blynk configuration
    WiFi.begin(SSID, PASSWORD);
    int wifi_ctr = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected"); 
    Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);
}

void loop() {
    Blynk.run();
}

// Blynk functions

// Pump #1
BLYNK_WRITE(V0) {
    if (param.asInt()) {
        SwitchOnMotor_1(motor_1.getTime());
    }
}

// Pump #2
BLYNK_WRITE(V3) {
    if (param.asInt()) {
        SwitchOnMotor_2(motor_2.getTime());
    }
}

// Liquid #2 (ml)
BLYNK_WRITE(V1) {
    motor_2.setTime(param.asInt());
    Serial.println(motor_2.getTime());
}

// Liquid #1 (ml)
BLYNK_WRITE(V2) {
    motor_1.setTime(param.asInt());
    Serial.println(motor_1.getTime());
}

// Timers and interrupts
    // Motor #1
void IRAM_ATTR motor_1_switch() {
    motor_1.switchOff();
    Serial.println("OFF");
    clearTimer(My_timer_1);
}

    // Motor #2
void IRAM_ATTR motor_2_switch() {
    motor_2.switchOff();
    clearTimer(My_timer_1);
}

void SwitchOnMotor_1(int time) {
    My_timer_1 = timerBegin(0, 80, true);
    timerAttachInterrupt(My_timer_1, &motor_1_switch, true);
    time = secondsToMicroSeconds(time);
    timerAlarmWrite(My_timer_1, time, false);
    timerAlarmEnable(My_timer_1);
    motor_1.switchOn();
}

void SwitchOnMotor_2(int time) {
    My_timer_2 = timerBegin(1, 80, true);
    timerAttachInterrupt(My_timer_2, &motor_2_switch, true);
    time = secondsToMicroSeconds(time);
    timerAlarmWrite(My_timer_2, time, true);
    timerAlarmEnable(My_timer_2);
    motor_2.switchOn();
}

void clearTimer(hw_timer_t *timer) {
    timerDetachInterrupt(timer);
    timerEnd(timer);
}