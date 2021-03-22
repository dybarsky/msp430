/*
 * Pulse width modulation controller
 */

#include <stdint.h>

class Pwm {
    public:
        Pwm(uint8_t i2c_address, uint8_t frequency);
        void setup(void);
        void set_pwm(uint8_t channel, uint16_t duty_time);
        uint8_t * read();

    private:
        uint8_t i2c_address;
        uint8_t frequency;
        uint8_t read_byte(uint8_t address);
        void write_byte(uint8_t address, uint8_t value);
        void set_frequency(float frequency);
};

