/*
 * shell.c
 *
 *  Created on: 18.04.2011
 *      Author: Stefan
 */

#include "dbgu/dbgu.h"
#include <string.h>
#include "shell_commands.h"

// UART command buffer size
#define SHELL_BUF_SIZE   32

// UART command max length
#define SHELL_CMD_LENGH_MAX (SHELL_BUF_SIZE - 1)

// UART command max parameter count
#define SHELL_PARA_CNT_MAX 4

#define SHELL_MAX_LASTCOMMANDS	4

// UART command shell prompt
#define SHELL_PROMPT ">> "

void shell_process_char(char c);
int shell_cmd_help(char * args[], char num_args);

// type defines
// UART command function prototype
typedef int (*shell_cmd_func)(char * args[], char num_args);

// UART command structure
typedef struct shell_cmd_struct {
	char *name;
	char *help;
	shell_cmd_func do_cmd;
} shell_cmd_struct_t;

const shell_cmd_struct_t shell_cmd_set[] = {
		{ "help", "\t\thelp\t\t\tPrints help", shell_cmd_help },
		// Add commands here
		{ "bootloader", "\tbootloader\t\tRestarts board in SAM-BA mode", shell_cmd_bootloader },
		{ "checkservo", "\tcheckservo id1 id2\tMoves Servo id and checks if id2 returns same position", shell_cmd_checkservo },
		{ "dumpeeprom", "\tdumpeeprom nr\t\tDumps content of EEPROM with given ID", shell_cmd_dumpeeprom },
		{ "eraseeeprom", "\teraseeeprom\t\tClears contents of EEPROM", shell_cmd_eraseeeprom },
		{ "getoffset", "\tgetoffset index\t\tGets current offset for servo <index>", shell_cmd_getoffset },
		{ "getservo", "\tgetservo index\t\tGets current position of servo <index>", shell_cmd_getservo },
		{ "getvoltage", "\tgetvoltage\t\tGets current voltage", shell_cmd_getvoltage },
		{ "getzeropos", "\tgetzeropos\t\tSwitches all servos to sensor mode and returns their values", shell_cmd_getzeropos },
		{ "fakelook", "\tfakelook\t\tLook around like like vision", shell_cmd_fakelook },
		{ "imutest", "\t\timutest\t\t\tSends raw and filtered IMU data for visualization", shell_cmd_imutest },
		{ "kinematictest", "\tkinematictest\t\tAllows to execute inverse kinematics with variable parameters", shell_cmd_kinematictest },
		{ "kickertest", "\tkickertest\t\tAllows to execute kicks with variable parameters", shell_cmd_kickertest },
		{ "kickertuner", "\tkickertuner\t\tChange the parameters of the kick-engine", shell_cmd_kickertuner },
		{ "listoffsets", "\tlistoffsets\t\tLists all offsets", shell_cmd_listoffsets },
		{ "listpositions", "\tlistpositions\t\tList all positions (without offsets)", shell_cmd_listpositions },
		{ "listservos", "\tlistservos\t\tLists all connected servo IDs", shell_cmd_listservos },
		{ "motioneditor", "\tmotioneditor idx\tEdit motion <idx>", shell_cmd_motioneditor },
		{ "offsettuner", "\toffsettuner\t\tAllows to set offsets for servos", shell_cmd_offsettuner },
		{ "playkinscene", "\tplaykinscene idx <rpt>\tPlay KINEMATIC scene <index> (optional: <repeat> times)", shell_cmd_playkinscene },
		{ "playkinscenewithctl", "playkinscenewithctl idx <rpt>\tPlay KINEMATIC scene <index> with active controller (optional: <repeat> times)", shell_cmd_playkinscenewithcontroller },
		{ "playscene", "\tplayscene index\t\tPlay STATIC scene <index>", shell_cmd_playscene },
		{ "rbcmode", "\t\trbcmode\t\t\tSwitch to RBC mode on DBGU port", shell_cmd_rbcmode },
		{ "receivemotionfile", "receivemotionfile\tReceive new motion file via XMODEM and store in EEPROM", shell_cmd_receivemotionfile },
		{ "saveconfig", "\tsaveconfig\t\tSaves configuration to EEPROM when available", shell_cmd_saveconfig },
		{ "searchservos", "\tsearchservos\t\tTries different baud rates to find servos", shell_cmd_searchservos },
		{ "selectmotion", "\tselectmotion mID mPage\tAssignes motion page to motion", shell_cmd_selectmotion },
		{ "servoinfo", "\tservoinfo index\t\tPrint data about the servo with ID <index>", shell_cmd_servoinfo },
		{ "servohelp", "\tservohelp\t\tPrints robot's servo layout", shell_cmd_servohelp },
		{ "servopower", "\tservopower state\tTurns power to servos on (1) or off (0)", shell_cmd_servopower },
		{ "servotemps", "\tservotemps\t\tPrints robot's servos temperatures", shell_cmd_servotemps },
		{ "servotest", "\tservotest index\t\tTest the given servo", shell_cmd_servotest },
		{ "setbaudrate", "\tsetbaudrate rate\tSets the baud rate of the servo interface", shell_cmd_setbaudrate },
		{ "setoffset", "\tsetoffset index offset\tSets the offset for servo <index> to <offset>", shell_cmd_setoffset },
		{ "setservo", "\tsetservo index pos\tMoves servo <index> to <pos>", shell_cmd_setservo },
		{ "setservobaud", "\tsetservobaud index baud\tSets baudrate of servo", shell_cmd_setservobaud },
		{ "setoffsetenable", "\tsetoffsetenable status\tEnables (1) or disables (0) servo offsets", shell_cmd_setoffsetenable },
		{ "setservoid", "\tsetservoid oldid newid\tChanges the ID of the servo", shell_cmd_setservoid },
		{ "setservoboundary", "setservoboundary index lower upper\tSets boundaries of the given servo", shell_cmd_setservoboundary },
		{ "setservopid", "\tsetservopid index p i d\tSets controller gains of the given servo", shell_cmd_setservopid },
		{ "setservooverload", "setservooverload index threshold\tSets overload threshold of the given servo", shell_cmd_setservooverload },
		{ "settype", "\t\tsettype index type\tSets the type of servo <index>", shell_cmd_settype },
		{ "showservopos", "\tshowservopos index passive\t\tPrint position of servo with ID <index>", shell_cmd_showservopos },
		{ "showmagnet", "\tshowagnet\t\tPrint Magnetometerdata", shell_cmd_showmagnet },
		{ "walktuner", "\twalktuner\t\tAllows to tune parameters of the walk algorithm", shell_cmd_walktuner },
		{ "showodometrie", "\tshowodometrie\tShow the Position and the Change Values of the Robot", shell_cmd_showOdometrieData },
		{ "odometriexchange", "\todometriexchange\tSet the XChange Value to <value>", shell_cmd_setOdometrieDataXChange },
		{ "odometrieychange", "\todometrieychange\tSet the YChange Value to <value>", shell_cmd_setOdometrieDataYChange },
		{ "odometrieviewchange", "\todometrieviewchange\tSet the ViewChange Value to <value>", shell_cmd_setOdometrieDataViewChange },
		{ "odometriereset", "\todometriereset\tResets all Values off the Odometrie", shell_cmd_resetOdometrie },
		{ "odometriesave", "\todometriesave\tSave the Odometrie Change-Values in the EEPROM", shell_cmd_saveOdometrie},
		// End of command list
		{ 0, 0, 0 }
};

// UART command buffer
static unsigned char shell_buf[SHELL_BUF_SIZE];
// UART command buffer write pointer
static unsigned char shell_wptr = 0;
// UART command parameter pointer
static char* shell_para_ptr[SHELL_PARA_CNT_MAX];

static unsigned char shell_lastcmd[SHELL_MAX_LASTCOMMANDS][SHELL_BUF_SIZE];
static unsigned char shell_lastcmd_ptr = 0;

unsigned char shell_escape_state = 0;

// flush the UART command buffer
void shell_flush_buf(void) {
	shell_wptr = 0;
	memset(shell_buf, 0, sizeof(shell_buf));
	memset(shell_para_ptr, 0, sizeof(shell_para_ptr));
}

// print the command shell prompt
void shell_cmd_prompt() {
	dbgu_print_ascii("\r\n");
	dbgu_printf(SHELL_PROMPT);
}

void shell_restore_lastcmd(unsigned char down) {
	if (down == 1) {
		if (shell_lastcmd_ptr > 0) {
			shell_lastcmd_ptr--;
		}
	}
	if (shell_lastcmd[shell_lastcmd_ptr][0] != 0) {
		int i = 0;
		unsigned char chars = shell_wptr;
		// Remove old chars by sending backspaces
		for (i = 0; i < chars; i++) {
			shell_process_char(127);
		}
		// "Type" last command
		chars = strlen((char*)shell_lastcmd[shell_lastcmd_ptr]);
		for (i = 0; i < chars; i++) {
			shell_process_char(shell_lastcmd[shell_lastcmd_ptr][i]);
		}
		//memset(shell_lastcmd[0], 0, sizeof(shell_lastcmd[0]));
		if (shell_lastcmd_ptr < SHELL_MAX_LASTCOMMANDS - 1 && down == 0) {
			if (shell_lastcmd[shell_lastcmd_ptr + 1][0] != 0) {
				shell_lastcmd_ptr++;
			}
		}
	}
}

// Execute the command in the command buffer
void shell_cmd_execute(void) {
	unsigned char i = 0, para_cnt = 0, err = 0;

	while ((para_cnt < SHELL_PARA_CNT_MAX) && (shell_para_ptr[para_cnt]) && (*shell_para_ptr[para_cnt])) {
		para_cnt++;
	}

	// Move old saved commands to higher array indices
	for (i = SHELL_MAX_LASTCOMMANDS - 1; i > 0; --i) {
		strcpy((char*)shell_lastcmd[i], (char*)shell_lastcmd[i - 1]);
	}

	// Save current command
	strcpy((char*)shell_lastcmd[0], (char*)shell_buf);
	for (i = 0; i < para_cnt; i++) {
		strcat((char*)shell_lastcmd[0], " ");
		strcat((char*)shell_lastcmd[0], shell_para_ptr[i]);
	}

	shell_lastcmd_ptr = 0;

	while (0 != (shell_cmd_set[i].name)) {
		if (!strcmp((char*) shell_buf, shell_cmd_set[i].name)) {
			dbgu_print_ascii("\r\n");
			err = shell_cmd_set[i].do_cmd(shell_para_ptr, para_cnt);
			if (err != 0) {
				dbgu_print_ascii("\r\n");
				dbgu_printf("Error %d!\r\n", err);
			}
			shell_flush_buf();
			shell_cmd_prompt();
			return;
		}
		i++;
	}

	dbgu_printf("\r\nUnknown Command\r\n");
	shell_flush_buf();
	shell_cmd_prompt();
}

void shell_process_char(char c) {
	// if the first byte is ' ' or '\n', ignore it
	if ((0 == shell_wptr) && (' ' == c || '\r' == c)) {
		shell_cmd_prompt();
		return;
	}

	// if '\n' is read, execute the command
	if ('\r' == c) {
		shell_cmd_execute();
	}
	// if ' ' is read, record the parameter ptr
	else if (' ' == c) {
		dbgu_print_char(c);
		// damping the space
		if (shell_buf[shell_wptr - 1]) {
			// replace it with NULL
			shell_buf[shell_wptr] = 0;

			shell_wptr++;

			// record the parameter address
			int i = 0;
			for (i = 0; i < SHELL_PARA_CNT_MAX; i++) {
				if (!shell_para_ptr[i]) {
					shell_para_ptr[i] = (char*) (&shell_buf[shell_wptr]);
					break;
				}
			}

			if (SHELL_PARA_CNT_MAX == i) {
				shell_cmd_execute();
				return;
			}
		}
	} else if (127 == c || 8 == c) { // Backspace
		if (shell_wptr > 0) {
			dbgu_printf("\b \b");
			shell_wptr--;
			if (shell_buf[shell_wptr] == '\0') {
				// Delete last parameter
				int i = 0;
				for (i = SHELL_PARA_CNT_MAX - 1; i >= 0; --i) {
					if (shell_para_ptr[i]) {
						shell_para_ptr[i] = 0;
						break;
					}
				}
			}
			shell_buf[shell_wptr] = 0;
		}
	} else if (27 == c) { // Escape char
		shell_escape_state = 1;
	} else if (0xe0 == (c & 0xFF)) { // Escape char
		shell_escape_state = 2;
	} else if (shell_escape_state == 1) {
		if (c == 91) {
			shell_escape_state = 2;
		} else {
			shell_escape_state = 0;
		}
	} else if (shell_escape_state == 2) {
		shell_escape_state = 0;
		if (c == 65 || c == 0x48) { // Up key
			shell_restore_lastcmd(0);
		} else if (c == 66 || c == 0x50) { // Down key
			shell_restore_lastcmd(1);
		}
	} else if (c == 9) { // Tab
		if (shell_para_ptr[0] == 0) { // Only allow completion when entering command, not parameters
			unsigned char i = 0;
			unsigned char match = 0xFF;
			unsigned char matchcnt = 0;
			unsigned char matchlen = 0;
			while (0 != (shell_cmd_set[i].name)) {
				if (!strncmp((char*)shell_buf, shell_cmd_set[i].name, shell_wptr)) {
					matchcnt++;
					if (matchcnt == 1) {
						match = i;
						matchlen = strlen(shell_cmd_set[i].name);
					} else {
						// More than one match, so list
						if (matchcnt == 2) {
							// Print last match
							dbgu_printf("\r\n%s ", shell_cmd_set[match].name);
						}
						dbgu_printf("%s ",  shell_cmd_set[i].name);
						// Find how many chars are the same
						if (matchlen > strlen(shell_cmd_set[i].name)) {
							matchlen = strlen(shell_cmd_set[i].name);
						}
						for (unsigned char j = matchlen; j > 0; --j) {
							if (shell_cmd_set[i].name[j] != shell_cmd_set[match].name[j]) {
								matchlen = j;
							}
						}
					}
				}
				i++;
			}
			if (matchcnt == 1) {
				unsigned char len = strlen(shell_cmd_set[match].name);
				for (i = shell_wptr; i < len; ++i) {
					shell_process_char(shell_cmd_set[match].name[i]);
				}
				shell_process_char(' ');
			} else if (matchcnt > 1) {
				if (matchlen > shell_wptr) {
					for (i = shell_wptr; i < matchlen; ++i) {
						shell_buf[shell_wptr] = shell_cmd_set[match].name[i];
						shell_wptr++;
						if (shell_wptr == SHELL_CMD_LENGH_MAX) {
							shell_cmd_execute();
						}
					}
				}
				dbgu_printf("\r\n");
				shell_cmd_prompt();
				for (i = 0; i < shell_wptr; ++i) {
					dbgu_print_char(shell_buf[i]);
				}
			}
		}
	}
	// other characters, just record it
	else {
		dbgu_print_char(c);
		shell_buf[shell_wptr] = c;
		shell_wptr++;
		if (shell_wptr == SHELL_CMD_LENGH_MAX) {
			shell_cmd_execute();
		}
	}
}

void Shell_Main(void) {
	shell_flush_buf();
	shell_cmd_prompt();

	shell_escape_state = 0;
	while (1) {
		char c = 0;

		while (dbgu_char_available()) {
			// read one byte from serial port
			c = dbgu_get_char();
			shell_process_char(c);
		}
	}
}

// help command implementation
int shell_cmd_help(char * args[], char num_args) {
	int i = 0;

	dbgu_printf("\r\nCOMMAND SHELL HELP\n\n\rCommand list\r\n");

	while (shell_cmd_set[i].name) {
		dbgu_printf(shell_cmd_set[i].name);
		dbgu_printf("\t");
		dbgu_printf(shell_cmd_set[i].help);
		dbgu_print_ascii("\r\n");
		i++;
	}

	return 0;
}
