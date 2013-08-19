#include "stm32f10x.h"

#define STRIP_LENGTH 50

void strip_init();
void strip_write(uint16_t word);
void strip_refresh();

extern uint16_t strip_data[STRIP_LENGTH];

