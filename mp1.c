#include <lpc17xx_gpio.h>
#include "lpc17xx_uart.h"	
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"
#include <stdio.h>

int msTick = 0;

void delay(void){
	msTick = 0;
	while (msTick < 1000){
	}
}

void SysTick_Handler(void){
	msTick++;
}                               
 
// Read options
int read_usb_serial_none_blocking(char *buf,int length)
{
	return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

// Write options
int write_usb_serial_blocking(char *buf,int length)
{
	return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}

// init code for the USB serial line
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


void binary (void){
	int led;
	int x;
	int y;
	int convert;
	char decnum[10];
	char binnum[12] = "Bin: 0000\n\r";
	char hexnum[10];
	for(x = 0; x<16; x++){
		sprintf(decnum, "Dec: %2d\n\r", x);
		sprintf(hexnum, "Hex: %2x\n\r", x);
		write_usb_serial_blocking(decnum,10);
		write_usb_serial_blocking(hexnum,10);
		for(y = 0; y<4; y++){ 
			convert = (x >> y)& 1;
			if (convert == 1){
				switch(y){
					case 0: 
						led = 18;
						binnum[8] = '1';
						break;
					case 1:
						led = 20;
						binnum[7] = '1';
						break;
					case 2:
						led = 21;
						binnum[6] = '1';
						break;
					case 3:
						led = 23;
						binnum[5] = '1';
						break;
				}
				GPIO_SetValue(1, 1 <<led);
			}
		}
		write_usb_serial_blocking(binnum,12);
		binnum[8] = '0';
		binnum[7] = '0';
		binnum[6] = '0';
		binnum[5] = '0';
		delay();
		GPIO_ClearValue(1, 1 <<18);
		GPIO_ClearValue(1, 1 <<20);
		GPIO_ClearValue(1, 1 <<21);
		GPIO_ClearValue(1, 1 <<23);
	}
}
/*
void main (void) {
	SysTick_Config(SystemCoreClock / 1000); 

	GPIO_SetDir(1,1 <<18, 1);
	GPIO_SetDir(1,1 <<20, 1);
	GPIO_SetDir(1,1 <<21, 1);
	GPIO_SetDir(1,1 <<23, 1);
	

	serial_init();
	write_usb_serial_blocking("Starting count\n\r",17);
	binary();
	write_usb_serial_blocking("Finished count\n\r",17);
}
*/




