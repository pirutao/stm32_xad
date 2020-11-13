/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>
#include <usart.h>
#include "LD3320.h"
#include "tim_driver.h"


extern uint32_t timCounter;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
			if(gvr.uart1_rx.flag == 0)
			{
				gvr.uart1_rx.buf[gvr.uart1_rx.len++] = USART_ReceiveData(USART1);
			}
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);		
	} 
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{ 	
			USART_ReceiveData(USART1);
			gvr.uart1_rx.flag = 1;
		
			USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	} 
}


void USART2_IRQHandler(void)
{
	static u8 pingpong_flag = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	if(pingpong_flag == 0)
			{
				if(gvr.uart1_rx.flag == 0)
				{
					gvr.uart1_rx.buf[gvr.uart1_rx.len++] = USART_ReceiveData(USART2);
				}
			}
			else
			{
				if(gvr.uart1_rx1.flag == 0)
				{
					gvr.uart1_rx1.buf[gvr.uart1_rx1.len++] = USART_ReceiveData(USART2);
				}
			}
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);		
	} 
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{ 	
			USART_ReceiveData(USART2);
			if(pingpong_flag == 0)
			{
				pingpong_flag = 1;
				gvr.uart1_rx.flag = 1;
			}
			else
			{
				pingpong_flag = 0;
				gvr.uart1_rx1.flag = 1;
			}
		
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);
	} 
}

///中间层
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(LD3320IRQEXITLINE)!= RESET ) 
	{
		//ProcessInt(); 
		gvr.voice_alarm_flag = 1;
 		//printf("进入中断12\r\n");	
		EXTI_ClearFlag(LD3320IRQEXITLINE);
		EXTI_ClearITPendingBit(LD3320IRQEXITLINE);//清除LINE上的中断标志位  
	} 
}

void TIM2_IRQHandler(void)
{
 
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		timCounter++;
		//uart1_putc('A');
	}
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
