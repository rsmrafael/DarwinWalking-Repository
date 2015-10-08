/*
 * action.h
 *
 *  Created on: 17.10.2012
 *      Author: Stefan Krupop
 */

#ifndef ACTION_H_
#define ACTION_H_
#define SPEED_BASE_SCHEDULE	0
#define TIME_BASE_SCHEDULE	0x0a
#define MAXNUM_PAGE			256
#define INVALID_BIT_MASK	0x4000
#define TORQUE_OFF_BIT_MASK	0x2000
#define MAXNUM_STEP			7
#define MIN_SERVO_ID		1
#define NUMBER_OF_JOINTS	21
#define MAXNUM_NAME			13

typedef struct { // Header Structure (total 64unsigned char)
	unsigned char name[MAXNUM_NAME + 1]; 	// Name             		0~13
	unsigned char reserved1; 				// Reserved1        		14
	unsigned char repeat; 					// Repeat count     		15
	unsigned char schedule; 				// schedule         		16
	unsigned char reserved2[3]; 			// reserved2        		17~19
	unsigned char stepnum; 					// Number of step   		20
	unsigned char reserved3; 				// reserved3        		21
	unsigned char speed; 					// Speed            		22
	unsigned char reserved4; 				// reserved4        		23
	unsigned char accel; 					// Acceleration time 		24
	unsigned char next; 					// Link to next     		25
	unsigned char exit; 					// Link to exit     		26
	unsigned char reserved5[4]; 			// reserved5        		27~30
	unsigned char checksum; 				// checksum         		31
	unsigned char slope[31]; 				// CW/CCW compliance slope  32~62
	unsigned char reserved6; 				// reserved6        		63
} PAGEHEADER;

typedef struct { // Step Structure (total 64unsigned char)
	unsigned short position[31]; 			// Joint position			0~61
	unsigned char pause;					// Pause time       		62
	unsigned char time; 					// Time             		63
} STEP;

typedef struct { // Page Structure (total 512unsigned char)
	PAGEHEADER header;						// Page header  			0~64
	STEP step[MAXNUM_STEP];					// Page step    			65~511
} PAGE;


void Action_Init(void);
unsigned char Action_Start(int iPage);
unsigned char Action_Start_Name(char* namePage);
unsigned char Action_Start_Page(int idx, PAGE *pPage);
void Action_Stop(void);
void Action_Brake(void);
unsigned char Action_IsRunning(void);
void Action_ResetPage(PAGE *pPage);
unsigned char Action_LoadPage(int idx, PAGE *pPage);
unsigned char Action_SavePage(int idx, PAGE *pPage);



#endif /* ACTION_H_ */
