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

class BehaviorStatus {

public:
	BehaviorStatus();
	virtual ~BehaviorStatus();

	void setActualRole(int32_t role);
	void setActualStatus(string state);
	int32_t getActualRole() const;
	string getActualStatus() const;

private:
	int32_t actualRole;
	string actualState;

};

template<> class Serializer<BehaviorStatus> {
		public:
			static void serialize(const BehaviorStatus& representation, ostream& stream);
			static void deserialize(istream& stream, BehaviorStatus& representation);
};

#endif /* BEHAVIORSTATUS_H_ */
