#include "Arduino.h"

class Motor {
    private:
        int pin;
        int time;
        bool auto_program_execution;
    public:
        Motor(int activationPin) {
            pin = activationPin;
            time = 1;
            pinMode(pin, OUTPUT);
            switchOff();
        }

        bool isActive() {
            return digitalRead(pin);
        }

        void switchOn() {
            digitalWrite(pin, LOW);
        }

        void switchOff() {
            digitalWrite(pin, HIGH);
        }

        void setTime(int new_time) {
            time = new_time;
        }

        int getTime() {
            return time;
        }

        bool isAuto() {
            return auto_program_execution;
        }

        void setAuto(bool auto_execution) {
            auto_program_execution = auto_execution;
        }
};