/*
 * xmodem.h
 *
 *  Created on: 16.10.2012
 *      Author: Stefan Krupop
 */

#ifndef XMODEM_H_
#define XMODEM_H_

int xmodem_receive(unsigned char *dest, int destsz);
int xmodem_receiveEEPROM(void);
int xmodem_sendEEPROM(unsigned char id);

#endif /* XMODEM_H_ */
