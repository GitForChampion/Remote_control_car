#include "unit_test.h"
#include "unit_test_motor.h"
#include "unit_test_servo.h"
#include "unit_test_sensor.h"
#include "unit_test_ps2.h"

void unit_test_init()
{
	/* motor*/
	unit_test_motor_init();
	
	
	/*   ps2
	uint8_t key = 0;
	unit_test_ps2_init();
	*/
	
	/* max7219
	unit_test_max7219_init();
	*/
	
	/*  arc servo
	unit_test_arc_servo_init();
	*/
	
	/* led
	unit_test_led_init();
	*/
	
	/*  sensor 
	unit_test_sensor_gpio_init();
	*/
}

void unit_test_run()
{
	//unit_test_led_run();

	/* motor*/
	unit_test_motor_run();
	

	
	/*  arc servo
	unit_test_arc_servo_pwm_set();
	unit_test_arc_servo_run();
	*/
	
	/* sensor
	unit_test_sensor_gpio_run();
	*/

	/*  ps2   
	key = unit_test_ps2_run();
	if (key)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
	*/
	
	/*  max7219
	unit_test_max7219_run();
	*/
}
