//�E����(Real�����j���牺����(Imag����)�ɓ|���������u�����v���v�ƒ�`����B
//�����ڏ�́A�t�ɂȂ�̂Œ��ӁB
#pragma once
#include "stdafx.h"
typedef complex<double> Point;

bool operator<(const Point &a, const Point &b);
double cross(Point &a, Point &b);
double dot(Point &a, Point &b);
int ccw(Point a, Point b, Point c);
