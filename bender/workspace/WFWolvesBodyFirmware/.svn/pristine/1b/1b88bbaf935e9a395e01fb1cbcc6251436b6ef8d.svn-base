/*
 * servo_dynamixel.c
 *
 *  Created on: 16.08.2010
 *      Author: Stefan
 */

#ifdef DYNAMIXEL

#include "servo.h"
#include "servo_dynamixel.h"
#include "include/Board.h"
#include "uart/uart.h"
#include "ui/ui.h"
#include "tc/tc.h"
#include "dbgu/dbgu.h"
//#include "eeprom/eeprom.h"
#include <math.h>
#include <stdlib.h>

#define RS485_DIR		AT91C_PIO_PB6
#define DXL_POWER		AT91C_PIO_PB5

#define INST_WRITE		0x03
#define INST_READ		0x02
#define INST_SYNC_WRITE	0x83
#define DX_BROADCAST	0xFE

#define FRAME_START		0x00
#define FRAME_ID		0x02
#define FRAME_LEN		0x03
#define FRAME_INST		0x04 // Instruction packet is in req
#define FRAME_ERROR		0x04 // Error byte is in response
#define FRAME_PARM		0x05

#define SERVO_RATIO_VALUE2ANGLE			0.293 // 300 / 1024
#define SERVO_RATIO_ANGLE2VALUE			3.413 // 1024 / 300

#define SERVO_RATIO_VALUE2ANGLE_MX		0.087890625 // 360 / 4096
#define SERVO_RATIO_ANGLE2VALUE_MX		11.378 // 4096 / 360

#define ADDRESS_MODEL_NR			0x00
#define ADDRESS_ID					0x03
#define ADDRESS_BAUD				0x04
#define ADDRESS_CW_ANGLE_LIMIT_L	0x06
#define ADDRESS_TORQUE_ENABLE		0x18
#define ADDRESS_GOAL_POSITION_L		0x1E
#define ADDRESS_PRESENT_POS_L   	0x24
#define ADDRESS_PRESENT_VOLTAGE 	0x2A
#define ADDRESS_PRESENT_TEMPERATURE	0x2B
#define ADDRESS_PVALUE				0x1C
#define ADDRESS_IVALUE				0x1B
#define ADDRESS_DVALUE				0x1A

#define SERVO_TYPE_RX				0
#define SERVO_TYPE_MX				1

struct t_config_servo_types {
	unsigned char version;
	signed char type[32];
} config_servo_types;

CharReceivedCallback mOldDBGUCallback = 0;
CharReceivedCallback mOldUart1Callback = 0;
TxReadyCallback mOldDBGUTXBufCallback = 0;

volatile unsigned char mRXComplete, mRXBuffer[256];
static unsigned char mRXBufferPos = 0;

#define TX_BUFFER_SIZE	128
volatile unsigned char Servo_TXBuffer[TX_BUFFER_SIZE];
volatile unsigned char Servo_TXBufferWritePos = 0;
volatile unsigned char Servo_TXBufferReadPos = 0;

unsigned char servo_resetMaxError;

float servo_global_pGain;
float servo_global_iGain;
float servo_global_dGain;

void Servo_ResponseCallback(char rxd);
int Servo_Angle2Value(unsigned char id, float angle);
float Servo_Value2Angle(unsigned char id, int value);
void Servo_ListTypes(void);
unsigned char Servo_ReadModelNr(unsigned char id);

void Servo_Init_DYN(void) {
#ifndef DEBUG_X86
	AT91C_BASE_PIOB->PIO_PER |= RS485_DIR; // Allow PIO to control RS485 direction
	AT91C_BASE_PIOB->PIO_OER |= RS485_DIR; // Enable output
	AT91C_BASE_PIOB->PIO_CODR = RS485_DIR; // Start with receive mode (low)

	AT91C_BASE_PIOB->PIO_PER |= DXL_POWER; // Allow PIO to control dynamixel power
	AT91C_BASE_PIOB->PIO_OER |= DXL_POWER; // Enable output
	AT91C_BASE_PIOB->PIO_SODR = DXL_POWER; // Start with power on (high)
#endif

	uart1_AssignCharReceivedCallback(Servo_ResponseCallback);

	TC_DelayMs(100);			// Give the servos some time to initialize

	dbgu_printf("[Servo_Dynamixel] Autodetecting servo types... ");
	config_servo_types.version = 0;
	for (unsigned char i = 0; i < SERVO_COUNT_MAX; ++i) {
		unsigned char model = Servo_ReadModelNr(i);
		if (model == 29) {
			config_servo_types.type[i] = SERVO_TYPE_MX;
		} else {
			config_servo_types.type[i] = SERVO_TYPE_RX;
		}
	}
	dbgu_printf("Done!\r\n");
	//eeprom_read("SERVO_TYPE", &config_servo_types, sizeof(config_servo_types));

	Servo_ListTypes();

	servo_resetMaxError = 0;
}

void Servo_SendData(unsigned char* data, int length) {
	Servo_SpinLock();
	uart1_hintStartOfTransfer();

#ifndef DEBUG_X86
	AT91C_BASE_PIOB->PIO_SODR = RS485_DIR; // Send mode (high)
#endif
	for (int i = 0; i < length; i++) {
		uart1_putc(data[i]);
	}
	uart1_waitforsent();
#ifndef DEBUG_X86
	AT91C_BASE_PIOB->PIO_CODR = RS485_DIR; // Receive mode (low)
#endif

	uart1_hintEndOfTransfer();
	Servo_SpinUnLock();
}

unsigned char Servo_WriteData8(unsigned char id, unsigned char address, unsigned char data) {
    unsigned char packet[] = {0xff, 0xff, id, 3 + 1, INST_WRITE, address, data & 0xFF, 0};

    unsigned char checksum = 0;
    for (int i = 2; i < sizeof(packet); i++) {
    	checksum += packet[i];
    }
    packet[sizeof(packet) - 1] = ~checksum;

    for (unsigned char try = 0; try < 2; try++) {
    	unsigned long ticks_end = TC_GetSystemTicks() + 25 + (try == 0 ? 25 : 0);

    	mRXComplete = 0;
    	mRXBufferPos = 0;
    	Servo_SendData((unsigned char*)&packet, sizeof(packet));

    	if (id == DX_BROADCAST) {
    		return 1;
    	}

    	while (TC_GetSystemTicks() <= ticks_end) {
    		while (TC_GetSystemTicks() <= ticks_end && !mRXComplete) {
    		}
    		if (!mRXComplete) {
    			break;
    		}
    		if (mRXBuffer[FRAME_ID] == id) {
    			return 1;
    		}
    		ticks_end = TC_GetSystemTicks() + 25;
    		mRXComplete = 0;
    	}
    }
    return 0;
}

unsigned char Servo_WriteData16(unsigned char id, unsigned char address, int data) {
    unsigned char packet[] = {0xff, 0xff, id, 3 + 2, INST_WRITE, address, data & 0xFF, (data & 0xFF00) >> 8, 0};

    unsigned char checksum = 0;
    for (int i = 2; i < sizeof(packet); i++) {
    	checksum += packet[i];
    }
    packet[sizeof(packet) - 1] = ~checksum;

    for (unsigned char try = 0; try < 2; try++) {
    	unsigned long ticks_end = TC_GetSystemTicks() + 25 + (try == 0 ? 25 : 0);

    	mRXComplete = 0;
    	mRXBufferPos = 0;
    	Servo_SendData((unsigned char*)&packet, sizeof(packet));

    	if (id == DX_BROADCAST) {
    		return 1;
    	}

    	while (TC_GetSystemTicks() <= ticks_end) {
    		while (TC_GetSystemTicks() <= ticks_end && !mRXComplete) {
    		}
    		if (!mRXComplete) {
    			break;
    		}
    		if (mRXBuffer[FRAME_ID] == id) {
    			return 1;
    		}
    		ticks_end = TC_GetSystemTicks() + 25;
    		mRXComplete = 0;
    	}
    }
    return 0;
}

unsigned char Servo_SyncWrite(unsigned char startAddr, unsigned char eachLength, unsigned char number, unsigned char* params) {
	unsigned char packet_size = 7 + (number * eachLength) + 1;
    unsigned char* packet = malloc(packet_size);

    packet[0] = 0xff;
    packet[1] = 0xff;
    packet[2] = DX_BROADCAST;
    packet[3] = (eachLength * number) + 2 + 2;
    packet[4] = INST_SYNC_WRITE;
    packet[5] = startAddr;
    packet[6] = eachLength - 1;
    for(int i = 0; i < (number * eachLength); ++i) {
        packet[7 + i] = params[i];
    }

    unsigned char checksum = 0;
    for (int i = 2; i < packet_size - 1; ++i) {
    	checksum += packet[i];
    }
    packet[packet_size - 1] = ~checksum;

    Servo_SendData(packet, packet_size);
    free(packet);
    return 1;
}

unsigned char Servo_SendPosition(unsigned char id, float pos) {
	int tmp = Servo_Angle2Value(id, pos);
	if (id < SERVO_COUNT_MAX) {
		tmp += Servo_GetOffset(id);
		if (config_servo_types.type[id] == SERVO_TYPE_MX) {
			if (tmp > SERVO_MAX_VALUE_MX) tmp = SERVO_MAX_VALUE_MX;
		} else {
			if (tmp > SERVO_MAX_VALUE) tmp = SERVO_MAX_VALUE;
		}
		if (tmp < SERVO_MIN_VALUE) tmp = SERVO_MIN_VALUE;
	} else {
		dbgu_printf("[Servo] WARNING: Trying to access servo %d which is outside of calibrated offsets!\r\n", id);
	}

	return Servo_WriteData16(id, ADDRESS_GOAL_POSITION_L, tmp);
}

void Servo_SendPositions(void) {
	unsigned char data[SERVO_COUNT_MAX * 3];

	for(int i = 0; i < SERVO_COUNT_MAX; i++) {
		int tmp = Servo_Angle2Value(i, Servo_GetPosition(i));
		tmp += Servo_GetOffset(i);
		if (tmp < SERVO_MIN_VALUE) tmp = SERVO_MIN_VALUE;
		if (config_servo_types.type[i] == SERVO_TYPE_MX) {
			if (tmp > SERVO_MAX_VALUE_MX) tmp = SERVO_MAX_VALUE_MX;
		} else {
			if (tmp > SERVO_MAX_VALUE) tmp = SERVO_MAX_VALUE;
		}
		data[i * 3 + 0] = i; // ID
		data[i * 3 + 1] = tmp & 0xff;
		data[i * 3 + 2] = (tmp >> 8) & 0xff;
	}
	Servo_SyncWrite(ADDRESS_GOAL_POSITION_L, 3, SERVO_COUNT_MAX, (unsigned char*)&data);
}


float Servo_ReadPos(unsigned char id) {
	unsigned char data[SERVO_COUNT_MAX + 10] = { 0, };
	unsigned char dataRX[SERVO_COUNT_MAX + 10] = { 0, };
	float pValue = -1000;
	data[FRAME_ID] = (unsigned char) id;
	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = ADDRESS_PRESENT_POS_L;
	data[FRAME_PARM + 1] = 2;
	data[FRAME_LEN] = 4;

	if (Servo_TxRxPacket(data, dataRX) == 0) {
		unsigned short word;
		word = (int) dataRX[FRAME_PARM + 1];
		word = word << 8;
		word = word + (int) dataRX[FRAME_PARM];
		word -= Servo_GetOffset(id);
		pValue = Servo_Value2Angle(id, word);
	}
	return pValue;
}


int Servo_TxRxPacket(unsigned char *dataTX, unsigned char *dataRX)
{
	int res = 1;
	int length = dataTX[FRAME_LEN] + 4;
	dataTX[0] = 0xFF;
    dataTX[1] = 0xFF;
    unsigned char checksum = 0x00;
    for(int i=2; i<dataTX[FRAME_LEN]+3; i++ )
    	checksum += dataTX[i];
	dataTX[length - 1] =~checksum;


	for(int i=0;i<dataTX[FRAME_LEN]+4;i++)
	{
		//dbgu_printf("0x%x ",dataTX[i]);
	}

	mRXComplete = 0;
	mRXBufferPos = 0;
	for (unsigned char try = 0; try < 2; try++) {
	    	unsigned long ticks_end = TC_GetSystemTicks() + 25 + (try == 0 ? 25 : 0);
	    	Servo_SendData(dataTX, length);
	    	while (TC_GetSystemTicks() <= ticks_end) {
	    		while (TC_GetSystemTicks() <= ticks_end && !mRXComplete) {
	    		}
	    		if (!mRXComplete) {
	    			break;
	    		}
	    		if (mRXBuffer[FRAME_ID] == dataTX[FRAME_ID]) {
	    			for(int i=0;i<mRXBuffer[FRAME_LEN] + 4;i++)
	    			{
	    				dataRX[i]=mRXBuffer[i];
	    			}
	    			return 0;
	    		}
	    		ticks_end = TC_GetSystemTicks() + 25;
	    		mRXComplete = 0;
	    	}
	}

	return res;
}

unsigned char Servo_SetPassive(unsigned char id, unsigned char state) {
	return Servo_WriteData8(id, ADDRESS_TORQUE_ENABLE, state == 1 ? 0 : 1);
}

unsigned char Servo_GetPassive(unsigned char id) {
	unsigned char data[SERVO_COUNT_MAX + 10] = { 0, };
	unsigned char dataRX[SERVO_COUNT_MAX + 10] = { 0, };
	data[FRAME_ID] = id;
	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = ADDRESS_TORQUE_ENABLE;
	data[FRAME_PARM + 1] = 1;
	data[FRAME_LEN] = 4;

	if (Servo_TxRxPacket(data, dataRX) == 0) {
		return dataRX[FRAME_PARM] == 0 ? 1 : 0;
	} else {
		return 0;
	}
}


void Servo_SetPositionsTorque(unsigned char torque) {
	//TODO: Implement
}

void Servo_SendTGain(unsigned char id, char pGain, char iGain, char dGain) {
	// Only do this for MX servos
	if (config_servo_types.type[id] == SERVO_TYPE_MX) {
		Servo_WriteData8(id,ADDRESS_DVALUE,dGain);
		Servo_WriteData8(id,ADDRESS_IVALUE,iGain);
		Servo_WriteData8(id,ADDRESS_PVALUE,pGain);
	}
}

void Servo_SendTGainGlobal(float pGain, float iGain, float dGain) {
	unsigned char data[SERVO_COUNT_MAX * 4];

	unsigned char pGain_c = (unsigned char)pGain;
	unsigned char iGain_c = (unsigned char)iGain;
	unsigned char dGain_c = (unsigned char)dGain;

//	dbgu_printf("Setting global PID %u %u %u\r\n", pGain_c, iGain_c, dGain_c);

	int cnt = 0;
	for(int i = 0; i < SERVO_COUNT_MAX; i++) {
		// Only do this for MX servos
		if (config_servo_types.type[i] == SERVO_TYPE_MX) {
			data[cnt * 4 + 0] = i; // ID
			data[cnt * 4 + 1] = dGain_c & 0xff;
			data[cnt * 4 + 2] = iGain_c & 0xff;
			data[cnt * 4 + 3] = pGain_c & 0xff;
			cnt++;
		}
	}
	Servo_SyncWrite(ADDRESS_DVALUE, 4, cnt, (unsigned char*)&data);

	servo_global_pGain = pGain;
	servo_global_iGain = iGain;
	servo_global_dGain = dGain;
}

void Servo_GetTGainGlobal(float* pGain, float* iGain, float* dGain) {
	*pGain = servo_global_pGain;
	*iGain = servo_global_iGain;
	*dGain = servo_global_dGain;
}

unsigned char Servo_IsResponding(unsigned char id) {
	return Servo_ReadPos(id) != -1000 ? 1 : 0;
}

void Servo_SetID(unsigned char oldid, unsigned char newid) {
	Servo_WriteData8(oldid, ADDRESS_ID, newid);
}

void Servo_SetBaudrate(unsigned char id, unsigned int baud) {
	// Speed(BPS)  = 2000000/(Data+1)
	unsigned char baudvalue = (2000000 / baud) - 1;
	Servo_WriteData8(id, ADDRESS_BAUD, baudvalue);
	dbgu_printf("[Servo] Set baudrate to %d (%d)\r\n", baud, baudvalue);
}

unsigned char Servo_ReadLoad(unsigned char id) {
	//TODO: Implement
	return 0;
}

void Servo_ReadPID(unsigned char id, unsigned char* p, unsigned char* i, unsigned char* d) {
	*p = Servo_ReadAddress8(id,ADDRESS_PVALUE);
	*i = Servo_ReadAddress8(id,ADDRESS_IVALUE);
	*d = Servo_ReadAddress8(id,ADDRESS_DVALUE);

}
unsigned char Servo_ReadAddress8(unsigned char id,unsigned char address)
{
	unsigned char data[SERVO_COUNT_MAX + 10] = { 0, };
	unsigned short value = 255;
	unsigned char dataRX[SERVO_COUNT_MAX + 10] = { 0, };
	data[FRAME_ID] = (unsigned char) id;
	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = address;
	data[FRAME_PARM + 1] = 1;
	data[FRAME_LEN] = 4;

	if (Servo_TxRxPacket(data, dataRX) == 0) {
		value = dataRX[FRAME_PARM];
	}
	return value;
}
void Servo_SendExPortD(unsigned char id, char exPortD) {}
void Servo_SetOverload(unsigned char id, unsigned char overload) {}
void Servo_SetBoundaries(unsigned char id, unsigned short lower, unsigned short upper) {}

void Servo_ReadBounds(unsigned char id, unsigned short* bound_l, unsigned short* bound_u) {
	unsigned char data[SERVO_COUNT_MAX + 10] = { 0, };
	unsigned char dataRX[SERVO_COUNT_MAX + 10] = { 0, };
	data[FRAME_ID] = id;
	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = ADDRESS_CW_ANGLE_LIMIT_L;
	data[FRAME_PARM + 1] = 4;
	data[FRAME_LEN] = 4;

	Servo_TxRxPacket(data, dataRX);
	*bound_l = (dataRX[FRAME_PARM + 1] << 8) | dataRX[FRAME_PARM];
	*bound_u = (dataRX[FRAME_PARM + 3] << 8) | dataRX[FRAME_PARM + 2];
}

unsigned char Servo_ReadOverload(unsigned char id) { return 0; }

void Servo_ResponseCallback(char rxd) {
	UI_ToggleLED(2);

	mRXBuffer[mRXBufferPos] = rxd;
	mRXBuffer[0] += rxd;

	if (mRXBufferPos < 2 && rxd != 0xff)
		mRXBufferPos = 0;
	else if (mRXBufferPos == 2 && rxd == 0xff)
		mRXBuffer[0] -= rxd;
	else
		mRXBufferPos++;

	if (mRXBufferPos > 4 && mRXBufferPos == mRXBuffer[3] + 4) {
		if (mRXBuffer[0] == 0xfc) {
			mRXBuffer[0] = 0xff;
			mRXComplete = 1;
			//UI_SetLED(2, 0);
		}

		mRXBufferPos = 0;
	}
}

void Servo_ExtCharRecv(char ch) {
	UI_ToggleLED(0);
	Servo_SendData((unsigned char*)&ch, 1);
}

unsigned char Servo_ExtTXBufferEmpty(void) {
	// Print next from TX buffer
	if (Servo_TXBufferReadPos != Servo_TXBufferWritePos) {
		dbgu_print_char(Servo_TXBuffer[Servo_TXBufferReadPos]);
		Servo_TXBufferReadPos = (Servo_TXBufferReadPos + 1) % TX_BUFFER_SIZE;
		return 1; // More characters to send
	} else {
		return 0; // No more characters
	}
}

void Servo_ServoCharRecv(char ch) {
	UI_ToggleLED(1);
	// Add to TX buffer
	Servo_TXBuffer[Servo_TXBufferWritePos] = ch;
	Servo_TXBufferWritePos = (Servo_TXBufferWritePos + 1) % TX_BUFFER_SIZE;

#ifndef DEBUG_X86
	unsigned char empty = (Servo_TXBufferWritePos == Servo_TXBufferReadPos);
	if ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) && (empty)) {
        unsigned char temp_char = Servo_TXBuffer[Servo_TXBufferReadPos];
        Servo_TXBufferReadPos = (Servo_TXBufferReadPos + 1) % TX_BUFFER_SIZE;
        AT91C_BASE_DBGU->DBGU_THR = (temp_char & 0x1FF);
       	dbgu_enable_TxReadyCallback();
	}
#endif
}

void Servo_SetForwardingMode(unsigned char state) {
	if (state == 1) {
		mOldDBGUCallback = dbgu_AssignCharReceivedCallback(Servo_ExtCharRecv);
		mOldUart1Callback = uart1_AssignCharReceivedCallback(Servo_ServoCharRecv);
		mOldDBGUTXBufCallback = dbgu_AssignTxReadyCallback(Servo_ExtTXBufferEmpty);
	} else {
		dbgu_AssignCharReceivedCallback(mOldDBGUCallback);
		uart1_AssignCharReceivedCallback(mOldUart1Callback);
		dbgu_AssignTxReadyCallback(mOldDBGUTXBufCallback);
	}
}

int Servo_Angle2Value(unsigned char id, float angle) {
	if (config_servo_types.type[id] == SERVO_TYPE_MX) {
		return round(-angle * SERVO_RATIO_ANGLE2VALUE_MX) + SERVO_CENTER_VALUE_MX;
	} else {
		return round(-angle * SERVO_RATIO_ANGLE2VALUE) + SERVO_CENTER_VALUE;
	}
}

float Servo_Value2Angle(unsigned char id, int value) {
	if (config_servo_types.type[id] == SERVO_TYPE_MX) {
		return -(value - SERVO_CENTER_VALUE_MX) * SERVO_RATIO_VALUE2ANGLE_MX;
	} else {
		return -(value - SERVO_CENTER_VALUE) * SERVO_RATIO_VALUE2ANGLE;
	}
}

void Servo_SetPowerState(unsigned char state) {
	if (state == 1) {
#ifndef DEBUG_X86
		AT91C_BASE_PIOB->PIO_SODR = DXL_POWER; // Dynamixels on (high)
#endif
	} else {
#ifndef DEBUG_X86
		AT91C_BASE_PIOB->PIO_CODR = DXL_POWER; // Dynamixels off (low)
#endif
	}
}

unsigned char Servo_ReadVoltage(unsigned char id) {
	unsigned char data[SERVO_COUNT_MAX + 10] = { 0, };
	unsigned char dataRX[SERVO_COUNT_MAX + 10] = { 0, };
	data[FRAME_ID] = id;
	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = ADDRESS_PRESENT_VOLTAGE;
	data[FRAME_PARM + 1] = 1;
	data[FRAME_LEN] = 4;

	Servo_TxRxPacket(data, dataRX);
	return dataRX[FRAME_PARM];
}

unsigned char Servo_ReadTemperature(unsigned char id) {
	unsigned char data[SERVO_COUNT_MAX + 10] = { 0, };
	unsigned char dataRX[SERVO_COUNT_MAX + 10] = { 0, };
	data[FRAME_ID] = id;
	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = ADDRESS_PRESENT_TEMPERATURE;
	data[FRAME_PARM + 1] = 1;
	data[FRAME_LEN] = 4;

	Servo_TxRxPacket(data, dataRX);
	return dataRX[FRAME_PARM];
}

unsigned char Servo_ReadModelNr(unsigned char id) {
	unsigned char data[SERVO_COUNT_MAX + 10] = { 0, };
	unsigned char dataRX[SERVO_COUNT_MAX + 10] = { 0, };
	data[FRAME_ID] = id;
	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = ADDRESS_MODEL_NR;
	data[FRAME_PARM + 1] = 1;
	data[FRAME_LEN] = 4;

	Servo_TxRxPacket(data, dataRX);
	return dataRX[FRAME_PARM];
}

void Servo_SetType(unsigned char id, unsigned char type) {
	if (id < sizeof(config_servo_types.type)) {
		config_servo_types.type[id] = type;
		//eeprom_write("SERVO_TYPE", &config_servo_types, sizeof(config_servo_types));
	}
}

void Servo_ListTypes(void) {
	dbgu_printf("[Servo] Types:\r\n");
	for (int i = 0; i < SERVO_COUNT_MAX; i++) {
		dbgu_printf(" %2d: %s;", i, config_servo_types.type[i] == SERVO_TYPE_MX ? "MX" : "RX");
		if ((i + 1) % 10 == 0) {
			dbgu_printf("\r\n");
		}
	}
	dbgu_printf("\r\n");
}

void Servo_UpdateStatus(struct servoData_t servo_status[]) {
	static unsigned char nextReadID = 1;
	unsigned char data[10] = { 0, };
	unsigned char dataRX[16] = { 0, };

	data[FRAME_INST] = INST_READ;
	data[FRAME_PARM] = ADDRESS_PRESENT_POS_L;
	data[FRAME_PARM + 1] = 8;
	data[FRAME_LEN] = 4;
	//for (int i = 0; i < 2; i++) {
		data[FRAME_ID] = nextReadID;
		Servo_TxRxPacket(data, dataRX);

		unsigned short word = (dataRX[FRAME_PARM + 1] << 8) | dataRX[FRAME_PARM];
		word -= Servo_GetOffset(nextReadID);
		float pos = Servo_Value2Angle(nextReadID, word);
		float target = Servo_GetPosition(nextReadID);
		float err = abs(pos - target);
		if (servo_resetMaxError > 0) {
			servo_status[nextReadID].maxPositionError = 0;
			servo_resetMaxError--;
		}
		unsigned char error = err > 25.5f ? 255 : (unsigned char)(err * 10.0f);
		if (error > servo_status[nextReadID].maxPositionError) { servo_status[nextReadID].maxPositionError = error; }

		servo_status[nextReadID].voltage = dataRX[FRAME_PARM + 6];
		servo_status[nextReadID].temperature = dataRX[FRAME_PARM + 7];

		++nextReadID;
		if (nextReadID >= SERVO_COUNT_MAX) {
			nextReadID = 1;
		}
	//}
}

void Servo_ResetMaxError(void) {
	servo_resetMaxError = SERVO_COUNT_MAX;
}
#endif
