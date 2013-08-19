//#include "hw_config.h"
//#include "usb_lib.h"
//#include "usb_desc.h"
//#include "usb_pwr.h"

#include <string.h>
#include "led_strip_driver.h"
#include "can.h"
#include "bespeckle.h"

extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
//extern __IO  uint32_t length ;

/*
uint8_t Line_Buffer[64];
uint32_t Line_length=0;

uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

void poll_serial();
uint8_t receive_line();
*/

void rx_data(CanRxMsg msg);

int main(void)
{
  /*
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
  */

  can_init();
  strip_init();

  for(;;)
  {
    populate_strip(strip_data);
    strip_refresh();
  }
}

void rx_data(CanRxMsg msg)
{
  if(msg.DLC!=8)
  {return;}
  message((canpacket_t*)msg.Data);
}

/*
void poll_serial()
{
  int i;
  char c;

  if (bDeviceState == CONFIGURED)
  {
    CDC_Receive_DATA();

    //CDC_Send_DATA ("OK\r",4);
  }
}
*/

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
