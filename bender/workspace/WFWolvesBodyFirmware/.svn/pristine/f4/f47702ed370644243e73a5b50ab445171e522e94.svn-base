/*
 * xmodem.c
 *
 *  Created on: 16.10.2012
 *      Author: Stefan Krupop
 */

#include "xmodem.h"
#include "dbgu/dbgu.h"
#include "eeprom/eeprom.h"
#include "tc/tc.h"
#include <string.h>

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1000
#define MAXRETRANS 25

unsigned short crc16_ccitt(const unsigned char* bufptr, unsigned short len) {
	register unsigned short crc = 0;
	register int i;

	while (len--) { // calculate CRC from end to start
		crc ^= (unsigned short) (*bufptr++) << 8;
		for (i = 0; i < 8; ++i) {
			if (crc & 0x8000)
				crc = (crc << 1) ^ 0x1021;
			else
				crc <<= 1;
		}
	}
	return crc;
}

int _inbyte(unsigned short timeout) { // msec timeout
	unsigned long start = TC_GetSystemTicks();
	do {
		if (dbgu_char_available()) {
			return dbgu_get_char();
		}
	} while (TC_GetMsSinceTick(start) < timeout);
	return -1;
}

void _outbyte(int c) {
	dbgu_print_char(c);
}

static int check(int crc, const unsigned char *buf, int sz) {
	if (crc) {
		unsigned short ccrc = crc16_ccitt(buf, sz);
		unsigned short tcrc = (buf[sz] << 8) + buf[sz + 1];
		if (ccrc == tcrc)
			return 1;
	} else {
		int i;
		unsigned char cks = 0;
		for (i = 0; i < sz; ++i) {
			cks += buf[i];
		}
		if (cks == buf[sz])
			return 1;
	}

	return 0;
}

static void flushinput(void) {
	while (_inbyte(((DLY_1S) * 3) >> 1) >= 0);
}

typedef void* (*storeFunction)(void *, const void *, size_t);

int xmodemReceive_int(unsigned char *dest, int destsz, storeFunction store) {
	unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	unsigned char *p;
	int bufsz, crc = 0;
	unsigned char trychar = 'C';
	unsigned char packetno = 1;
	int i, c, len = 0;
	int retry, retrans = MAXRETRANS;

	for (;;) {
		for (retry = 0; retry < 16; ++retry) {
			if (trychar)
				_outbyte(trychar);
			if ((c = _inbyte((DLY_1S) << 1)) >= 0) {
				switch (c) {
					case SOH:
						bufsz = 128;
						goto start_recv;
					case STX:
						bufsz = 1024;
						goto start_recv;
					case EOT:
						flushinput();
						_outbyte(ACK);
						return len; /* normal end */
					case CAN:
						if ((c = _inbyte(DLY_1S)) == CAN) {
							flushinput();
							_outbyte(ACK);
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		if (trychar == 'C') {
			trychar = NAK;
			continue;
		}
		flushinput();
		_outbyte(CAN);
		_outbyte(CAN);
		_outbyte(CAN);
		return -2; /* sync error */

start_recv:
		if (trychar == 'C')
			crc = 1;
		trychar = 0;
		p = xbuff;
		*p++ = c;
		for (i = 0; i < (bufsz + (crc ? 1 : 0) + 3); ++i) {
			if ((c = _inbyte(DLY_1S)) < 0)
				goto reject;
			*p++ = c;
		}

		if (xbuff[1] == (unsigned char) (~xbuff[2]) && (xbuff[1] == packetno || xbuff[1] == (unsigned char) packetno - 1) && check(crc, &xbuff[3], bufsz)) {
			if (xbuff[1] == packetno) {
				register int count = destsz - len;
				if (count > bufsz)
					count = bufsz;
				if (count > 0) {
					store(&dest[len], &xbuff[3], count);
					len += count;
				}
				++packetno;
				retrans = MAXRETRANS + 1;
			}
			if (--retrans <= 0) {
				flushinput();
				_outbyte(CAN);
				_outbyte(CAN);
				_outbyte(CAN);
				return -3; /* too many retry error */
			}
			_outbyte(ACK);
			continue;
		}
reject:
		flushinput();
		_outbyte(NAK);
	}
	return -1; // To make Eclipse happy - should never arrive here
}

int xmodem_receive(unsigned char *dest, int destsz) {
	return xmodemReceive_int(dest, destsz, memcpy);
}

void* store_eeprom(void* pos, const void* source, size_t size) {
	eeprom_writeRaw(EEPROM_MOTION_ID, (unsigned int)(size_t)pos, (const unsigned char*)source, size);
	return NULL;
}

int xmodem_receiveEEPROM() {
	return xmodemReceive_int(0, EEPROM_MOTION_SIZE, store_eeprom);
}

int xmodemTransmit_int(unsigned char *src, int srcsz, storeFunction store) {
	unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	int bufsz, crc = -1;
	unsigned char packetno = 1;
	int i, c, len = 0;
	int retry;

	for (;;) {
		for (retry = 0; retry < 16; ++retry) {
			if ((c = _inbyte((DLY_1S) << 1)) >= 0) {
				switch (c) {
					case 'C':
						crc = 1;
						goto start_trans;
					case NAK:
						crc = 0;
						goto start_trans;
					case CAN:
						if ((c = _inbyte(DLY_1S)) == CAN) {
							_outbyte(ACK);
							flushinput();
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		_outbyte(CAN);
		_outbyte(CAN);
		_outbyte(CAN);
		flushinput();
		return -2; /* no sync */

start_trans:
		for (;;) {
			xbuff[0] = SOH;
			bufsz = 128;
			xbuff[1] = packetno;
			xbuff[2] = ~packetno;
			c = srcsz - len;
			if (c > bufsz)
				c = bufsz;
			if (c >= 0) {
				memset(&xbuff[3], 0, bufsz);
				if (c == 0) {
					xbuff[3] = CTRLZ;
				} else {
					store(&xbuff[3], &src[len], c);
					if (c < bufsz)
						xbuff[3 + c] = CTRLZ;
				}
				if (crc) {
					unsigned short ccrc = crc16_ccitt(&xbuff[3], bufsz);
					xbuff[bufsz + 3] = (ccrc >> 8) & 0xFF;
					xbuff[bufsz + 4] = ccrc & 0xFF;
				} else {
					unsigned char ccks = 0;
					for (i = 3; i < bufsz + 3; ++i) {
						ccks += xbuff[i];
					}
					xbuff[bufsz + 3] = ccks;
				}
				for (retry = 0; retry < MAXRETRANS; ++retry) {
					for (i = 0; i < bufsz + 4 + (crc ? 1 : 0); ++i) {
						_outbyte(xbuff[i]);
					}
					if ((c = _inbyte(DLY_1S)) >= 0) {
						switch (c) {
							case ACK:
								++packetno;
								len += bufsz;
								goto start_trans;
							case CAN:
								if ((c = _inbyte(DLY_1S)) == CAN) {
									_outbyte(ACK);
									flushinput();
									return -1; /* canceled by remote */
								}
								break;
							case NAK:
							default:
								break;
						}
					}
				}
				_outbyte(CAN);
				_outbyte(CAN);
				_outbyte(CAN);
				flushinput();
				return -4; /* xmit error */
			} else {
				for (retry = 0; retry < 10; ++retry) {
					_outbyte(EOT);
					if ((c = _inbyte((DLY_1S) << 1)) == ACK)
						break;
				}
				flushinput();
				return (c == ACK) ? len : -5;
			}
		}
	}
	return -1; // To make Eclipse happy
}

void* read_eeprom0(void* dest, const void* pos, size_t size) {
	eeprom_readRaw(EEPROM_CONFIG_ID, (unsigned int)(size_t)pos, (unsigned char*)dest, size);
	return NULL;
}

void* read_eeprom1(void* dest, const void* pos, size_t size) {
	eeprom_readRaw(EEPROM_MOTION_ID, (unsigned int)(size_t)pos, (unsigned char*)dest, size);
	return NULL;
}

int xmodem_sendEEPROM(unsigned char id) {
	if (id == 1) {
		return xmodemTransmit_int(0, EEPROM_MOTION_SIZE, read_eeprom1);
	} else {
		return xmodemTransmit_int(0, EEPROM_CONFIG_SIZE, read_eeprom0);
	}
}
