/*
 * GoalSide.h
 *
 *  Created on: 25.03.2014
 *      Author: Oliver Krebs
 */

#ifndef GOALSIDE_H_
#define GOALSIDE_H_

class GoalSide
{
public:
	enum eGoalSide {
		UNKNOWN_GOAL,
		MY_GOAL,
		OP_GOAL
	};

	GoalSide();
	virtual ~GoalSide();

	void setMinInterval( double min);
	void setAccuracy( double accuracy);

	// set probability for opponent's goal
	void setProbability(double probOpGoal);

	double getProbabilityOpGoal() const;
	double getProbabilityMyGoal() const;
	double getAccuracy() const;

	eGoalSide getGoalSide() const;

private:
	eGoalSide mGoalSideChance;
	double mProbabilityOpGoal;	// probability for OP_GOAL (0.0 - 1.0)
	double mMinInterval;		// minimal interval for decide its my / op goal
	double mAccuracy;
};


#endif /* GOALSIDE_H_ */
