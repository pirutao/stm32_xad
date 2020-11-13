#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include "usart_config.h"

#define KEY_NUM_MAX 50
#define KEY_LEN_MAX 20
typedef struct{
	unsigned char num;
	unsigned char rsv[3];//no use
	unsigned char key[KEY_NUM_MAX][KEY_LEN_MAX];
}XAD_Voice_Key_S;

typedef enum{
	KEY_ALARM = 0x01,
	VOICE_ALARM = 0x02,
	SYS_REBOOT = 0x03,
}STM32_DATA_TYPE;

typedef enum{
	NORMAL_VOICE = 0x01,
	DUTY_VOICE = 0x02,
}STM32_CMD_TYPE;

#define BUF_LEN_MAX 1024

typedef struct{
	uint8_t buf[BUF_LEN_MAX];
	uint16_t len;
	uint8_t flag;
}uart_rec_s;

typedef struct{
	uint8_t head1;
	uint8_t head2;
	uint8_t type;
	uint8_t sn;
	uint8_t voice_str[KEY_LEN_MAX];
}uart_send_s;

typedef struct{
	uart_rec_s uart1_rx;
	uart_rec_s uart1_rx1;
	uart_send_s uart1_tx;
	XAD_Voice_Key_S voiceKey;
	XAD_Voice_Key_S duty_voiceKey;
	int voice_alarm_flag;
}global_var_s;

extern global_var_s gvr;
void USART_init(void);
void USART_SendBuf(char *buf,int nLen);
#endif /* __USART_H */
