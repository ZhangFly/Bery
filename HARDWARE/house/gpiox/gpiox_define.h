#ifndef GPIO_DEFINE_H_
#define GPIO_DEFINE_H_

#define GPIO_CONFIG_OUTPUT_50M(_Pin_) {*(_Pin_).C1 = 0; *(_Pin_).C0 = 0; *(_Pin_).M1 = 1; *(_Pin_).M0 = 1;}
#define GPIO_CONFIG_INPUT_ANALOG(_Pin_) {*(_Pin_).C1 = 0; *(_Pin_).C0 = 0; *(_Pin_).M1 = 0; *(_Pin_).M0 = 0;}
#define GPIO_CONFIG_INPUT_FLOATING(_Pin_) {*(_Pin_).C1 = 0; *(_Pin_).C0 = 1; *(_Pin_).M1 = 0; *(_Pin_).M0 = 0;}

typedef struct _GPIO_Pin {
    u32 *R;
    u32 *W;
    u32 *C1;
    u32 *C0;
    u32 *M1;
    u32 *M0;
}GPIO_Pin;

typedef struct _GPIO_Group {
    GPIO_TypeDef *no;
    GPIO_Pin _0;
    GPIO_Pin _1;
    GPIO_Pin _2;
    GPIO_Pin _3;
    GPIO_Pin _4;
    GPIO_Pin _5;
    GPIO_Pin _6;
    GPIO_Pin _7;
    GPIO_Pin _8;
    GPIO_Pin _9;
    GPIO_Pin _10;
    GPIO_Pin _11;
    GPIO_Pin _12;
    GPIO_Pin _13;
    GPIO_Pin _14;
    GPIO_Pin _15;
    void (*enable)(void);
    void (*disable)(void);
}GPIO_Group;

#endif
