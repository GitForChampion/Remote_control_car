#include "unit_test_motor.h"
#include "bsp_motor.h"

void unit_test_motor_init()
{
	bsp_motor_init();
}

void unit_test_motor_run()
{
	bsp_motor1_pwm_set(20);   
	bsp_motor2_pwm_set(20);
	bsp_motor3_pwm_set(20);		
	bsp_motor4_pwm_set(20);
}
