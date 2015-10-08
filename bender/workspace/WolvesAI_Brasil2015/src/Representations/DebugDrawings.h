/*
 * DebugDrawings.h
 *
 *  Created on: 15.01.2013
 *      Author: Stefan Krupop
 */

#ifndef DEBUGDRAWINGS_H_
#define DEBUGDRAWINGS_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include "../Debugging/DebugDrawer.h"
#include <vector>
#include <string>

/**
 * Representation for handling the debug drawings
 */
class DebugDrawings: public Printable {
public:

	/**
	 * Representation for debug drawings like dot, circle, line, box
	 */
	class DebugDrawing {
	public:
		/**
		 * Constructor
		 * @param module_		name of module
		 * @param type_			type of shape
		 * @param x1_			x position
		 * @param y1_			y position
		 * @param p1_			parameter 1
		 * @param p2_			parameter 2
		 * @param penColor_		color of shape
		 */
		DebugDrawing(std::string module_, DebugDrawer::drawShape type_,
				int x1_, int y1_, int p1_, int p2_, DebugDrawer::drawColor penColor_)
		:	module(module_),
			type(type_),
			x1(x1_),
			y1(y1_),
			p1(p1_),
			p2(p2_),
			penColor(penColor_) {
		}

		std::string module;					//!< name of module
		DebugDrawer::drawShape type;		//!< type of shape
		int x1;								//!< position x1
		int y1;								//!< position y1
		int p1;								//!< value p1
		int p2;								//!< value p2
		DebugDrawer::drawColor penColor;	//!< color of pen
	};

	/**
	 * Representation for debug text
	 */
	class DebugText {
	public:
		/**
		 * Constructor
		 * @param module_		name of module
		 * @param text_			debugging text
		 * @param x_			x position
		 * @param y_			y position
		 * @param penColor_		color of text
		 */
		DebugText(std::string module_, std::string text_,
				int x_, int y_, DebugDrawer::drawColor penColor_)
		:	module(module_),
			text(text_),
			x(x_),
			y(y_),
			penColor(penColor_) {
		}

		std::string module;				//!< name of module
		std::string text;				//!< debugging text
		int x;							//!< position x
		int y;							//!< position y
		DebugDrawer::drawColor penColor;//!< color of displayed text
	};

	/**
	 * Constructor
	 */
	DebugDrawings();

	/**
	 * Destructor
	 */
	virtual ~DebugDrawings();

	/**
	 * clear all debug drawings and debug texts
	 */
	void clear();

	/**
	 * add a debug drawing
	 * @param drawing 	the debug drawing
	 */
	void add(DebugDrawing drawing);

	/**
	 * add a debug text
	 * @param text	the debug text
	 */
	void add(DebugText text);

	/**
	 * return pointer to container of all debug drawings
	 */
	const std::vector<DebugDrawing>* getData() const;

	/**
	 * return pointer to container of all debug texts
	 */
	const std::vector<DebugText>* getTextData() const;

	/**
	 * print to stream
	 * @param stream	stream
	 */
	virtual void print(std::ostream& stream) const {
	}

private:
	//lint -e(1704)
    DebugDrawings(const DebugDrawings& cSource);			//!< Copy-Constructor
    DebugDrawings& operator=(const DebugDrawings& cSource);	//!< assignment

    std::vector<DebugDrawing>* mData;	//!< container for debug drawings
    std::vector<DebugText>* mTextData;	//!< container for debug text objects
};

/**
 * spezialization of Serializer for DebugDrawings
 */
template<>
class Serializer<DebugDrawings> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const DebugDrawings& representation, std::ostream& stream);

	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(std::istream& stream, DebugDrawings& representation);
};

#endif /* DEBUGDRAWINGS_H_ */
