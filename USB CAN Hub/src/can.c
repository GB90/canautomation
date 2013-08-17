#include "can.h"
#include <string.h>

void can_init()
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

  // Configure CAN pins
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);   

  GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
  
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  
  /* CAN Baudrate = 125KBps*/
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler = 48;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
}

void can_tx(uint16_t addr,uint8_t dlc,uint8_t* data)
{
  CanTxMsg TxMessage;
  static volatile uint8_t status,i;
  uint8_t TransmitMailbox = 0;

  /* transmit */
  TxMessage.StdId=addr;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=dlc;
  memcpy(TxMessage.Data,data,dlc);

  TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);
  
  for(;;)
  {
    status=CAN_TransmitStatus(CAN1, TransmitMailbox);
    if(status!=CANTXPENDING)
    {break;}
  }
}

