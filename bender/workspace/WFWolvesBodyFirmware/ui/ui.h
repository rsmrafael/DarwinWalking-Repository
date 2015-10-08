/*
 * ui.h
 *
 *  Created on: 15.03.2010
 *      Author: Stefan
 */

#ifndef UI_H_
#define UI_H_

void UI_Init(void);

void UI_SetLED(char led, unsigned char state);
void UI_ToggleLED(char led);
unsigned char UI_GetKeyState(char key);
unsigned char UI_GetKeyState_NoDebounce(char key);

#endif /* UI_H_ */