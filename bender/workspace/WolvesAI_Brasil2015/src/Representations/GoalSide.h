/*
 * GoalSide.h
 *
 *  Created on: 25.03.2014
 *      Author: Oliver Krebs
 */

#ifndef GOALSIDE_H_
#define GOALSIDE_H_

/**
 * Representation of a goal side
 */
class GoalSide
{
public:
	/**
	 * possible goal sides
	 */
	enum eGoalSide {
		UNKNOWN_GOAL,	//!< goal side unknown, not set
		MY_GOAL,		//!< own goal side
		OP_GOAL			//!< opponent goal side
	};

	/**
	 * Constructor
	 */
	GoalSide();

	/**
	 * Destructor
	 */
	virtual ~GoalSide();

	/**
	 * set minimal interval
	 * @param min	value for decide its my or opponent goal (0.0 to 0.5)
	 */
	void setMinInterval( double min);

	/**
	 * set accuracy for goal side
	 * @param accuracy 	accuracy for goal side (0.0 to 1.0)
	 */
	void setAccuracy( double accuracy);

	/**
	 * set probability for opponent's goal
	 * @param probOpGoal 	probability for opponent goal (0.0 to 1.0)
	 */
	void setProbability(double probOpGoal);

	/**
	 * return probability for opponent goal side (0.0 to 1.0)
	 */
	double getProbabilityOpGoal() const;

	/**
	 * return probability for own goal side (0.0 to 1.0)
	 */
	double getProbabilityMyGoal() const;

	/**
	 * return accuracy for goal side (0.0 to 1.0)
	 */
	double getAccuracy() const;

	/**
	 * return goal side
	 */
	eGoalSide getGoalSide() const;

private:
	eGoalSide mGoalSideChance;	//!< the predicted goal side
	double mProbabilityOpGoal;	//!< probability for OP_GOAL (0.0 to 1.0)
	double mMinInterval;		//!< minimal interval for decide its my / op goal
	double mAccuracy;			//!< accuracy of goal side
};


#endif /* GOALSIDE_H_ */
