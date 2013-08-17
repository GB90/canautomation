#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include <string.h>
#include "can.h"

extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
//extern __IO  uint32_t length ;

uint8_t Line_Buffer[64];
uint32_t Line_length=0;

uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

void serial_init();
void poll_serial();
void canout();

int state=0;
uint8_t can_frame[11];
uint16_t *addr=(uint16_t*)(&can_frame[0]);
uint8_t *dlc=&can_frame[2];
uint8_t *data=&can_frame[3];

int main(void)
{
  serial_init();
  can_init();
  
  while (1)
  {
    poll_serial();
  }
}

void serial_init()
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

  // Configure USART pins
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  
  USART_InitTypeDef USART_InitStructure;  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  USART_Cmd(USART2, ENABLE);
}

void serial_putchar(uint8_t c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void serial_putstr(char* str,uint16_t len)
{
  uint16_t i;
  for(i=0;i<len;i++)
  {serial_putchar(str[i]);}
}

void poll_serial()
{
  uint8_t c;
  if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
  {return;}

  c=USART_ReceiveData(USART2);

  if(state<3)
  {
    *dlc=0;
  }

  if(state<0)
  {
    if(c==0xFF)
    {
      state=0;
    }
  }
  else if(state>=0 && state<*dlc+3)
  {
    can_frame[state]=c;
    state++;
  }
  else if(state>=*dlc+3)
  {
    if(c==0xFF)
    {
      canout();
      serial_putstr("OK\r",3);
      state=0;
    }
    else
    {
      state=-1;

    }
  }
}

#define LINE_IS(x) (Line_length==sizeof(x)-1 && memcmp(Line_Buffer,(x),sizeof(x)-1)==0)

void canout()
{
  can_tx(*addr,*dlc,data);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
