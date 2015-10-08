/*
 * LEDs.h
 *
 *  Created on: 01.07.2009
 *      Author: Stefan
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

/**
 * @deprecated LEDs was used on the Blackfin
 */
class LEDs {
public:
	LEDs();
	~LEDs();

	/**
	 * switch on LED with given index
	 * @param led		index of LED to switch on
	 */
	void on(uint8_t led);

	/**
	 * switch off LED with given index
	 * @param led		index of LED to switch off
	 */
	void off(uint8_t led);

	/**
	 * toggle LED with given index
	 * @param led		index of LED to toggle
	 */
	void toggle(uint8_t led);

	/**
	 * get the status of the LED
	 * @param led		index of LED
	 * @return true if LED is switched on
	 */
	bool status(uint8_t led) const;

private:
	int fpLED1;
	int fpLED2;
};

#endif /* LEDS_H_ */
