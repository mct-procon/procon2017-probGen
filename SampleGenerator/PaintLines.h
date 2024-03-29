#pragma once
#include "Line.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "DrawInfo.h"
#include "stdafx.h"

class PaintLines
{
	Keyboard *keyboard;
	Mouse *mouse;
	DrawInfo *drawInfo;
	vector<Line> lines;
	int photoHandle;

	void drawLines();
	void drawGrid();
	void drawPhoto();
	void addLine(Point s, Point e);

public:
	PaintLines(Keyboard *keyboard, Mouse *mouse, DrawInfo *drawInfo);
	pair<int, vector<Line>> main();		//最後に押したキー, 線分集合, を返す
	void setPhotoHandle(int photoHandle);
};