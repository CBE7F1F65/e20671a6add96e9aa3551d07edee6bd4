/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeRect helper class implementation
*/


#include "hgerect.h"
#include <math.h>


void hgeRect::Encapsulate(float x, float y)
{
	if(bClean)
	{
		x1=x2=x;
		y1=y2=y;
		bClean=0;
	}
	else
	{
		if(x<x1) x1=x;
		if(x>x2) x2=x;
		if(y<y1) y1=y;
		if(y>y2) y2=y;
	}
}

BOOLu8 hgeRect::TestPoint(float x, float y) const
{
	if(x>=x1 && x<x2 && y>=y1 && y<y2) return 1;

	return 0;
}

BOOLu8 hgeRect::Intersect(const hgeRect *rect) const
{
	if(fabs(x1 + x2 - rect->x1 - rect->x2) < (x2 - x1 + rect->x2 - rect->x1))
		if(fabs(y1 + y2 - rect->y1 - rect->y2) < (y2 - y1 + rect->y2 - rect->y1))
			return 1;

	return 0;
}