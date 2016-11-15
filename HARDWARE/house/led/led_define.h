#ifndef LED_DEFINE_H_
#define LED_DEFINE_H_ 

typedef u32 LED_Typedef;

#define LEDG ((LED_Typedef*)0x111222)
#define LEDR ((LED_Typedef*)0x111223)



typedef struct _LED{
    LED_Typedef *no;
    struct _LED* (*init)(void);
    void (*deInit)(void);
    void (*light)(void);
    void (*close)(void);
    void (*twinkle)(void);
}LED;

#endif /* LED_DEFINE_H_ */
