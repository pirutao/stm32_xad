#ifndef __USART_CONFIG_H
#define	__USART_CONFIG_H

#define TEST_USART								USART2
#define TEST_USART_CLS						RCC_APB1Periph_USART2
#define TEST_USART_BAUD						115200

#define TEST_USART_TXD_PIN				GPIO_Pin_2
#define TEST_USART_TXD_GPIO_PORT	GPIOA
#define TEST_USART_TXD_CLK				RCC_APB2Periph_GPIOA

#define TEST_USART_RXD_PIN				GPIO_Pin_3
#define TEST_USART_RXD_GPIO_PORT  GPIOA
#define TEST_USART_RXD_CLK				RCC_APB2Periph_GPIOA

#define USART_IRQ									USART2_IRQn

#endif /* __USART_CONFIG_H */
