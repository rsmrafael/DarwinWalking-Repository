/*
 * uart_win32.c
 *
 *  Created on: 29.07.2011
 *      Author: Stefan
 */

#ifdef DEBUG_X86

#include "dbgu.h"
#include <stdarg.h>
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <string.h>
#endif

#ifndef WIN32
int kbhit(void) {
   struct termios term, oterm;
   int fd = 0;
   int c = 0;

   tcgetattr(fd, &oterm);
   memcpy(&term, &oterm, sizeof(term));

   term.c_lflag = term.c_lflag & (!ICANON);
   term.c_cc[VMIN] = 0;
   term.c_cc[VTIME] = 1;

   tcsetattr(fd, TCSANOW, &term);

   c = getchar();

   tcsetattr(fd, TCSANOW, &oterm);

   if (c != -1)
	   ungetc(c, stdin);

   return ((c != -1) ? 1 : 0);
}

int getch() {
   static int ch = -1, fd = 0;
   struct termios term, oterm;

   fd = fileno(stdin);
   tcgetattr(fd, &oterm);

   memcpy(&term, &oterm, sizeof(term));
   term.c_lflag &= ~(ICANON|ECHO);

   tcsetattr(fd, TCSANOW, &term);

   ch = getchar();

   tcsetattr(fd, TCSANOW, &oterm);

   return ch;
}
#endif

CharReceivedCallback dbgu_CharReceivedCallbackFnct = 0;
TxReadyCallback dbgu_TxBufferEmptyCallbackFnct = 0;

// Prototypes
void DBGUIRQHandler(void);

/*-----------------------------------------------------------------------------
 * Function Name       : configure_dbgu
 * Object              : Configure DBGU
 *-----------------------------------------------------------------------------*/
void DBGU_Init(void) {
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_gets
 * Object              : This function is used to get a char Array through the
 *                       DBGU channel (Very low level debugging)
 *----------------------------------------------------------------------------*/
void dbgu_gets(char* buffer, unsigned short maxSize) {

	unsigned char ch = 0;
	int i;
	for(i=0;i<maxSize&& ch!='\n'&&ch!='\r';i++)
	{
		ch = dbgu_get_char();
		buffer[i]=ch;
	}
	buffer[i] = '\0';
}
/*----------------------------------------------------------------------------
 * Function Name       : dbgu_char_available
 * Object              : This function is used to determine if a char is waiting on the
 *                       DBGU channel (Very low level debugging)
 *----------------------------------------------------------------------------*/
unsigned char dbgu_char_available(void) {
	if (kbhit()) {
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_get_char
 * Object              : This function is used to get a char through the
 *                       DBGU channel (Very low level debugging)
 *----------------------------------------------------------------------------*/
unsigned char dbgu_get_char(void) {
    return getch();
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_ascii
 * Object              : This function is used to send a string through the
 *                       DBGU channel (Very low level debugging)
 *----------------------------------------------------------------------------*/
void dbgu_print_ascii(const char *buffer) {
	printf("%s", buffer);
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_char
 * Object              : This function is used to send a char through the
 *                       DBGU channel (Very low level debugging)
 *----------------------------------------------------------------------------*/
void dbgu_print_char(const unsigned char ch) {
	printf("%c", ch);
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_char_in_hex
 * Object              : This function is used to print a 8-bit value in hexa
 *----------------------------------------------------------------------------*/
void dbgu_print_char_in_hex(unsigned char value) {
    unsigned char c1, c2;
    c1 = (value & 0x0F); // LSB
    c2 = (value & 0xF0) >> 4; //MSB
    dbgu_print_ascii("0x"); // print the hex notation first
    dbgu_print_4bit_in_hex(c2); // print LSB
    dbgu_print_4bit_in_hex(c1); // print MSB
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_4bit_in_hex
 * Object              : This function is used to print a 8-bit value in hexa
 *----------------------------------------------------------------------------*/
void dbgu_print_4bit_in_hex(unsigned char value) {
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_digit
 * Object              : This function is used to print a 32-bit value in hexa
 *----------------------------------------------------------------------------*/
void dbgu_print_digit(unsigned char value) {
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_hex8
 * Object              : This function is used to print a 32-bit value in hexa
 *----------------------------------------------------------------------------*/
void dbgu_print_hex8(unsigned long value) {
    char shift = sizeof(unsigned long) * 8;

    dbgu_print_ascii("0x");
    do {
        shift -= 4;
    } while (shift != 0);
}

void dbgu_print_int(int zahl) {
	if (zahl <= 0) {
		dbgu_print_char('-');
		zahl = zahl * -1;
	}

	int mod = 10, durch = 1;
	long int temp;
	//Variablen der L�ngenerkenung
	int laenge = 0;

	//Ermittlung der anzahl der Stellen in der Variabl Bsp: 123 -> 3 Stellen
	do {
		laenge++;
		durch = durch * 10;
	} while (zahl / durch > 0);
	durch = 1; //Reset der "durch" Variable

	//Ausgabe der einzelnen Stellenwerte, von links nach rechts (F�r USART oder der Gleichen)
	int i, j, modtemp, durchtemp;
	for (i = laenge; i > 0; i--) {
		modtemp = mod;
		durchtemp = durch;
		for (j = 1; j < i; j++) //Berechnen des Modulu und des Dividenden
		{
			modtemp *= 10;
			durchtemp *= 10;
		}
		temp = (zahl % modtemp) / durchtemp;
		dbgu_print_char(temp + 48); //Ausgabe
	}
}

void dbl2stri(char *outbfr, double dbl, unsigned dec_digits) {
   char *output = outbfr;

	//*******************************************
	//  extract negative info
	//*******************************************
	int negative = (dbl < 0.0) ? 1 : 0;
	if (negative) {
		*output++ = '-';
		dbl *= -1.0;
	}

	//**************************************************************************
	//  construct fractional multiplier for specified number of digits.
	//  Note that we draw one digit more than requested in order
	//  to handle last-digit rounding.
	//**************************************************************************
	unsigned int mult = 10;
	unsigned int idx;
	for (idx = 0; idx < dec_digits; idx++)
		mult *= 10;

	int wholeNum = (int) dbl;
	int decimalNum = (int) ((dbl - wholeNum) * mult);
	//  round off low digit
	decimalNum += 5;
	decimalNum /= 10;

	//*******************************************
	//  convert integer portion
	//*******************************************
	char tbfr[40];
	idx = 0;
	while (wholeNum != 0) {
		tbfr[idx++] = '0' + (wholeNum % 10);
		wholeNum /= 10;
	}
	if (idx == 0) {
		*output++ = '0';
	} else {
		while (idx > 0) {
			*output++ = tbfr[idx - 1];
			idx--;
		}
	}
	if (dec_digits > 0) {
		*output++ = '.';

		//*******************************************
		//  convert fractional portion
		//*******************************************
		idx = 0;
		while (decimalNum != 0) {
			tbfr[idx++] = '0' + (decimalNum % 10);
			decimalNum /= 10;
		}
		//  pad the decimal portion with 0s as necessary;
		//  We wouldn't want to report 3.093 as 3.93, would we??
		while (idx < dec_digits) {
			tbfr[idx++] = '0';
		}
		// printf("decimal=%s\n", tbfr) ;
		if (idx == 0) {
			*output++ = '0';
		} else {
			while (idx > 0) {
				*output++ = tbfr[idx - 1];
				idx--;
			}
		}
	}
	*output = 0;
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(const char *string, int width, int pad) {
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			dbgu_print_char(padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		dbgu_print_char(*string);
		++pc;
	}
	for ( ; width > 0; --width) {
		dbgu_print_char(padchar);
		++pc;
	}

	return pc;
}

// the following should be enough for 32 bit int
#define PRINT_BUF_LEN 12

static int printi(int i, int b, int sg, int width, int pad, int letbase) {
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			dbgu_print_char('-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints(s, width, pad);
}

int dbgu_printf(const char *format, ...) {
    va_list args;

    va_start(args, format);

	int post_decimal;
	int width, pad;
	unsigned dec_width = 0;
	int pc = 0;
	char scr[2];
	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0')
				break;
			if (*format == '%')
				goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			post_decimal = 0;
			if (*format == '.' || (*format >= '0' && *format <= '9')) {
				while (1) {
					if (*format == '.') {
						post_decimal = 1;
						dec_width = 0;
						format++;
					} else if ((*format >= '0' && *format <= '9')) {
						if (post_decimal) {
							dec_width *= 10;
							dec_width += *format - '0';
						} else {
							width *= 10;
							width += *format - '0';
						}
						format++;
					} else {
						break;
					}
				}
			}
			if (*format == 'l')
				++format;
			switch (*format) {
			case 's': {
				char *s = (char *)va_arg( args, char* );
				// printf("[%s] w=%u\n", s, width) ;
				pc += prints(s ? s : "(null)", width, pad);
			}
				break;
			case 'd':
				pc += printi(va_arg( args, int ), 10, 1, width, pad, 'a');
				break;
			case 'x':
				pc += printi(va_arg( args, int ), 16, 0, width, pad, 'a');
				break;
			case 'X':
				pc += printi(va_arg( args, int ), 16, 0, width, pad, 'A');
				break;
			case 'u':
				pc += printi(va_arg( args, int ), 10, 0, width, pad, 'a');
				break;
			case 'c':
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += prints(scr, width, pad);
				break;
			case 'f': {
				double dbl = va_arg(args, double);
				char bfr[81];
				dbl2stri(bfr, dbl, dec_width);
				pc += prints(bfr, width, pad);
			}
				break;
			default:
				dbgu_print_char('%');
				dbgu_print_char(*format);
				break;
			}
		} else {
			out: dbgu_print_char(*format);
			++pc;
		}
	}
	return pc;
}

void DBGUIRQHandler(void) {
//	if (status & AT91C_US_RXRDY) {
		if (dbgu_CharReceivedCallbackFnct != 0) {
			char ch = 0; //AT91C_BASE_DBGU->DBGU_RHR;
			dbgu_CharReceivedCallbackFnct(ch);
		}
//	}
}

CharReceivedCallback dbgu_AssignCharReceivedCallback(CharReceivedCallback callback) {
	 CharReceivedCallback temp;

	 temp = dbgu_CharReceivedCallbackFnct;
	 dbgu_CharReceivedCallbackFnct = callback;
	 return temp;
}

TxReadyCallback dbgu_AssignTxReadyCallback(TxReadyCallback callback) {
	 TxReadyCallback temp;

	 temp = dbgu_TxBufferEmptyCallbackFnct;
	 dbgu_TxBufferEmptyCallbackFnct = callback;
	 return temp;
}

void dbgu_ClearScreen(void) {
#ifdef WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {0, 0};
	DWORD dw;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwSize;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, ' ', dwSize, coord, &dw);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwSize, coord, &dw);
	SetConsoleCursorPosition(hConsole, coord);
#else
	dbgu_printf("\x1B[2J");
#endif
}

void dbgu_GoToCursor(int col, int row) {
#ifdef WIN32
	HANDLE screen_buffer_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(screen_buffer_handle, coord);
#else
	dbgu_printf("\x1B[%d;%dH", row + 1, col + 1);
#endif
}

#endif
