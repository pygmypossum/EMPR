#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_uart.h"	
#include "lpc17xx_pinsel.h"
#include "serial.h"
#include <stdio.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0

int msTick = 0;
int x = 0;
int y = 0;
int width = 0;
bool flag = false;

void delay(void){
	msTick = 0;
	while (msTick < 100){
	}
}

void SysTick_Handler(void){
	msTick++;
	pwm_set(width);
	if(flag && msTick % 50 == 0){

		if(width == 1000){
			width = 0;
		} else {
			width++;
		}
	}

}                
                          
void wait_for_tick(void){
	int startVal = msTick;
	while(startVal == msTick){}
}

void serial_init(void)
{
	UART_CFG_Type UARTConfigStruct;			// UART Configuration structure variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable
	PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
	/*
	 * Initialize UART pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	// USB serial first
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
		
	/* Initialize UART Configuration parameter structure to default state:
	 * - Baudrate = 9600bps
	 * - 8 data bit
	 * - 1 Stop bit
	 * - None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);
	/* Initialize FIFOConfigStruct to default state:
	 * - FIFO_DMAMode = DISABLE
	 * - FIFO_Level = UART_FIFO_TRGLEV0
	 * - FIFO_ResetRxBuf = ENABLE
	 * - FIFO_ResetTxBuf = ENABLE
	 * - FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	// Built the basic structures, lets start the devices/
	// USB serial
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);		// Initialize UART0 peripheral with given to corresponding parameter
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);	// Initialize FIFO for UART0 peripheral
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);			// Enable UART Transmit
}

int write_usb_serial_blocking(char *buf,int length)
{width = 0;
		} else {
			width++;
		}
	return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}

void setup_DAC(){

	PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
	/*
	 * Initialize UART pin connect
	 */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);

	DAC_Init(LPC_DAC);
	delay();
}

void waveform(double amplitude, double frequency){
	double time = 0;
	while(time < 50000){
		double x = (time/10000)/(1/frequency) * 2 * 3.14159;
		double y = sin(x) * amplitude + 1.5;
		int scaled_y = (y / 3) * 1023;
		DAC_UpdateValue(LPC_DAC,scaled_y);
		time ++;
		wait_for_tick();
	}
}

void setup_ADC(void){

		PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
	/*
	 * Initialize UART pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	// USB serial first
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 24;
	PINSEL_ConfigPin(&PinCfg);

	ADC_Init(LPC_ADC, 200000); 	

	ADC_ChannelCmd(LPC_ADC,1,ENABLE); 
	ADC_BurstCmd(LPC_ADC,ENABLE);
	}


double read_voltage(){

	uint32_t voltage;
	voltage = ADC_ChannelGetData(LPC_ADC,1);
	double Vin = (double)voltage / 4096 * 3;
	char numb[7];
	sprintf(numb, "%5.3f\n\r", Vin);
	// Using the serial out slows down the loop 	
	// write_usb_serial_blocking(numb,7);
	return Vin;
}

void pwm_setup(void){

		PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
	/*
	 * Initialize UART pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	// USB serial first
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	LPC_PWM1->PR = 0x18;
	LPC_PWM1->MR0 = 1000;
	LPC_PWM1->MCR = 2;
	LPC_PWM1->LER = 1;
	LPC_PWM1->PCR = (1 << 9);
	LPC_PWM1->TCR = 1;

}

void pwm_set(int value){
	LPC_PWM1->MR1 = value;
	LPC_PWM1->LER = 2;
}

void main(void){
	serial_init();
	SysTick_Config(SystemCoreClock / 10000);
	// setup_ADC();
	// setup_DAC();
	// while(1){
	// 	int OutputVal = ADC_ChannelGetData(LPC_ADC,1)/4;
	// 	DAC_UpdateValue(LPC_DAC,OutputVal);
	// }

	pwm_setup();
	
	// waveform(10,10);
	// waveform(1.5,30);
	// waveform(0.5,30);
	// waveform(1,100); 	Analog 
	// waveform(1,5);
	// setup_ADC();


}

