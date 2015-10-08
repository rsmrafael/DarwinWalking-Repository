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
#include "PanTiltUnit.h"

/*!
 * Body Class is for the translation of basic commands into the right Communication form for
 * the BodyCommunicator (Real Robot) or the SimulationCommunicator (Simulation).
 */
class Body {
public:
	/** BodyType.
	* The Possible Communicators for the commands of this class.
	*/
	enum BodyType {
		Regular = 0x00, /**< Regular means, it is the communication to the Robot */
		Simulator = 0x01 /**< Simulator is for communications with webots and other simulations */
	};

	Body(ICommunicator *comm, bool debugEnabled = true);//!<  Constructor with the Comm type and debug option
	Body(string device, BodyType type, bool noComms);//!<  Constructor Commication choose by device and type.
	~Body();//!<  Destructor for Pantilt and Comm

	void setWalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim = false);//!< Send a command to the Body to the walker to walk in x,y,z.

	void Stop();//!< Send a Stop command to the Body. The Body should stop the walker to the next time.
	void CancelKick();//!< Send a CancelKick command to the Body. End kick before end.
	void KickLeft();//!< \deprecated Send a static KickLeft to the Body.(Out of Date)
	void KickRight();//!< \deprecated Send a static KickRight to the Body.(Out of Date
	void KickBall(const vect3d_t& ball);//!< Send a Kick to the Kickengine with the Vector of the Ball.
	void KickToTarget(const vect3d_t& ball, const vect3d_t& target);//!< Send a Kick to the Kickengine with the Ball and the target Destination.
	void KickAdvanced(const vect3d_t& ball, const vect3d_t& target, uint8_t kicktype);//!<Send a Kick to the Kickengine with the Ball and the target Destination and add a Information the Kicktype.
	void GoaliMoveRight();//!< Send a static GoalieRight command to the Body for a Motion.
	void GoaliMoveLeft();//!< Send a static GoalieLeft command to the Body for a Motion.
	void MoveHi();//!< Send a static Hi command to the Body for a Motion.
	void YeahGoal();//!< Send a static YeahGoal(Both hands up) command to the Body for a Motion.
	void PlayMotion( enum ICommunicator::Motion motion);//!< Send a Motion to the Body.

	PanTiltUnit* PanTilt();//!< Get the PanTilt data.
	bool isParalysed() const;//!< Return if the Robot is allowed move (Disabled before Game).
	void setParalysis(bool value);//!< Set if the Robot is allowed to move.

	uint64_t getKickingTime() const;//!< TODO Check if needed

	double GetBatteryVoltage();//!< Returns the BatteryVoltage from StatusMessage.
	void GetIMUData(int* pitch, int* roll);//!< Return the Data from the IMU from the Statusmessage.

	const struct ICommunicator::statusMessage_t* GetStatusMessage();//!< Returns the Statusmessage send by the body.
private:
	//lint -e(1704)
    Body(const Body& cSource);//!<  Private Constructor TODO
    Body& operator=(const Body& cSource);//!<  Copy method? TODO

	PanTiltUnit* mPanTilt;/*!< A new PanTiltUnit to handle all Commands going to the PanTilt.*/
	ICommunicator* mComm;/*!< The ICommunicator as the Abstract class for the choice RealRobot<->Simulator.*/
	uint64_t mKickingTime;/*!< TODO Kicking time. Time since the last kick NOTUSED!!!!.*/
	bool mParalysis;/*!< Boolean says if the Roboter is allowed to Move.*/
	bool mDebugEnabled;/*!< If True Debugmessages will be send.*/
};

#endif /* BODY_H_ */
