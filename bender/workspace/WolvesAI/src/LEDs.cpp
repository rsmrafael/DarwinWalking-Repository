/*
 * LEDs.cpp
 *
 *  Created on: 01.07.2009
 *      Author: Stefan
 */

#include "LEDs.h"

#include <iostream>
#include <fcntl.h>
#include "Debugging/Debugger.h"

using namespace std;

//#define USE_LED_GPIOS

LEDs::LEDs() {
#ifdef USE_LED_GPIOS
	Debugger::DEBUG("LEDs", "Opening handles to GPIOs...");
	fpLED1 = open("/sys/class/gpio/gpio42/value", O_RDWR);
	if (!fpLED1) {
		cout << "Could not open /sys/class/gpio/gpio42/value!" << endl;
	}
	fpLED2 = open("/dev/gpio45", O_RDWR);
	if (!fpLED2) {
		cout << "Could not open /dev/gpio45!" << endl;
	}
	Debugger::DEBUG("LEDs", "Setting output mode...");
/*	if (fpLED1 > 0) {
		if (write(fpLED1, "O1", 2) != 2) {
			cout << "Could not set output mode for /dev/gpio42!" << endl;
		}
#ifdef TARGET_BLACKFIN
		fsync(fpLED1);
#endif
	}
*/
	if (fpLED2 > 0) {
		if (write(fpLED2, "O1", (size_t)2) != 2) {
			cout << "Could not set output mode for /dev/gpio45!" << endl;
		}
#ifdef TARGET_BLACKFIN
		fsync(fpLED2);
#endif
	}
#else
	fpLED1 = 0;
	fpLED2 = 0;
#endif
}

LEDs::~LEDs() {
#ifdef USE_LED_GPIOS
	Debugger::DEBUG("LEDs", "Closing GPIO handles...");
	if (fpLED1 > 0) {
		close(fpLED1);
	}
	if (fpLED2 > 0) {
		close(fpLED2);
	}
#endif
}

void LEDs::on(uint8_t led) {
#ifdef USE_LED_GPIOS
	if (led == 1) {
		if (fpLED2 > 0) {
			if (write(fpLED2, "0", (size_t)1) != 1) {
				cout << "Could not switch on /dev/gpio45!" << endl;
			}
#ifdef TARGET_BLACKFIN
			fsync(fpLED2);
#endif
		}
	} else {
		if (fpLED1 > 0) {
			if (write(fpLED1, "0", (size_t)1) != 1) {
				cout << "Could not switch on GPIO 42!" << endl;
			}
#ifdef TARGET_BLACKFIN
			fsync(fpLED1);
#endif
		}
	}
#else
	if (led == 1) {
		fpLED2 = 1;
	} else {
		fpLED1 = 1;
	}
#endif
}

void LEDs::off(uint8_t led) {
#ifdef USE_LED_GPIOS
	if (led == 1) {
		if (fpLED2 > 0) {
			if (write(fpLED2, "1", (size_t)1) != 1) {
				cout << "Could not switch off /dev/gpio45!" << endl;
			}
#ifdef TARGET_BLACKFIN
			fsync(fpLED2);
#endif
		}
	} else {
		if (fpLED1 > 0) {
			if (write(fpLED1, "1", (size_t)1) != 1) {
				cout << "Could not switch off GPIO 42!" << endl;
			}
#ifdef TARGET_BLACKFIN
			fsync(fpLED1);
#endif
		}
	}
#else
	if (led == 1) {
		fpLED2 = 0;
	} else {
		fpLED1 = 0;
	}
#endif
}

void LEDs::toggle(uint8_t led) {
#ifdef USE_LED_GPIOS
	uint8_t buf[2];

	if (led == 1) {
		if (fpLED2 > 0) {
			if (write(fpLED2, "T", (size_t)1) != 1) {
				cout << "Could not toggle /dev/gpio45!" << endl;
			}
#ifdef TARGET_BLACKFIN
			fsync(fpLED2);
#endif
		}
	} else {
		if (fpLED1 > 0) {
			lseek(fpLED1, (off_t)0, SEEK_SET);
			if (read(fpLED1, &buf[0], (size_t)1) != 1) {
				cout << "Could not toggle GPIO 42 (read)!" << endl;
				return;
			}
			if (buf[0] == '1') {
				buf[0] = '0';
			} else {
				buf[0] = '1';
			}
			buf[1] = 0;
			if (write(fpLED1, &buf[0], (size_t)1) != 1) {
				cout << "Could not toggle GPIO 42 (write)!" << endl;
			}
#ifdef TARGET_BLACKFIN
			fsync(fpLED1);
#endif
		}
	}
#else
	if (led == 1) {
		fpLED2 = !fpLED2;
	} else {
		fpLED1 = !fpLED1;
	}
#endif
}

bool LEDs::status(uint8_t led) const {
#ifdef USE_LED_GPIOS
	char data = 1;

	if (led == 1) {
		if (fpLED2 > 0) {
			if (read(fpLED2, &data, (size_t)1) != 1) {
				cout << "Could not read status for /dev/gpio45!" << endl;
			}
		}
	} else {
		if (fpLED1 > 0) {
			if (read(fpLED1, &data, (size_t)1) != 1) {
				cout << "Could not read status for /dev/gpio42!" << endl;
			}
		}
	}

	return (data == 0);
#else
	if (led == 1) {
		return (fpLED2 == 1);
	} else {
		return (fpLED1 == 1);
	}
#endif
}
