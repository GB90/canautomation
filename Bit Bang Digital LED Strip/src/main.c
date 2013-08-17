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
  strip_color(0,0,0);
  
  rgb_t color,old_color;
  rgba_t color2;
    
  while (1)
  {
    populate_strip(&color);
    if(color == old_color)
    {
      continue;
    }
    old_color=color;
    color2=unpack_rgb(color);
    strip_color(color2.r,color2.g,color2.b);
//    poll_serial();
//    strip_refresh();
  }
}

void rx_data(CanRxMsg msg)
{
  if(msg.DLC!=8)
  {
    return;
  }
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

    if (Receive_length  != 0)
    {
      for(i=0;i<Receive_length;i++)
      {
        c=Receive_Buffer[i];
        switch(c)
        {
        case '\r':
          if(receive_line())
          {
            while(!packet_sent);
            CDC_Send_DATA ("OK\r",4);
          }
          else
          {
            while(!packet_sent);
            CDC_Send_DATA ("FAIL\r",6);
          }
          Line_length=0;
          break;
        case '\n':
          break;
        default:
          Line_Buffer[Line_length++]=c;
        }
      }
      Receive_length = 0;
    }
  }
}

#define LINE_IS(x) (Line_length==sizeof(x)-1 && memcmp(Line_Buffer,(x),sizeof(x)-1)==0)

uint8_t receive_line()
{
  if(LINE_IS("red"))
  {
    strip_color(255,0,0);
    return 1;
  }
  if(LINE_IS("green"))
  {
    strip_color(0,255,0);
    return 1;
  }
  if(LINE_IS("blue"))
  {
    strip_color(0,0,255);
    return 1;
  }
  if(LINE_IS("black"))
  {
    strip_color(0,0,0);
    return 1;
  }
  if(LINE_IS("white"))
  {
    strip_color(10,10,10);
    return 1;
  }
  return 0;
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
