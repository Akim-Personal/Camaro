#ifndef _INSTANCEPROPERTIES_H_
#define _INSTANCEPROPERTIES_H_

enum EInstanceShapes
{
	e_Rectangle		= 0,
	e_Circle			= 1,
	e_Polygon			= 2
};

struct SSolidProperties
{
	int type;
	float boxWidth;
	float boxHeight;
	float posX;
	float posY;
	float radius;
};

struct SDynamicProperties
{
	int type;
	float boxWidth;
	float boxHeight;
	float posX;
	float posY;
	float radius;
};

#endif // _INSTANCEPROPERTIES_H_