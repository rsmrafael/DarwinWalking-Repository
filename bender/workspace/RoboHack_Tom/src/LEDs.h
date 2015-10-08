/*
 * LEDs.h
 *
 *  Created on: 01.07.2009
 *      Author: Stefan
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

class LEDs {
public:
	LEDs();
	~LEDs();

	void on(uint8_t led);
	void off(uint8_t led);
	void toggle(uint8_t led);
	bool status(uint8_t led) const;

private:
	int fpLED1;
	int fpLED2;
};

#endif /* LEDS_H_ */
