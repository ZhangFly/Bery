#include "adc1.h"
#include "stdlib.h"
#include "hardware_factory.h"

#define ADC1_IN_ENABLE() IOPC->enable() 
#define ADC1_IN          IOPC->_4
#define ADC1_INSTANCE    adc1

static ADC *ADC1_INSTANCE;

__inline static void _ADC1_GPIO_Init(){
    ADC1_IN_ENABLE();
    GPIO_CONFIG_INPUT_ANALOG(ADC1_IN);   
}

static ADC* _ADC1_Init(){
    ADC_InitTypeDef ADC_InitStructure; 
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);   
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                           

    _ADC1_GPIO_Init();

	ADC_DeInit(ADC1);                                                           

	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;	        
	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;	                        
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	                        
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;	    
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;	            
	ADC_InitStructure.ADC_NbrOfChannel       = 1;	                           
	ADC_Init(ADC1, &ADC_InitStructure);   

	ADC_Cmd(ADC1, ENABLE);	                                                  
	ADC_ResetCalibration(ADC1);	                                                
	while(ADC_GetResetCalibrationStatus(ADC1));	                               
	ADC_StartCalibration(ADC1);	                                                
	while(ADC_GetCalibrationStatus(ADC1));
    
    return ADC1_INSTANCE;
}			  

static void _ADC1_DeInit(){
    ADC_DeInit(ADC1);
};

static u16 _ADC1_Read()   
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);	          			    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                                
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));                            
    return ADC_GetConversionValue(ADC1);
}

ADC* ADC1_Create(void){   
    if(!ADC1_INSTANCE)
    {
        ADC1_INSTANCE = (ADC*)malloc(sizeof(ADC));
        ADC1_INSTANCE->no = ADC1;
        ADC1_INSTANCE->init = _ADC1_Init;
        ADC1_INSTANCE->deInit = _ADC1_DeInit;
        ADC1_INSTANCE->read = _ADC1_Read;
    }
    return ADC1_INSTANCE;
}  
    
void ADC1_Destroy(void){
    _ADC1_DeInit();
    free(ADC1_INSTANCE);
    ADC1_INSTANCE = 0; 
}
