#ifndef GPIO_TYPEDEF_H_
#define GPIO_TYPEDEF_H_	

#include "stm32f10x.h"

typedef struct _GPIO_Group{
	u32 _0;
	u32 _1;
	u32 _2;
	u32 _3;
	u32 _4;
	u32 _5;
	u32 _6;
	u32 _7;
	u32 _8;
	u32 _9;
	u32 _10;
	u32 _11;
	u32 _12;
	u32 _13;
	u32 _14;
	u32 _15;
}GPIO_Group;

struct _GPIO{
	GPIO_Group A;
	GPIO_Group B;
	GPIO_Group C;
	GPIO_Group D;
};

extern struct _GPIO GPIO;

#endif /* GPIO_TYPEDEF_H_ */