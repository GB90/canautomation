#include "led_strip_driver.h"

#include "stm32f10x.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_tim.h"

static TIM_OCInitTypeDef TIM_OCInitStructure;

void strip_init()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Time Base configuration */
  //TIM_TimeBaseStructure.TIM_Prescaler = 15;
  TIM_TimeBaseStructure.TIM_Prescaler = 60;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 254;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
 
  strip_color(0,0,0);
  
  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  
}

void strip_color(uint8_t red,uint8_t green,uint8_t blue)
{
  TIM_OCInitStructure.TIM_Pulse = blue;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = green;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = red;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
}
