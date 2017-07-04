#include "Poly.h"
#include "Line.h"

//多角形(始点 = 終点)を与えると, 文字を表示する場所を描画座標で返す.
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

		//倍率
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

//符号付き面積, 反時計回りなら正の値になるらしい
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

//点pointが多角形polyの内部ならin, 外周ならon, 外側ならoutを返す
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