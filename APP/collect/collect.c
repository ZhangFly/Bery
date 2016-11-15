#include "collect.h"
#include "transmit.h"
#include "net.h"
#include "buffer.h"
#include "hardware_factory.h"

#define COLLECT_BUFFER_SIZE 30000

static Conn *conn_2_MCU2;
static Conn *conn_2_MCU3;

static Conn *conn_2_MCU6;
static Conn *conn_2_MCU7;

static Transfer* trans_2_PC;
static TIM* tim_4_collect;
static Buffer *collect_buffer;

void _Collect_Tim_Task(void)
{     
	conn_2_MCU2->read(conn_2_MCU2, collect_buffer);
	if(!collect_buffer->empty(collect_buffer)) {
		trans_2_PC->send(trans_2_PC, collect_buffer);
		collect_buffer->reset(collect_buffer);
	}
	
	conn_2_MCU3->read(conn_2_MCU3, collect_buffer);
	if(!collect_buffer->empty(collect_buffer)) {
		trans_2_PC->send(trans_2_PC, collect_buffer);
		collect_buffer->reset(collect_buffer);
	}
	conn_2_MCU7->read(conn_2_MCU7, collect_buffer);
	if(!collect_buffer->empty(collect_buffer)) {
		trans_2_PC->send(trans_2_PC, collect_buffer);
		collect_buffer->reset(collect_buffer);
	}
    conn_2_MCU6->read(conn_2_MCU6, collect_buffer);
	if(!collect_buffer->empty(collect_buffer)) {
		trans_2_PC->send(trans_2_PC, collect_buffer);
		collect_buffer->reset(collect_buffer);
	}
	
//    if(collect_buffer->empty(collect_buffer)) return;
//    trans_2_PC->send(trans_2_PC, collect_buffer);
//	collect_buffer->reset(collect_buffer);
}


__inline static void _Tim_4_Collect_Init_Helper()
{
    tim_4_collect = Hardware_Factory_Get(TIM3);
    tim_4_collect->init(72, 31999);
    tim_4_collect->task = _Collect_Tim_Task;
}





__inline static void _Conn_2_MCU2_Init_Helper()
{
   
    conn_2_MCU2 = Net_Create_Conn(MCU2);
}

__inline static void _Conn_2_MCU3_Init_Helper()
{
   
    conn_2_MCU3 = Net_Create_Conn(MCU3);
}

__inline static void _Conn_2_MCU6_Init_Helper()
{
   
    conn_2_MCU6 = Net_Create_Conn(MCU6);
}


__inline static void _Conn_2_MCU7_Init_Helper()
{
    conn_2_MCU7 = Net_Create_Conn(MCU7);
}


__inline static void _Trans_2_PC_Init_Helper()
{
    Transmit_Init();
    trans_2_PC = Transmit_Create_Transfer();
}

void Collect_Start(void)
{
    collect_buffer = Buffer_Create(COLLECT_BUFFER_SIZE);
    _Trans_2_PC_Init_Helper();
	 Net_Init(SPI_Mode_Master);
	_Conn_2_MCU2_Init_Helper();
	_Conn_2_MCU3_Init_Helper();
    _Conn_2_MCU6_Init_Helper();
	_Conn_2_MCU7_Init_Helper();
    _Tim_4_Collect_Init_Helper();
    tim_4_collect->enable();
}

void Collect_Stop(void)
{
	 Net_Destroy_Conn(conn_2_MCU2);
	Net_Destroy_Conn(conn_2_MCU3);
    Net_Destroy_Conn(conn_2_MCU6);
	Net_Destroy_Conn(conn_2_MCU7);
    Transmit_Destroy_Transfer(trans_2_PC);
    Hardware_Factory_Release(TIM3);
    Buffer_Destroy(collect_buffer);
}

