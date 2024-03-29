#pragma once
#include "stdafx.h"
#include "Point.h"
#include "DrawInfo.h"

typedef vector<Point> Poly;

enum CONTAIN { out, on, in };	//劣悪enum

Point getDrawPosition(Poly & poly, DrawInfo *drawInfo);
double area(Poly &poly);
CONTAIN contain(Poly &poly, Point &point);			//点pointがpolyの外周…out