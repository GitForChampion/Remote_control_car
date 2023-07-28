#include "unit_test_sensor.h"
#include "bsp_sensor_gpio.h"

/*
void unit_test_adc_init()
{
	bsp_sensor_adc_init();
}

uint32_t sensor_adc[2] = {0};
void unit_test_adc_run()
{
	sensor_adc[0] = bsp_sensor_adc_get(0);
	sensor_adc[1] = bsp_sensor_adc_get(1);
}
*/

void unit_test_sensor_gpio_init()
{
	bsp_sensor_gpio_init();
}

uint8_t sensor_sta[4]={0};
void unit_test_sensor_gpio_run()
{
	sensor_sta[0] = bsp_sensor0_get();
	sensor_sta[1] = bsp_sensor1_get();
	sensor_sta[2] = bsp_sensor2_get();
	sensor_sta[3] = bsp_sensor3_get();
}
