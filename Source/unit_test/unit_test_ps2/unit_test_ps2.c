#include "unit_test_ps2.h"
#include "ps2.h"

void unit_test_ps2_init()
{
	PS2_Init();
}

uint8_t unit_test_ps2_run()
{
	uint8_t state;
	state = PS2_DataKey();	
	
	return state;
}
