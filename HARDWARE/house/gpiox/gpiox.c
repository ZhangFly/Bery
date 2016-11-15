#include "gpiox.h"
#include "stdlib.h"

#define BIT_WORD_ADDR(_bit_band_base_, _band_region_base_, _byte_addr_, _bit_number_) \
						((_bit_band_base_) + (((_byte_addr_) - (_band_region_base_)) << 5) + ((_bit_number_) << 2))

#define GPIO_BIT_WORD_ADDR(_byte_addr_, _bit_number_) BIT_WORD_ADDR(PERIPH_ALIAS_BASE,PERIPH_BASE,_byte_addr_,_bit_number_)

#define PERIPH_ALIAS_BASE 0x42000000

#define GPIO_CRL_ADDR(_GPIO_No_) (GPIO##_GPIO_No_##_BASE + 0x0)
#define GPIO_CRH_ADDR(_GPIO_No_) (GPIO##_GPIO_No_##_BASE + 0x4)
#define GPIO_IDR_ADDR(_GPIO_No_) (GPIO##_GPIO_No_##_BASE + 0x8)
#define GPIO_ODR_ADDR(_GPIO_No_) (GPIO##_GPIO_No_##_BASE + 0xC)

#define GPIO_CRL_BIT_WORD_ADDR(_GPIO_No_, _bit_number_) (u32 *)GPIO_BIT_WORD_ADDR(GPIO_CRL_ADDR(_GPIO_No_),_bit_number_)
#define GPIO_CRH_BIT_WORD_ADDR(_GPIO_No_, _bit_number_) (u32 *)GPIO_BIT_WORD_ADDR(GPIO_CRH_ADDR(_GPIO_No_),_bit_number_)
#define GPIO_IDR_BIT_WORD_ADDR(_GPIO_No_, _bit_number_) (u32 *)GPIO_BIT_WORD_ADDR(GPIO_IDR_ADDR(_GPIO_No_),_bit_number_)
#define GPIO_ODR_BIT_WORD_ADDR(_GPIO_No_, _bit_number_) (u32 *)GPIO_BIT_WORD_ADDR(GPIO_ODR_ADDR(_GPIO_No_),_bit_number_)

#define GPIO_GROUP_CREATE_HELPER(_GPIO_No_) 							\
		{GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 0); 						\
		 GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 1); 						\
		 GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 2); 						\
		 GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 3); 						\
		 GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 4); 						\
		 GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 5); 						\
		 GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 6); 						\
		 GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, 7); 						\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 8); 						\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 9); 						\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 10);   					\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 11);   					\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 12);   					\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 13);   					\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 14);   					\
		 GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, 15);   					\
		 INSTANCE(_GPIO_No_)##->enable = _GPIO##_GPIO_No_##_Enable;		\
		 INSTANCE(_GPIO_No_)##->disable = _GPIO##_GPIO_No_##_Disable;}

#define GPIO_PIN_L_CREATE_HELPER(_GPIO_No_, _Pin_No_) 														\
		{INSTANCE(_GPIO_No_)##->_##_Pin_No_##.R = GPIO_IDR_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_); 			\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.W = GPIO_ODR_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_); 			\
	     INSTANCE(_GPIO_No_)##->_##_Pin_No_##.C1 = GPIO_CRL_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4 + 3); 	\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.C0 = GPIO_CRL_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4 + 2); 	\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.M1 = GPIO_CRL_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4 + 1); 	\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.M0 = GPIO_CRL_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4);}

#define GPIO_PIN_H_CREATE_HELPER(_GPIO_No_, _Pin_No_) 								\
		{INSTANCE(_GPIO_No_)##->_##_Pin_No_##.R = GPIO_IDR_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_); 			\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.W = GPIO_ODR_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_); 			\
	     INSTANCE(_GPIO_No_)##->_##_Pin_No_##.C1 = GPIO_CRH_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4 - 29); 	\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.C0 = GPIO_CRH_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4 - 30); 	\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.M1 = GPIO_CRH_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4 - 31);	\
		 INSTANCE(_GPIO_No_)##->_##_Pin_No_##.M0 = GPIO_CRH_BIT_WORD_ADDR(_GPIO_No_, _Pin_No_ * 4 - 32);}

#define GPIO_CREATE_INSTANCE(_GPIO_No_) 		 						\
		{if (!INSTANCE(_GPIO_No_)){									 	\
		 INSTANCE(_GPIO_No_) = (GPIO_Group*)malloc(sizeof(GPIO_Group)); \
		 INSTANCE(_GPIO_No_)##->no = GPIO##_GPIO_No_;					\
		 GPIO_GROUP_CREATE_HELPER(_GPIO_No_);}}									

#define GPIO_DESTROY_INSTANCE(_GPIO_No_)    \
		{free(gpio##_GPIO_No_);						\
		 gpio##_GPIO_No_ = 0;}

#define INSTANCE(_GPIO_No_) gpio##_GPIO_No_

static GPIO_Group *gpioA,*gpioB,*gpioC,*gpioD;

void _GPIOA_Enable(){
		RCC->APB2ENR |= 0x1 << 2;
}

void _GPIOB_Enable(){
		RCC->APB2ENR |= 0x1 << 3;
}

void _GPIOC_Enable(){
		RCC->APB2ENR |= 0x1 << 4;
}

void _GPIOD_Enable(){
		RCC->APB2ENR |= 0x1 << 5;
}

void _GPIOA_Disable(){
		RCC->APB2ENR &= ~(0x1 << 2);
}

void _GPIOB_Disable(){
		RCC->APB2ENR &= ~(0x1 << 3);
}

void _GPIOC_Disable(){
		RCC->APB2ENR &= ~(0x1 << 4);
}

void _GPIOD_Disable(){
		RCC->APB2ENR &= ~(0x1 << 5);
}

GPIO_Group* GPIOx_Create(GPIO_TypeDef* GPIOx)
{
	if(GPIOx == GPIOA)
	{
		GPIO_CREATE_INSTANCE(A);
		return gpioA;
	}
	if(GPIOx == GPIOB)
	{
		GPIO_CREATE_INSTANCE(B);
		return gpioB;
	}
	if(GPIOx == GPIOC)
	{
		GPIO_CREATE_INSTANCE(C);
		return gpioC;
	}
	if(GPIOx == GPIOD)
	{
		GPIO_CREATE_INSTANCE(D);
		return gpioD;
	}
	return 0;
}

void GPIOx_Destroy(GPIO_TypeDef* thiz)
{
	if (thiz == GPIOA)
	{
		GPIO_DESTROY_INSTANCE(A);
	}
	if (thiz == GPIOB)
	{
		GPIO_DESTROY_INSTANCE(B);
	}
	if (thiz == GPIOC)
	{
		GPIO_DESTROY_INSTANCE(C);
	}
	if (thiz == GPIOD)
	{
		GPIO_DESTROY_INSTANCE(D);
	}
}
