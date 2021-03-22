/*
 PWM servo controlling functions.
*/

void configure_servos();

void set_servo_angle(unsigned char servo_id, unsigned char angle);

int check_data_range(unsigned char servo_id, unsigned char angle);
