/*
 * PWM servo controlling functions.
 */

#include <stdint.h>
#include "pwm.h"

class Servo {
    public:
        Servo(uint8_t id, Pwm *pwm);
        void set_angle(uint8_t angle);
        void max_angle();
        void min_angle();
    private:
        uint8_t id;
        Pwm * pwm;
};
