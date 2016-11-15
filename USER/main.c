#include "stm32f10x.h"
#include "collect.h"
#include "sample.h"
#include "hardware_factory.h"

#define SYS_MODE_MASTER 1
#define SYS_MODE_SLAVE  0

u8 Sys_Mode;

__inline u8 mode_check()
{
    IOPC->enable();
    GPIO_CONFIG_INPUT_FLOATING(IOPC->_13);
    return *IOPC->_13.R;
}

__inline void _sys_start_as(u8 mode)
{
    if(mode == SYS_MODE_MASTER){  
        Sys_Mode = SYS_MODE_MASTER;
        Collect_Start();
    }else if(mode == SYS_MODE_SLAVE){
        Sys_Mode = SYS_MODE_SLAVE;
        Sample_Start();        
    }
}

__inline void boot_loader(void)
{
    mode_check() ? _sys_start_as(SYS_MODE_MASTER) : _sys_start_as(SYS_MODE_SLAVE);
}

int main(void)
{ 
 	boot_loader();
	//testCode();
	while(1);
}
