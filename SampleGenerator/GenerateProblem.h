#pragma once
#include "stdafx.h"
#include "Point.h"
#include "DrawInfo.h"
#include "Keyboard.h"
#include "Poly.h"

class GenerateProblem {
	DrawInfo *drawInfo; 
	Keyboard *keyboard;

public:
	GenerateProblem(DrawInfo *drawInfo, Keyboard *keyboard);
	tuple<int, vector<Poly>, vector<Poly>> main(vector<Poly> wakus, vector<Poly> pieces, bool canReverse);
	
private:
	void draw(vector<Poly> &wakus, vector<Poly> &pieces);
	
	Poly convertPoly(Poly poly, bool canReverse);
	vector<Point> getKohoDirs(Point dir);
	Poly rotate(Poly poly, Point center, Point mul);
	Poly transrate(Poly poly, Point trans);
	Poly reverse(Poly poly, int centerX);
};