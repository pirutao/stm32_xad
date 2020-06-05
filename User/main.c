#include "stm32f10x.h"
#include "usart.h"
#include "LD3320.h"
#include <string.h>

/******************************************************************************/
static void key_alarm_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void audio_key_recv_loop()
{
	int i;
	int flag = 0;
	if(gvr.uart1_rx.flag)
	{
		for(i=0;i<gvr.uart1_rx.len;i++)
		{
			if(flag == 0)
			{
				if(gvr.uart1_rx.buf[i] == 0xeb)
				{
					flag = 1;
				}
			}
			else if(flag == 1)
			{
				if(gvr.uart1_rx.buf[i] == 0x90)
				{
					flag = 2;
				}
				else
				{
					flag = 0;
				}
			}
			else
			{
				switch(gvr.uart1_rx.buf[i])
				{
					case NORMAL_VOICE://nomal voice key
						memset(&gvr.voiceKey, 0, sizeof(gvr.voiceKey));
						memcpy((char *)&gvr.voiceKey, (char *)&gvr.uart1_rx.buf[i+1], gvr.uart1_rx.len-i-1);
						LD_reset();
						break;
					
					case DUTY_VOICE: //duty voice key
						memset(&gvr.duty_voiceKey, 0, sizeof(gvr.duty_voiceKey));
						memcpy((char *)&gvr.duty_voiceKey, (char *)&gvr.uart1_rx.buf[i+1], gvr.uart1_rx.len-i-1);
					  LD_reset();
						break;
					
					default: break;
				}
				memset(&gvr.uart1_rx,0,sizeof(gvr.uart1_rx));
				break;
			}
		}
	}
	
	
	flag = 0;
	if(gvr.uart1_rx1.flag)
	{
		for(i=0;i<gvr.uart1_rx1.len;i++)
		{
			if(flag == 0)
			{
				if(gvr.uart1_rx1.buf[i] == 0xeb)
				{
					flag = 1;
				}
			}
			else if(flag == 1)
			{
				if(gvr.uart1_rx1.buf[i] == 0x90)
				{
					flag = 2;
				}
				else
				{
					flag = 0;
				}
			}
			else
			{
				switch(gvr.uart1_rx1.buf[i])
				{
					case NORMAL_VOICE://nomal voice key
						memset(&gvr.voiceKey, 0, sizeof(gvr.voiceKey));
						memcpy((char *)&gvr.voiceKey, (char *)&gvr.uart1_rx1.buf[i+1], gvr.uart1_rx1.len-i-1);
						LD_reset();
						break;
					
					case DUTY_VOICE: //duty voice key
						memset(&gvr.duty_voiceKey, 0, sizeof(gvr.duty_voiceKey));
						memcpy((char *)&gvr.duty_voiceKey, (char *)&gvr.uart1_rx1.buf[i+1], gvr.uart1_rx1.len-i-1);
					  LD_reset();
						break;
					
					default: break;
				}
				memset(&gvr.uart1_rx1,0,sizeof(gvr.uart1_rx1));
				break;
			}
		}
	}
	//memset(&gvr.uart1_rx,0,sizeof(gvr.uart1_rx));
	//memset(&gvr.uart1_rx1,0,sizeof(gvr.uart1_rx1));
}


void key_alarm_loop(void)
{
	static uint32 key_alarm_cnt = 0;
	static uint32 doudong_cnt = 0;
	if(doudong_cnt)
	{
		doudong_cnt ++;
		if(doudong_cnt > 0x10000)//several ms
		{
			doudong_cnt = 0;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == RESET)
	{
		key_alarm_cnt++;
		if(key_alarm_cnt == 1 && doudong_cnt == 0 )
		{
			doudong_cnt ++;
			gvr.uart1_tx.head1 = 0xeb;
			gvr.uart1_tx.head2 = 0x90;
			gvr.uart1_tx.type = KEY_ALARM;
			USART_SendBuf((char *)&gvr.uart1_tx,3);
			gvr.uart1_tx.type = 0;
		}
	}
	else
	{
		key_alarm_cnt = 0;
	}
}

int main(void)
{
	USART_init();
	key_alarm_gpio_config();
	LD3320_init();
	//printf("\r\n 开始运行LD3320测试程序 \r\n");	
	//LD3320_main();				//LD3320执行函数
	for(;;)
	{
		if(gvr.voice_alarm_flag)
		{
			ProcessInt(); 
			gvr.voice_alarm_flag = 0;
		}
		audio_key_recv_loop();
		key_alarm_loop();
		LD3320_loop();
	}
}
/*********************************************END OF FILE**********************/
