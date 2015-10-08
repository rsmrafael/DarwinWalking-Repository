/*
 * DebugDrawer.h
 *
 *  Created on: 15.01.2013
 *      Author: Stefan Krupop
 */

#ifndef DEBUGDRAWER_H_
#define DEBUGDRAWER_H_

#include <string>
#include <map>
#include <pthread.h>

class DebugDrawings;

/**
 * Drawing tools for debug purposes
 */
class DebugDrawer {
public:
	/**
	 * available colors for debug drawings and text
	 */
	enum drawColor {
		BLACK = 0,
		DARK_GRAY = 1,
		LIGHT_GRAY = 2,
		WHITE = 3,

		RED = 4,
		ORANGE = 5,
		YELLOW = 6,
		LIME = 7,
		GREEN = 8,
		CYAN = 9,
		BLUE = 10,
		VIOLET = 11,
		PURPLE = 12,
		MAGENTA = 13,

		LIGHT_RED = 14,
		LIGHT_YELLOW = 15,
		LIGHT_GREEN = 16,
		LIGHT_BLUE = 17,
		LIGHT_PURPLE = 18,

		BROWN = 19,
		OLIVE = 20,
		TEAL = 21,
		AZURE = 22,

		DARK_RED = 23,
		DARK_GREEN = 24,
		DARK_BLUE = 25
	};

	/**
	 * available shapes for debug drawings
	 */
	enum drawShape {
		DOT = 0,
		LINE = 1,
		CIRCLE = 2,
		BOX = 3
	};

	/**
	 * get the instance to DebugDrawer (singleton)
	 */
    static DebugDrawer* GetInstance();

    /**
	 * shutdown instance and clean up
	 */
	static void shutdown();

    /**
     * draws an object
     * @param module	name of the module
     * @param type		the type: dot, line, circle, box
     * @param x1		dot, circle: center x; line, box: x (left)
     * @param y1		dot, circle: center y; line, box: y (top)
     * @param p1		circle: radius; line: x2; box: width
     * @param p2		line: y2; box: height
     * @param penColor	color of object
     */
    static void drawObject(std::string module, drawShape type, int x1, int y1, int p1, int p2, drawColor penColor);

    /**
     * draws text
     * @param module	name of the module
     * @param x			x position of text
     * @param y			y position of text
     * @param penColor	color of text
     * @param text		text to be drawn
     */
    static void drawText(std::string module, int x, int y, drawColor penColor, std::string text);

    /**
     * draws text with variable extensions
     * @param module	name of the module
     * @param x			x position of text
     * @param y			y position of text
     * @param penColor	color of text
     * @param text		text to be drawn
     */
    static void drawText(std::string module, int x, int y, drawColor penColor, const char *text, ...);

    /**
     * start a new cycle, clear old drawings
     */
    static void newCycle();

protected:
    /**
     * private constructor
     */
	DebugDrawer();

	/**
	 * private destructor
	 */
	virtual ~DebugDrawer();

	/**
	 * pointer to DebugDrawings
	 */
	DebugDrawings* mDrawings;
private:
	pthread_mutex_t mMutex;

	static DebugDrawer* mInstance;
};

/// Makro for drawing text
#define DRAW_TEXT DebugDrawer::drawText
/// Makro for drawing a line
#define DRAW_LINE(module, x1, y1, x2, y2, penColor) DebugDrawer::drawObject(module, DebugDrawer::LINE, x1, y1, x2, y2, penColor)
/// Makro for drawing a circle
#define DRAW_CIRCLE(module, x1, y1, radius, penColor) DebugDrawer::drawObject(module, DebugDrawer::CIRCLE, x1, y1, radius, 0, penColor)
/// Makro for drawing a dot
#define DRAW_POINT(module, x1, y1, penColor) DebugDrawer::drawObject(module, DebugDrawer::DOT, x1, y1, 0, 0, penColor)
/// Makro for drawing a rectangle
#define DRAW_BOX(module, x1, y1, width, height, penColor) DebugDrawer::drawObject(module, DebugDrawer::BOX, x1, y1, width, height, penColor)

#endif /* DEBUGDRAWER_H_ */
