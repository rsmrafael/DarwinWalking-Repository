/*
 * BehaviorStatus.h
 *
 *  Created on: 28.04.2013
 *      Author: 7
 */



#ifndef BEHAVIORSTATUS_H_
#define BEHAVIORSTATUS_H_

#include <string>
#include "../Blackboard/Serializer.h"
#include "../Body/ICommunicator.h"
#include "../Blackboard/Printable.h"

/**
 * Representation for status of behavior
 */
class BehaviorStatus {

public:
	/**
	 * Constructor
	 */
	BehaviorStatus();

	/**
	 * Destructor
	 */
	virtual ~BehaviorStatus();

	/**
	 * set the current role
	 * @param role		index of role
	 */
	void setActualRole(int32_t role);

	/**
	 * set the current status
	 * @param state		name of state
	 */
	void setActualStatus(string state);

	/**
	 * return the current role
	 */
	int32_t getActualRole() const;

	/**
	 * return the current status
	 */
	string getActualStatus() const;

private:
	int32_t actualRole;		//!< index of the current role
	string actualState;		//!< the name of the current state

};

/**
 * spezialization of Serializer for BehaviorStatus
 */
template<> class Serializer<BehaviorStatus> {
	public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const BehaviorStatus& representation, ostream& stream);

	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, BehaviorStatus& representation);
};

#endif /* BEHAVIORSTATUS_H_ */
