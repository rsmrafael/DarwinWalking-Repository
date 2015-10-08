/*
 * FieldDetection.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: Jan Carstensen
 *      Edited: Oliver Krebs
 */

#include "FieldDetection.h"
#include "../../Debugging/DebugDrawer.h"
#include "../../Utils/Clustering/Ransac/Ransac.h"
#include "../../Utils/Clustering/Ransac/Models/LineModel.h"
#include "../../Config.h"
#include <map>

FieldDetection::FieldDetection()
:	mNumIterations( 200),
	mMinPoints( 8),
	mMaxError( 2.0f),
	mShowDebugDrawings( false)
{
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

FieldDetection::~FieldDetection() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool FieldDetection::execute() {



	const YUVImage* image = &getImage();
	int width = image->getWidth();
	int height = image->getHeight();

	 //Declaration of minimum Rows that are in the Field
	 //int minNum = height;

	list<Point>* greenPoints = const_cast<list<Point>*>(getGreenPoints().getCloud());
	getFieldLines().clearPoints();

	size_t iterations = mNumIterations;

	LineModel *line;
	Ransac ransac( greenPoints, new LineModel(), iterations, mMaxError);

	map<int, LineModel*> lines;
	Point p1, p2;

	for( int i = 0; i < 3; i++) {
		line = dynamic_cast<LineModel*>(ransac.run(mMinPoints));
		if( line != NULL) {
			//p1 = Point(0, line->getY(0));
			//p2 = Point(width - 1, line->getY(width - 1));

			//p1 = line->calcLeftPoint(greenPoints, mMaxError);
			//p2 = line->calcRightPoint(greenPoints, mMaxError);

			p1 = line->getStartPoint();
			p2 = line->getEndPoint();

			line->eraseFittedPoints(greenPoints, mMaxError);
			auto entry = std::pair<int, LineModel*>(p1.getX(), line);
			lines.insert(entry);
			iterations /= 2;
			ransac.setNumberOfIterations( iterations);
		} else {
			if( i == 0) {
				// no line found, so use bottom of image (nothing is in field)
				p1 = Point(0, height - 1);
				p2 = Point(width - 1, height - 1);
				getFieldLines().addEdgePoint(p1);
				getFieldLines().addEdgePoint(p2);
#ifdef _DEBUG
				if( mShowDebugDrawings) {
					DRAW_LINE("FieldBorder", p1.getX(), p1.getY(),
						p2.getX(), p2.getY(), DebugDrawer::GREEN);
				}
#endif
				return true;
			}
			break;
		}
	}

	int lastX = 0, lastY = 0;
	Point p3, p4;
	map<int, LineModel*>::iterator lineIt1, lineIt2;
	for( lineIt1 = lines.begin(); lineIt1 != lines.end(); ++lineIt1) {
		int x, y;
		if( lineIt1 == lines.begin()) {
			lastX = 0;
			lastY = lineIt1->second->getY(0);
			getFieldLines().addEdgePoint(Point(lastX, lastY));
		}
		lineIt2 = lineIt1;
		if( ++lineIt2 == lines.end()) {
			x = width;
			y = lineIt1->second->getY(width);
			getFieldLines().addEdgePoint(Point(x, y));
#ifdef _DEBUG
			if( mShowDebugDrawings) {
				DRAW_LINE("FieldBorder", lastX, lastY,
					x, y, DebugDrawer::GREEN);
			}
#endif
			delete lineIt1->second;
			break;
		}

		double dx, dy, b1, b2, m1, m2;
		p1 = lineIt1->second->getStartPoint();
		p2 = lineIt1->second->getEndPoint();
		dy = p2.getY() - p1.getY();
		dx = p2.getX() - p1.getX();
		m1 = dy / dx;
		b1 = p1.getY() - (m1 * p1.getX());

		p3 = lineIt2->second->getStartPoint();
		p4 = lineIt2->second->getEndPoint();
		dy = p4.getY() - p3.getY();
		dx = p4.getX() - p3.getX();
		m2 = dy / dx;
		b2 = p3.getY() - (m2 * p3.getX());

		if(m1==m2) {
			if(b1 < b2) {
				x = p2.getX();
				y = p2.getY();
			} else {
				x = p4.getX();
				y = p4.getY();
			}
		} else {
			x = (int)((b2 - b1) / (m1 - m2));
			y = (int)(m1 * x + b1);
		}
		getFieldLines().addEdgePoint(Point(x, y));
		//Look for biggest y
		/*int usedY = y;
		 if(minNum>usedY){
			 if(y<0){
				 usedY = 0;
			 }
			 minNum=usedY;
		 }*/
#ifdef _DEBUG
		if( mShowDebugDrawings) {
			DRAW_LINE("FieldBorder", lastX, lastY,
				x, y, DebugDrawer::GREEN);
			lastX = x;
			lastY = y;
		}
#endif
		delete lineIt1->second;
	}
	//ToDo Make it Work
	//Now cut Image with minimum rows
	//if(minNum<30){
	//	minNum = 30;
	//}
	//char* data =image->getData();
	//getCuttetImage().copyImage(data,image->getWidth(),image->getHeight(),minNum);
	return true;
}

void FieldDetection::configChanged() {
	Debugger::INFO("FieldDetection", "config changed");

	ConfigFile *config = Config::getInstance();
	mShowDebugDrawings	= config->get<bool>("Vision", "FieldBorder_debugDrawings", true);
	mNumIterations	= config->get<size_t>("Vision", "FieldBorder_numIterations", (size_t)100);//100
	mMinPoints		= config->get<size_t>("Vision", "FieldBorder_minPoints", (size_t)6);//6//8
	mMaxError		= config->get<double>("Vision", "FieldBorder_maxError", 2.0f);
}
