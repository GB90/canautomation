#include "stm32f10x.h"

void can_init();
void can_tx(uint16_t addr,uint8_t dlc,uint8_t* data);