#include "i2c.h"
#include "include/Board.h"

#define SDATA		AT91C_PIO_PA21
#define SCLK		AT91C_PIO_PA22

#define DELAY		asm volatile("nop; nop; nop; nop;");

void I2C_Init(void) {
#ifndef DEBUG_X86
	// Disable internal pull-ups - there are external ones on the IMU board
	AT91C_BASE_PIOA->PIO_PPUDR = SCLK; // disable pull up
	AT91C_BASE_PIOA->PIO_PPUDR = SDATA;	// disable pull up

	AT91C_BASE_PIOA->PIO_PER |= SDATA; // Allow PIO to control DATA
	AT91C_BASE_PIOA->PIO_SODR = SDATA; // Start with DATA high
	AT91C_BASE_PIOA->PIO_OER |= SDATA; // Enable output.

	AT91C_BASE_PIOA->PIO_PER |= SCLK; // Allow PIO to control CLK
	AT91C_BASE_PIOA->PIO_CODR = SCLK; // Start with CLK low
	AT91C_BASE_PIOA->PIO_OER |= SCLK; // Enable output.

	for (unsigned char i = 0; i < 4; i++) {
		I2C_stop();
	}
#endif
}

//------------------------------------------------------------------------------
// I2C Functions - Bit Banged
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 	Routine:	i2c_start
//	Inputs:		none
//	Outputs:	none
//	Purpose:	Sends I2C Start Transfer - State "B"
//------------------------------------------------------------------------------
void I2C_start (void) {
#ifndef DEBUG_X86
	AT91C_BASE_PIOA->PIO_SODR = SDATA | SCLK; // Set data & clock line high
	AT91C_BASE_PIOA->PIO_OER |= SDATA; // Enable output
	DELAY
	AT91C_BASE_PIOA->PIO_CODR = SDATA; // Set data line low
	DELAY
	AT91C_BASE_PIOA->PIO_CODR = SCLK; // Set clock line low
	DELAY
#endif
}

//------------------------------------------------------------------------------
// 	Routine:	i2c_stop
//	Inputs:		none
//	Outputs:	none
//	Purpose:	Sends I2C Stop Trasfer - State "C"
//------------------------------------------------------------------------------
void I2C_stop (void) {
#ifndef DEBUG_X86
	AT91C_BASE_PIOA->PIO_CODR = SDATA; // Set data line low
	AT91C_BASE_PIOA->PIO_OER |= SDATA; // Enable output
	DELAY
	AT91C_BASE_PIOA->PIO_SODR = SCLK; // Set clock line high
	DELAY
	AT91C_BASE_PIOA->PIO_SODR = SDATA; // Set data line high (STOP SIGNAL)
	DELAY
	AT91C_BASE_PIOA->PIO_CODR = SCLK; // Set clock line low
	DELAY

	AT91C_BASE_PIOA->PIO_ODR |= SDATA; // Disable output
#endif
}

//------------------------------------------------------------------------------
// 	Routine:	i2c_write
//	Inputs:		output byte
//	Outputs:	none
//	Purpose:	Writes data over the I2C bus
//------------------------------------------------------------------------------
void I2C_write (unsigned char output_data) {
#ifndef DEBUG_X86
	unsigned char index;

	AT91C_BASE_PIOA->PIO_OER |= SDATA; // Put data pin into write mode
	for(index = 0; index < 8; index++) { 	// Send 8 bits to the I2C Bus
    	DELAY
		if (output_data & 0x80) { // Output the data bit to the I2C Bus
			AT91C_BASE_PIOA->PIO_SODR = SDATA;
		} else {
			AT91C_BASE_PIOA->PIO_CODR = SDATA;
		}
      	output_data  <<= 1;            		// Shift the byte by one bit
      	DELAY
    	AT91C_BASE_PIOA->PIO_SODR = SCLK; // Clock the data into the I2C Bus
    	DELAY
    	AT91C_BASE_PIOA->PIO_CODR = SCLK;
	}

	AT91C_BASE_PIOA->PIO_ODR |= SDATA; // Put data pin into read mode
	DELAY
	AT91C_BASE_PIOA->PIO_SODR = SCLK; // Clock the ACK from the I2C Bus
	DELAY
	AT91C_BASE_PIOA->PIO_CODR = SCLK;
	DELAY
#endif
}

//------------------------------------------------------------------------------
// 	Routine:	i2c_read
//	Inputs:		none
//	Outputs:	input byte
//	Purpose:	Reads data from the I2C bus
//------------------------------------------------------------------------------
unsigned char I2C_read (unsigned char ack) {
#ifndef DEBUG_X86
	unsigned char index, input_data;

	AT91C_BASE_PIOA->PIO_ODR |= SDATA; // Put data pin into read mode

	input_data = 0x00;
	for(index = 0; index < 8; index++) {  	// Send 8 bits to the I2C Bus
      	DELAY
		input_data <<= 1;					// Shift the byte by one bit
		AT91C_BASE_PIOA->PIO_SODR = SCLK; // Clock the data into the I2C Bus
		DELAY
		if (AT91C_BASE_PIOA->PIO_PDSR & SDATA) {
			input_data |= 1; 		   		// Input the data from the I2C Bus
		}
      	AT91C_BASE_PIOA->PIO_CODR = SCLK;
	}

	if (ack == 1) {
		AT91C_BASE_PIOA->PIO_CODR = SDATA;
	} else {
		AT91C_BASE_PIOA->PIO_SODR = SDATA;
	}
	AT91C_BASE_PIOA->PIO_OER |= SDATA; // Put data pin into write mode
	DELAY
	AT91C_BASE_PIOA->PIO_SODR = SCLK; // Clock the ACK to the I2C Bus
	DELAY
	DELAY
	AT91C_BASE_PIOA->PIO_CODR = SCLK;
	DELAY

	return input_data;
#else
	return 0;
#endif
}

unsigned char I2C_ReadRegister(unsigned char address, unsigned char reg) {
	unsigned char data;

  	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write(address << 1);				// Send identifier I2C address - Write
   	I2C_write(reg);						// Send register address
   	I2C_stop();                   			// Send I2C Stop Transfer

   	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write((address << 1) | 0x01); 		// Send identifier I2C address - Read
	data = I2C_read(0);						// Read
	I2C_stop();
	return data;
}

void I2C_ReadMultipleRegisters(unsigned char address, unsigned char startreg, unsigned char* buffer, unsigned short length) {
  	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write(address << 1);				// Send identifier I2C address - Write
   	I2C_write(startreg);					// Send register address
   	I2C_stop();                   			// Send I2C Stop Transfer

   	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write((address << 1) | 0x01); 		// Send identifier I2C address - Read
   	for (int i = 0; i < length; i++) {
   		buffer[i] = I2C_read(i < length - 1 ? 1 : 0); // Read
   	}
   	I2C_stop();
}

void I2C_ReadMultipleRegisters2(unsigned char address, unsigned short startreg, unsigned char* buffer, unsigned short length) {
  	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write(address << 1);				// Send identifier I2C address - Write
   	I2C_write((startreg >> 8) & 0xFF);		// Send register address
   	I2C_write(startreg & 0xFF);				// Send register address
   	I2C_stop();                   			// Send I2C Stop Transfer

   	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write((address << 1) | 0x01); 		// Send identifier I2C address - Read
   	for (int i = 0; i < length; i++) {
   		buffer[i] = I2C_read(i < length - 1 ? 1 : 0); // Read
   	}
  	I2C_stop();
}

void I2C_WriteRegister(unsigned char address, unsigned char reg, unsigned char value) {
  	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write(address << 1);				// Send identifier I2C address - Write
   	I2C_write(reg);							// Send register address
   	I2C_write(value);						// Send value
   	I2C_stop();                   			// Send I2C Stop Transfer
}

void I2C_WriteMultipleRegisters2(unsigned char address, unsigned short startreg, const unsigned char* data, unsigned short length) {
  	I2C_start();                  			// Send I2C Start Transfer
   	I2C_write(address << 1);				// Send identifier I2C address - Write
   	I2C_write((startreg >> 8) & 0xFF);		// Send register address
   	I2C_write(startreg & 0xFF);				// Send register address
   	for (int i = 0; i < length; i++) {
   		I2C_write(data[i]); 				// Write
   	}
   	I2C_stop();
}
