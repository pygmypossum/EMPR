#include "lpc17xx_i2c.h"
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"	
#include "mp1.c"

void main(void){
	serial_init();
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);
	
	I2C_Init(LPC_I2C1, 100000);
	I2C_Cmd(LPC_I2C1, ENABLE);

	int buffer = 0;
	uint8_t x = 0;
    I2C_M_SETUP_Type test;
	for(x = 0; x < 128; x++){
		




		test.sl_addr7bit = x;
		test.tx_count = 0;
		test.tx_data = &buffer;
		test.tx_length = 1;
		test.rx_length = 0;
		test.rx_data = NULL;

	


		if((I2C_MasterTransferData(LPC_I2C1, &test,I2C_TRANSFER_POLLING)) != 0){
			char temp[5];
			sprintf(temp, "%2X\n\r",x);
			write_usb_serial_blocking(temp,5);
					
		}
	}
}
