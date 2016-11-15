#include "conn.h"
#include "stdlib.h"
#include "hardware_factory.h"
#include "net_master.h"

#define MCU0_NSS IOPC->_14 //PC.14
#define MCU1_NSS IOPC->_15 //PC.15
#define MCU2_NSS IOPC->_0  //PC.0
#define MCU3_NSS IOPC->_1  //PC.1
#define MCU4_NSS IOPC->_2  //PC.2
#define MCU5_NSS IOPC->_3  //PC.3
#define MCU6_NSS IOPA->_1  //PA.1
#define MCU7_NSS IOPA->_2  //PA.2

static void _Conn_Read(struct _Conn *thiz, Buffer *buffer)
{
     switch (thiz->target){
        case MCU0: 
            *MCU0_NSS.W = 0;
            Net_Master_Read(buffer);
		 	buffer->put(buffer, 0);			//xoffset
			buffer->put(buffer, 0);			//yoffset

            *MCU0_NSS.W = 1;
            break;
        case MCU1:
            *MCU1_NSS.W = 0;
            Net_Master_Read(buffer);
			buffer->put(buffer,48);
			buffer->put(buffer,0);

            *MCU1_NSS.W = 1;           
            break;
        case MCU2: 
            *MCU2_NSS.W = 0;
            Net_Master_Read(buffer);
			buffer->put(buffer,0);
			buffer->put(buffer,48);

            *MCU2_NSS.W = 1;
            break;
        case MCU3: 
            *MCU3_NSS.W = 0;
            Net_Master_Read(buffer);
			buffer->put(buffer,48);
			buffer->put(buffer,48);

            *MCU3_NSS.W = 1;
            break;
        case MCU4: 
            *MCU4_NSS.W = 0;
            Net_Master_Read(buffer);
			buffer->put(buffer,0);
			buffer->put(buffer,96);

            *MCU4_NSS.W = 1;
            break;
        case MCU5: 
            *MCU5_NSS.W = 0;
            Net_Master_Read(buffer);
			buffer->put(buffer,48);
			buffer->put(buffer,96);

            *MCU5_NSS.W = 1;
            break;
        case MCU6: 
            *MCU6_NSS.W = 0;
            Net_Master_Read(buffer);
			buffer->put(buffer,0);
			buffer->put(buffer,144);

            *MCU6_NSS.W = 1;
            break;
        case MCU7: 
            *MCU7_NSS.W = 0;
            Net_Master_Read(buffer);
			buffer->put(buffer,48);
			buffer->put(buffer,144);

            *MCU7_NSS.W = 1;
            break;
        default: ;
    } 
}

static void _Conn_Write(struct _Conn *thiz, Buffer *buffer)
{
    
}

Conn* Conn_Create(void)
{
    Conn *conn = (Conn*)malloc(sizeof(Conn));

    conn->read = _Conn_Read;
    conn->write = _Conn_Write;
    
    GPIO_CONFIG_OUTPUT_50M(MCU0_NSS);
    GPIO_CONFIG_OUTPUT_50M(MCU1_NSS);
    GPIO_CONFIG_OUTPUT_50M(MCU2_NSS);
    GPIO_CONFIG_OUTPUT_50M(MCU3_NSS);
    GPIO_CONFIG_OUTPUT_50M(MCU4_NSS);
    GPIO_CONFIG_OUTPUT_50M(MCU5_NSS);
    GPIO_CONFIG_OUTPUT_50M(MCU6_NSS);
    GPIO_CONFIG_OUTPUT_50M(MCU7_NSS);
    
    *MCU0_NSS.W = 1;
    *MCU1_NSS.W = 1;           
    *MCU2_NSS.W = 1;
    *MCU3_NSS.W = 1;
    *MCU4_NSS.W = 1;
    *MCU5_NSS.W = 1;
    *MCU6_NSS.W = 1;
    *MCU7_NSS.W = 1;

    return conn;
}

void Conn_Destroy(Conn* thiz)
{
    free(thiz);
}
