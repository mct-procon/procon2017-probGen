#include "Poly.h"
#include "Line.h"

//���p�`(�n�_ = �I�_)��^�����, ������\������ꏊ��`����W�ŕԂ�.
Point getDrawPosition(Poly & poly, DrawInfo *drawInfo)
{
	int n = poly.size() - 1;
	Point base = Point(0, 0);
	for (int i = 0; i < n; i++) {
		base += poly[i];
	}
	base /= n;

	Point pos;
	if (contain(poly, base) == in) {
		pos = base;
	}
	else {
		pos = poly[1] - poly[0]; pos /= abs(pos);
		if (area(poly) > 0) {
			pos *= Point(0, 1);
		}
		else {
			pos *= Point(0, -1);
		}

		//�{��
		double dist = 11451419;
		Line l = Line((poly[0] + poly[1]) * 0.5, (poly[0] + poly[1]) * 0.5 + pos * dist);
		for (int i = 1; i < n; i++) {
			if (l.ishit(Line(poly[i], poly[i + 1]))) {
				dist = min(dist, Line(poly[i], poly[i + 1]).dist((poly[0] + poly[1]) * 0.5));
			}
		}
		pos *= dist / 2;

		pos += (poly[0] + poly[1]) * 0.5;
	}
	return Point((double)drawInfo->toDrawX(pos.real()) - 8, (double)drawInfo->toDrawY(pos.imag()) - 15);
}

//�����t���ʐ�, �����v���Ȃ琳�̒l�ɂȂ�炵��
double area(Poly & poly)
{
	int n = poly.size() - 1;
	double ret = 0;
	for (int i = 0; i < n; i++) {
		ret += cross(poly[i], poly[i + 1]);
	}
	ret /= 2;
	return ret;
}

//�_point�����p�`poly�̓����Ȃ�in, �O���Ȃ�on, �O���Ȃ�out��Ԃ�
CONTAIN contain(Poly & poly, Point & point)
{
	bool isin = false;
	for (int i = 0; i < poly.size() - 1; i++) {
		Point a = poly[i] - point;
		Point b = poly[i + 1] - point;
		if (imag(a) > imag(b)) swap(a, b);
		if (imag(a) <= 0 && 0 < imag(b))
			if (cross(a, b) > 0) isin = !isin;
		if (cross(a, b) == 0 && dot(a, b) <= 0) return on;
	}
	return isin ? in : out;
}