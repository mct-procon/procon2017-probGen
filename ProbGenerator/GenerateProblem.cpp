#include "GenerateProblem.h"
#include "DxLib.h"
#include "Line.h"
#include <cmath>
#include <cassert>

//�R���X�g���N�^
GenerateProblem::GenerateProblem(DrawInfo * drawInfo, Keyboard * keyboard)
{
	this->drawInfo = drawInfo;
	this->keyboard = keyboard;
}

//���C��
tuple<int, vector<Poly>, vector<Poly>> GenerateProblem::main(vector<Poly> wakus, vector<Poly> pieces, bool canReverse)
{
	for (int i = 0; i < pieces.size(); i++) { pieces[i] = convertPoly(pieces[i], canReverse); }

	//���ʕ\��
	int endKeycode[3] = { KEY_INPUT_ESCAPE, KEY_INPUT_LEFT, KEY_INPUT_RIGHT };		//�I���L�[�̏W��. ���ԓ���ւ���.

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		keyboard->update();
		drawInfo->update(keyboard);

		for (int i = 0; i < 3; i++) {
			if (keyboard->is_click(endKeycode[i])) {
				return tuple<int, vector<Poly>, vector<Poly>>(endKeycode[i], wakus, pieces);
			}
		}
		draw(wakus, pieces);
	}

	return tuple<int, vector<Poly>, vector<Poly>>(endKeycode[0], wakus, pieces);
}

//�`��
void GenerateProblem::draw(vector<Poly>& wakus, vector<Poly>& pieces)
{
	//�s�[�X
	for (int i = 0; i < pieces.size(); i++) {
		Point pos = getDrawPosition(pieces[i], drawInfo);
		DrawFormatString(pos.real(), pos.imag(), GetColor(0, 0, 255), "%d", i);
		for (int j = 0; j < pieces[i].size() - 1; j++) {
			Line(pieces[i][j], pieces[i][j + 1]).draw(drawInfo, GetColor(0, 255, 0));
		}
	}
	//�g��
	for (int i = 0; i < wakus.size(); i++) {
		for (int j = 0; j < (int)wakus[i].size() - 1; j++) {
			Line(wakus[i][j], wakus[i][j + 1]).draw(drawInfo, GetColor(255, 0, 255));
		}
	}

	//��
	DrawFormatString(drawInfo->window_width() - 300, drawInfo->window_height() - 110, GetColor(255, 0, 255), "�g�̌� = %d", wakus.size());
	DrawFormatString(drawInfo->window_width() - 300, drawInfo->window_height() - 70, GetColor(0, 255, 0), "�s�[�X�̌� = %d", pieces.size());
}


//���p�`�̃����_�����s�ړ��E��]�E���]�B(canReverse = false�Ȃ�, ���]���Ȃ�)
Poly GenerateProblem::convertPoly(Poly poly, bool canReverse)
{
	int i;

	//���]
	if (canReverse) {
		if (GetRand(1) == 0) {
			poly = reverse(poly, poly[0].real());
		}
	}

	//��]
	vector<Point> kohoDirs = getKohoDirs(poly[1] - poly[0]);
	vector<Point> muls;

	for (i = 0; i < kohoDirs.size(); i++) {
		Point mul = kohoDirs[i] / (poly[1] - poly[0]);
		Poly newPoly = rotate(poly, poly[0], mul);

		int j;
		for (j = 0; j < poly.size(); j++) {
			if (newPoly[j].real() != (int)newPoly[j].real()) break;
			if (newPoly[j].imag() != (int)newPoly[j].imag()) break;
		}
		if (j == poly.size()) {
			muls.push_back(mul);
		}
	}
	if (muls.size() == 0) { assert("��]��₪����܂���."); }
	int id = GetRand(muls.size() - 1);
	poly = rotate(poly, poly[0], muls[id]);

	//���s�ړ�
	int transX = GetRand(50) - 25;
	int transY = GetRand(50) - 25;
	poly = transrate(poly, Point((double)transX, (double)transY));

	return poly;
}

//dir(�����x�N�g��)��{�����x�N�g��}�Ɏʂ��B
vector<Point> GenerateProblem::getKohoDirs(Point dir)
{
	vector<Point> ret;
	int r = abs(dir) + 1e-8;
	for (int x = -r; x <= r; x++) {
		double y = sqrt((double)norm(dir) - x * x) + 1e-10;
		if (y - (int)y < 1e-9) {
			ret.push_back(Point((double)x, (double)((int)y)));
			if ((int)y > 0) {
				ret.push_back(Point((double)x, (double)((int)-y)));
			}
		}
	}
	return ret;
}

//���p�`poly����]����.
Poly GenerateProblem::rotate(Poly poly, Point center, Point mul)
{
	for (int i = 0; i < poly.size(); i++) {
		poly[i] -= center;
		poly[i] *= mul;
		poly[i] += center;
	}
	return poly;
}

//���p�`poly�𕽍s�ړ�����
Poly GenerateProblem::transrate(Poly poly, Point trans)
{
	for (int i = 0; i < poly.size(); i++) {
		poly[i] += trans;
	}
	return poly;
}

//���p�`poly��x = centerX��Ώ̎��Ƃ��Ĕ��]����
Poly GenerateProblem::reverse(Poly poly, int centerX)
{
	for (int i = 0; i < poly.size(); i++) {
		poly[i] = Point(2.0 * centerX - poly[i].real(), poly[i].imag());
	}
	
	int l = 0, r = poly.size() - 1;
	while (l < r) {
		swap(poly[l], poly[r]);
		l++;
		r--;
	}

	return poly;
}
