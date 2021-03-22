/*
 * UART transmission
 */

#include <stdint.h>
#include <stdbool.h>

#define UART_PACKAGE_LENGTH 2

 typedef struct {
     uint8_t servo;
     uint8_t angle;
 } Package;

class Uart {
    public:
        void setup();
        bool data_available();
        void write(uint8_t data);
        uint8_t read(Package * package);
    private:
        uint8_t buffer[UART_PACKAGE_LENGTH];
};
