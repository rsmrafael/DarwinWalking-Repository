/*
 * cm730.c
 *
 *  Created on: 01.10.2012
 *      Author: Stefan
 */

#include "cm730.h"
#include "include/Board.h"
#include "parser/parser.h"
#include "uart/uart.h"
#include "dbgu/dbgu.h"
#include "servo/servo.h"
#include "ui/ui.h"
#include "imu/imu.h"
#include "usb/usb.h"
#include <string.h>

#define	CM730_MY_ID					200
#define CM730_BROADCAST				0xfe

#define CM730_HEADER1				0xff
#define CM730_HEADER2				0xff

#define CM730_STATE_IDLE			0xfff0
#define CM730_STATE_HEADER1			0xfff1
#define CM730_STATE_ID				0xfff8
#define CM730_STATE_SIZE			0xfffa
#define CM730_STATE_INSTRUCTION		0xfffb
#define CM730_STATE_PARAMETER		0xfffc

#define CM730_CMD_PING				0x1
#define CM730_CMD_WRITE				0x3
#define CM730_CMD_SYNC_WRITE		0x83
#define CM730_CMD_BULK_READ			0x92

#define CM730_ERROR_RANGE			(1 << 3)

#define P_DXL_POWER					24
#define P_BUTTON					30
#define P_GYRO_Z					38
#define P_GYRO_Y					40
#define P_GYRO_X					42
#define P_ACC_X						44
#define P_ACC_Y						46
#define P_ACC_Z						48

#define WRITE_WORD(POS, VALUE) cm730_data[POS + 1] = (VALUE >> 8) & 0xFF; cm730_data[POS] = VALUE & 0xFF;

CharReceivedCallback mCM730OldUart0Callback = 0;
CharReceivedCallback mCM730OldUart1Callback = 0;
TxReadyCallback mCM730OldUart0TXBufCallback = 0;

unsigned short cm730_state = CM730_STATE_IDLE;
unsigned char cm730_checksum;
struct parser_message cm730_currentMessage;
struct parser_message cm730_lastMessage;
volatile unsigned char cm730_messageProcessed;

char cm730_data[90];

extern void IMU_accel_update(void);
extern void IMU_gyro_update(void);
extern int imu_AccelX_raw, imu_AccelY_raw, imu_AccelZ_raw;
extern int imu_GyroX_raw, imu_GyroY_raw, imu_GyroZ_raw;


#define TX_BUFFER_SIZE	128
volatile unsigned char CM730_TXBuffer[TX_BUFFER_SIZE];
volatile unsigned char CM730_TXBufferWritePos = 0;
volatile unsigned char CM730_TXBufferReadPos = 0;

void CM730_ProcessChar(char ch);
void CM730_ProcessServoChar(char ch);
unsigned char CM730_ExtTXBufferEmpty(void);
void CM730_SendStatus(unsigned char id, unsigned char error, unsigned int length, char* data);

void CM730_Main() {
	char usbBuffer[128];
	char usbSendBuffer[64];

	Servo_SetPowerState(0);

	memset(&cm730_currentMessage, 0, sizeof(cm730_currentMessage));
	memset(&cm730_lastMessage, 0, sizeof(cm730_lastMessage));
	cm730_state = CM730_STATE_IDLE;
	cm730_messageProcessed = 1;

	uart0_SetBaudrate(AT91B_DBGU_BAUD_RATE);
	mCM730OldUart0Callback = uart0_AssignCharReceivedCallback(CM730_ProcessChar);
	mCM730OldUart1Callback = uart1_AssignCharReceivedCallback(CM730_ProcessServoChar);
#ifndef DEBUG_X86
	mCM730OldUart0TXBufCallback = uart0_AssignTxReadyCallback(CM730_ExtTXBufferEmpty);
#endif
	while (1) {
		// Send chars in TX buffer via USB
		if (CM730_TXBufferReadPos != CM730_TXBufferWritePos) {
			unsigned char idx = 0;
			do {
				usbSendBuffer[idx] = CM730_TXBuffer[CM730_TXBufferReadPos];
				idx++;
				CM730_TXBufferReadPos = (CM730_TXBufferReadPos + 1) % TX_BUFFER_SIZE;
			} while (CM730_TXBufferReadPos != CM730_TXBufferWritePos && idx < sizeof(usbSendBuffer));
			USB_send(usbSendBuffer, idx);
		}

		// Look for USB chars, when available feed to parser
		int read = USBRead_nonblocking(&usbBuffer[0], sizeof(usbBuffer));
		if (read > 0) {
			for (unsigned char i = 0; i < read; ++i) {
				CM730_ProcessChar(usbBuffer[i]);
			}
		}

		// Read button state
		unsigned char v = 0;
		if (UI_GetKeyState_NoDebounce(0)) v |= 0x1;
		if (UI_GetKeyState_NoDebounce(1)) v |= 0x2;
		cm730_data[P_BUTTON] = v;
		// Update IMU
    	IMU_accel_update();
    	IMU_gyro_update();
		WRITE_WORD(P_ACC_Z, imu_AccelX_raw);
		WRITE_WORD(P_ACC_Y, imu_AccelZ_raw);
		WRITE_WORD(P_ACC_X, imu_AccelY_raw);
		WRITE_WORD(P_GYRO_Z, imu_GyroX_raw);
		WRITE_WORD(P_GYRO_Y, imu_GyroY_raw);
		WRITE_WORD(P_GYRO_X, imu_GyroZ_raw);

		if (cm730_messageProcessed == 0) { // Message waiting
			if (cm730_lastMessage.type == CM730_MY_ID || cm730_lastMessage.type == CM730_BROADCAST) {
				//if (cm730_lastMessage.type == CM730_BROADCAST) {
				//	dbgu_printf("[CM730] Received broadcast\r\n");
				//}
				if (cm730_lastMessage.instruction == CM730_CMD_WRITE) {
					if (cm730_lastMessage.length == 2) {
						//dbgu_printf("[CM730] Received byte write request: %d @ %d\r\n", cm730_lastMessage.data[0], cm730_lastMessage.data[1]);
						if (cm730_lastMessage.data[0] == P_DXL_POWER) {
							Servo_SetPowerState(cm730_lastMessage.data[1]);
							cm730_data[P_DXL_POWER] = cm730_lastMessage.data[1];
						}
					} else if (cm730_lastMessage.length == 3) {
						//dbgu_printf("[CM730] Received word write request: %d @ %d\r\n", cm730_lastMessage.data[0], cm730_lastMessage.data[1] | (cm730_lastMessage.data[2] << 8));
					}
					if (cm730_lastMessage.type != CM730_BROADCAST) {
						CM730_SendStatus(cm730_lastMessage.type, 0, 0, NULL);
					}
				} else if (cm730_lastMessage.instruction == CM730_CMD_PING) {
					//dbgu_printf("[CM730] Received ping request\r\n");
					if (cm730_lastMessage.type != CM730_BROADCAST) {
						CM730_SendStatus(cm730_lastMessage.type, 0, 0, NULL);
					}
				} else if (cm730_lastMessage.instruction == CM730_CMD_SYNC_WRITE) {
					//unsigned char start_addr = cm730_lastMessage.data[0];
					//unsigned char len = cm730_lastMessage.data[1];
					//for(unsigned char i = 2; i < cm730_lastMessage.length; i += (len + 1)) {
					//	unsigned char id = cm730_lastMessage.data[i];
					//	if (id == CM730_MY_ID) {
							//dbgu_printf("[CM730] Received sync write request: %d bytes from %d\r\n", len, start_addr);
					//	}
					//}
				} else if (cm730_lastMessage.instruction == CM730_CMD_BULK_READ) {
					struct parser_message bulk_message;
					memcpy(&bulk_message, &cm730_lastMessage, sizeof(struct parser_message));
					unsigned char num = (bulk_message.length - 1) / 3;
					unsigned char last_id = 0xFF;
					for(char i = 0; i < num; ++i) {
						unsigned char id = bulk_message.data[(3*i) + 2];
						unsigned char len = bulk_message.data[(3*i) + 1];
						unsigned char addr = bulk_message.data[(3*i) + 3];
						if (id == CM730_MY_ID) {
							//dbgu_printf("[CM730] Received bulk read request for %d bytes from %d\r\n", len, addr);
							if (last_id == 0xFF) { // We are the first in the list
								//process
								if (addr + (len - 1) >= sizeof(cm730_data)) {
									CM730_SendStatus(CM730_MY_ID, CM730_ERROR_RANGE, 0, NULL);
								} else {
									CM730_SendStatus(CM730_MY_ID, 0, len, &cm730_data[addr]);
								}
								break;
							} else {
								// wait
								while (1) {
									cm730_messageProcessed = 1; // Message processed
									//TODO: Add timeout
									while (cm730_messageProcessed == 1); // Wait for new message
									if (cm730_currentMessage.type == last_id) {
										break;
									}
								}
								if (addr + (len - 1) >= sizeof(cm730_data)) {
									CM730_SendStatus(CM730_MY_ID, CM730_ERROR_RANGE, 0, NULL);
								} else {
									CM730_SendStatus(CM730_MY_ID, 0, len, &cm730_data[addr]);
								}
								break;
							}
						}
						last_id = id;
					}
				} else {
					dbgu_printf("[CM730] Unknown instruction: %d (data length: %d)\r\n", cm730_lastMessage.instruction, cm730_lastMessage.length);
				}
			//} else {
			//	dbgu_printf("[CM730] Received instruction for ID %d\r\n", cm730_lastMessage.type);
			}

			cm730_messageProcessed = 1; // Message processed
		}
	}

	uart0_SetBaudrate(AT91_BAUD_RATE_EXT);
	uart0_AssignCharReceivedCallback(mCM730OldUart0Callback);
	uart1_AssignCharReceivedCallback(mCM730OldUart1Callback);
#ifndef DEBUG_X86
	uart0_AssignTxReadyCallback(mCM730OldUart0TXBufCallback);
#endif

	Servo_SetPowerState(1);
}

void CM730_SendStatus(unsigned char id, unsigned char error, unsigned int length, char* data) {
	unsigned char checksum = 0;
	char buffer[length + 6];

	buffer[0] = 0xFF;
	buffer[1] = 0xFF;
	buffer[2] = id;
	buffer[3] = 2 + length;
	buffer[4] = error;
	for (unsigned int i = 0; i < length; i++) {
		buffer[5 + i] = data[i];
		checksum += data[i];
	}
	buffer[length + 5] = (~(id + (2 + length) + error + checksum) & 0xFF);
	USB_send(buffer, length + 6);
}

void CM730_ProcessChar(char ch) {
	if (cm730_state == CM730_STATE_IDLE) {
		if (ch == CM730_HEADER1) {
			cm730_state = CM730_STATE_HEADER1;
			cm730_checksum = 0;
			memset(&cm730_currentMessage, 0, sizeof(cm730_currentMessage));
		}
	} else if (cm730_state == CM730_STATE_HEADER1) {
		if (ch == CM730_HEADER2) {
			cm730_state = CM730_STATE_ID;
		} else {
			cm730_state = CM730_STATE_IDLE;
		}
	} else if (cm730_state == CM730_STATE_ID) {
		cm730_currentMessage.type = ch;
		cm730_checksum += ch;
		cm730_state = CM730_STATE_SIZE;
	} else if (cm730_state == CM730_STATE_SIZE) {
		cm730_currentMessage.length = ch;
		cm730_checksum += ch;
		if (cm730_currentMessage.length >=2) {
			cm730_currentMessage.length -= 2;
		}
		cm730_state = CM730_STATE_INSTRUCTION;
	} else if (cm730_state == CM730_STATE_INSTRUCTION) {
		cm730_currentMessage.instruction = ch;
		cm730_checksum += ch;
		cm730_state = 0;
	} else if (cm730_state < cm730_currentMessage.length) {
		cm730_currentMessage.data[cm730_state] = ch;
		cm730_state++;
		cm730_checksum += ch;
	} else if (cm730_state == cm730_currentMessage.length) {
		if ((~(cm730_checksum) & 0xFF) == ch) {
			//if (cm730_messageProcessed == 1) { // If last message was processed
				memcpy(&cm730_lastMessage, &cm730_currentMessage, sizeof(cm730_lastMessage)); // Copy buffer
				cm730_messageProcessed = 0;
			//}
		} else {
			dbgu_printf("[CM730] Checksum error: Got %d, expected %d\r\n", ch, (~(cm730_checksum) & 0xFF));
		}
		cm730_state = CM730_STATE_IDLE;
	}
	Servo_SendData((unsigned char*)&ch, 1);
}

void CM730_ProcessServoChar(char ch) {
//	unsigned char empty = (CM730_TXBufferWritePos == CM730_TXBufferReadPos);
	// Add to TX buffer
	CM730_TXBuffer[CM730_TXBufferWritePos] = ch;
	CM730_TXBufferWritePos = (CM730_TXBufferWritePos + 1) % TX_BUFFER_SIZE;

/*	if ((AT91C_BASE_US0->US_CSR & AT91C_US_TXRDY) && (empty)) {
        unsigned char temp_char = CM730_TXBuffer[CM730_TXBufferReadPos];
        CM730_TXBufferReadPos = (CM730_TXBufferReadPos + 1) % TX_BUFFER_SIZE;
        AT91C_BASE_US0->US_THR = (temp_char & 0x1FF);
       	uart0_enable_TxReadyCallback();
	}
*/
}

unsigned char CM730_ExtTXBufferEmpty(void) {
	// Print next from TX buffer
//	if (CM730_TXBufferReadPos != CM730_TXBufferWritePos) {
//		uart0_putc(CM730_TXBuffer[CM730_TXBufferReadPos]);
//		CM730_TXBufferReadPos = (CM730_TXBufferReadPos + 1) % TX_BUFFER_SIZE;
//		return 1; // More characters to send
//	} else {
		return 0; // No more characters
//	}
}
