/*
 * Body.h
 *
 *  Created on: 17.06.2009
 *      Author: Stefan
 */

#ifndef BODY_H_
#define BODY_H_

using namespace std;

#include <string>
#include "ICommunicator.h"
#include "Communicator.h"

class Body {
public:
	enum BodyType { Regular = 0x00 };

	Body(string device, BodyType type);
	~Body();

	void setWalkerCommand(signed char x, signed char y, signed char r);
	void setWalkerCommand(signed char x, signed char y, signed char r, bool turningAim);

	void Stop();
	void KickLeft();
	void KickRight();
	void GoaliMoveRight();
	void GoaliMoveLeft();
	void MoveHi();
	void YeahGoal();

	bool isParalysed() const;
	void setParalysis(bool value);

	void startBootloader();

	double GetBatteryVoltage();
	void GetIMUData(int* pitch, int* roll);

	const struct ICommunicator::statusMessage_t* GetStatusMessage();
private:
	//lint -e(1704)
    Body(const Body& cSource);
    Body& operator=(const Body& cSource);

	ICommunicator* mComm;
	bool mParalysis;
};

#endif /* BODY_H_ */
