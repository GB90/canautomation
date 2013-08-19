#include "led_strip_driver.h"

#include "stm32f10x.h"

uint16_t strip_data[STRIP_LENGTH];

void strip_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  GPIO_ResetBits(GPIOA,GPIO_Pin_9 | GPIO_Pin_10);
  
  for(uint8_t i = 0; i < STRIP_LENGTH; i++)
  {
    strip_data[i]=0x8000;
  }
}

void strip_write(uint16_t word)
{
  uint8_t i;
  for(i=0;i<16;i++)
  {
    GPIO_SetBits(GPIOA,GPIO_Pin_10);
    if(word & (1<<15))
    {
      GPIO_ResetBits(GPIOA,GPIO_Pin_9);  
    }
    else
    {
      GPIO_SetBits(GPIOA,GPIO_Pin_9);  
    }
    GPIO_ResetBits(GPIOA,GPIO_Pin_10);
    word=word<<1;
  }
}

void strip_refresh()
{
  // Send 32 0 bits to initialize transaction
  strip_write(0x0000);
  strip_write(0x0000);
			
  // Send out pixels
  for(uint8_t i = 0; i < STRIP_LENGTH; i++)
  {
    strip_write(strip_data[i]);
  }
	
  // Send out more than enough trailing 1s
  for(uint8_t i = (STRIP_LENGTH/16)+1; i > 0;i--)
  {
    strip_write(0xFFFF);
  }
}
