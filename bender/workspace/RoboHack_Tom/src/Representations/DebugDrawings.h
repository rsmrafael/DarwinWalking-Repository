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

class DebugDrawings: public Printable {
public:

	class DebugDrawing {
	public:
		DebugDrawing(std::string module_, DebugDrawer::drawShape type_, int x1_, int y1_, int p1_, int p2_, DebugDrawer::drawColor penColor_) {
			module = module_;
			type = type_;
			x1 = x1_;
			y1 = y1_;
			p1 = p1_;
			p2 = p2_;
			penColor = penColor_;
		}

		std::string module;
		DebugDrawer::drawShape type;
		int x1;
		int y1;
		int p1;
		int p2;
		DebugDrawer::drawColor penColor;
	};

	class DebugText {
	public:
		DebugText(std::string module_, std::string text_, int x_, int y_, DebugDrawer::drawColor penColor_)
		{
			module = module_;
			text = text_;
			x = x_;
			y = y_;
			penColor = penColor_;
		}

		std::string module;
		std::string text;
		int x;
		int y;
		DebugDrawer::drawColor penColor;
	};

	DebugDrawings();
	virtual ~DebugDrawings();

	void clear();
	void add(DebugDrawing drawing);
	void add(DebugText text);
	const std::vector<DebugDrawing>* getData() const;
	const std::vector<DebugText>* getTextData() const;

	virtual void print(std::ostream& stream) const {
	}

private:
	//lint -e(1704)
    DebugDrawings(const DebugDrawings& cSource);
    DebugDrawings& operator=(const DebugDrawings& cSource);

    std::vector<DebugDrawing>* mData;
    std::vector<DebugText>* mTextData;
};

template<>
	class Serializer<DebugDrawings> {
	public:
		static void serialize(const DebugDrawings& representation, std::ostream& stream);
		static void deserialize(std::istream& stream, DebugDrawings& representation);
	};

#endif /* DEBUGDRAWINGS_H_ */
