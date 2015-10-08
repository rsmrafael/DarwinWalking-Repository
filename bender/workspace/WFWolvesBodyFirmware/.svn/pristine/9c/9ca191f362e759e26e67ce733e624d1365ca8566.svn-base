/*
 * Odemetry.c
 *
 *  Created on: 28.01.2013
 *      Author: Tobias Bolze
 */
#include "odometry.h"
#include "dbgu/dbgu.h"
#include "eeprom/eeprom.h"
#include <math.h>

struct position positionV;
/*
 * init the Position
 */
void odometry_initOdemetry(void){
	positionV.yPos=0.00;
	positionV.xPos=0.00;
	positionV.viewDirection=0.00;
	positionV.weight=100;
	positionV.changed = 0;
	ODOMETRY_XCHANGE = 0.8;
	ODOMETRY_VIEWCHANGE = 1.0;
	ODOMETRY_YCHANGE = 0.5;
	//eeprom_read("ODOMETRY", &positionV, sizeof(positionV));
}
void odometry_setxChange(float xChange)
{
	ODOMETRY_XCHANGE = xChange;
}
void odometry_setyChange(float yChange)
{
	ODOMETRY_YCHANGE = yChange;
}
void odometry_setViewChange(float viewChange)
{
	ODOMETRY_VIEWCHANGE = viewChange;
}
void odometry_saveChanges()
{
	eeprom_write("ODOMETRY", &positionV, sizeof(positionV));
}
/*
 * Calculate new Position with Given Walker Configurations
 */
void odometry_calculateWalkerPos(float xChange,float yChange,float directionChange )
{

	xChange *= ODOMETRY_XCHANGE;
	yChange *= ODOMETRY_YCHANGE;
	directionChange *= ODOMETRY_VIEWCHANGE;
	odometry_setViewDirection(directionChange);
	positionV.xPos += xChange;
	positionV.yPos += yChange;
	/*float z = 1;
	float view = 1;
	if(yChange==0)
	{
		z=xChange;
		view = positionV.viewDirection;
	}else if(xChange*xChange+yChange*yChange>0)
	{
		z = sqrt(xChange*xChange+yChange*yChange);
		view = positionV.viewDirection+ asin(yChange/z);
	}
	if(positionV.viewDirection>-90&&positionV.viewDirection<0)
	{
		positionV.xPos -= sin(view)*z;
		positionV.yPos += cos(view)*z;
	}else if(positionV.viewDirection>0&&positionV.viewDirection<90)
	{
		positionV.xPos += sin(view)*z;
		positionV.yPos += cos(view)*z;

	}else if(positionV.viewDirection<-90&&positionV.viewDirection>-180)
	{
		positionV.xPos -= sin(view)*z;
		positionV.yPos -= cos(view)*z;
	}else if(positionV.viewDirection<180&&positionV.viewDirection>90)
	{
		positionV.xPos += sin(view)*z;
		positionV.yPos -= cos(view)*z;
	}
	else if(positionV.viewDirection==90)
	{
		positionV.xPos += yChange;
		positionV.yPos += xChange;
	}else if(positionV.viewDirection==-90)
	{
		positionV.xPos -= yChange;
		positionV.yPos += xChange;
	}else if(positionV.viewDirection==0)
	{
		positionV.xPos += xChange;
		positionV.yPos += yChange;
	}else if(positionV.viewDirection==180||positionV.viewDirection==-180)
	{
		positionV.xPos += yChange;
		positionV.yPos -= xChange;
	}*/
	dbgu_printf("xChange:%f, yChange:%f, directionChange:%d\r\n",positionV.xPos,positionV.yPos,(int)positionV.viewDirection);

}
void odometry_setViewDirection(float direction)
{
	int newDirection = positionV.viewDirection + direction;
	if(newDirection>180.0)
	{
		newDirection = -180.0+ (newDirection-180.0);
	}else if(newDirection<-180.0)
	{
		newDirection = 180.0+ (newDirection+180.0);
	}
	positionV.viewDirection = newDirection;
}
/*
 * Calculate State with the Informations of the new State
 */
void odometry_changeWeight(char change)
{
	odometry_setWeight(odometry_getWeight()+change);
}
void odometry_setWeight(unsigned char weight)
{
	positionV.weight=weight;
}
unsigned char odometry_getWeight(void)
{
	return positionV.weight;
}
/*
 * Resets the Position
 */
void odometry_reset(void)
{
	positionV.yPos=0.00;
	positionV.xPos=0.00;
	positionV.viewDirection=0.00;
	positionV.weight=100;
}

struct position* odometry_getPosition()
{
	return &positionV;
}



