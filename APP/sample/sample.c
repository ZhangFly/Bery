#include "sample.h"
#include "net.h"
#include "sensor.h"
#include "hardware_factory.h"

#define SAMPLE_BUFFER_SIZE 5000

static Scan *scan;             
static Buffer *sample_buffer; 
static TIM *tim_4_sample;

static void _Sample_Tim_Task(void)
{
    if(scan->over)	
        return;
    scan->step(scan);
}

__inline static void _Scan_Init_Helper(void)
{
    Point point;
    ADC *adc;
    IDAC *idac;
    SPI *spi;
    
    if(!scan)
        scan = Sensor_Create_Scan();
    if(!sample_buffer)
        sample_buffer = Buffer_Create(SAMPLE_BUFFER_SIZE);
    
    point.x = 0;
	point.y = 0;
    scan->beg = point;
    scan->cur = point;
    
    point.x = 47;
    point.y = 47;
    scan->end = point; 

    adc = ((ADC*)Hardware_Factory_Get(ADC1))->init();
    spi = ((SPI*)Hardware_Factory_Get(SPI2))->init(SPI_Mode_Master);
    idac = ((IDAC*)Hardware_Factory_Get(IDAC1))->init(spi)->start()->setOutput(0.15);

    scan->bundle_adc = adc;
    scan->bundle_idac = idac;
    scan->buffer = sample_buffer;
    scan->mode = SCAN_MODE_X_FIRST;
}

__inline static void _Tim_4_Sample_Init_Helper(void)
{   
    if(!tim_4_sample)
        tim_4_sample = Hardware_Factory_Get(TIM2);
    
    tim_4_sample->init(1,3199);
    tim_4_sample->task = _Sample_Tim_Task;
    tim_4_sample->enable();
}

void Sample_Start(void)
{   	
    Sensor_Init();
    Net_Init(SPI_Mode_Slave);
    _Scan_Init_Helper();
    _Tim_4_Sample_Init_Helper();
}

void Sample_Stop(void)
{
    scan->stop(scan); 
    Hardware_Factory_Release(ADC1);
    Hardware_Factory_Release(IDAC1);
    Hardware_Factory_Release(TIM2);
    Hardware_Factory_Release(SPI2);
    tim_4_sample = 0;
    Sensor_Destroy_Scan(scan);
    scan = 0;
    Buffer_Destroy(sample_buffer);
    sample_buffer = 0;  
}

void Sample_Pause(void)
{
    tim_4_sample->disable();
}

void Sample_Resume(void)
{
    tim_4_sample->enable();
}

Buffer* Sample_Buffer_Get(void)
{
    if(!scan->over)
        return 0;
    
    scan->restart(scan);
    return sample_buffer;
}
