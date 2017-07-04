#pragma once
#include "Keyboard.h"

class DrawInfo {	//�l�̕ύX�͕s���l�����h�~���邽��, �u���\�b�h����āv�����Ȃ��Ă��������B
	int windowSizeX;		//��ʂ̉���
	int windowSizeY;		//��ʂ̏c��
	int defaultCellSize;	//���{����1�}�X��1�ӂ̒���
	int ORow, OCol;			//�}�X���W�ɂ����錴�_��, ��ʍ��ォ�牺��ORow�}�X�E��OCol�}�X, �̂Ƃ���Ɉʒu���Ă���B
	double transY, transX;	//���s�ړ��ʁi���s�ړ� --> �g��, �̏��Ŏ��s�j(�Ȃ����s�ړ����悩�F��ʒ��S�𒍎����Ȃ���g��, �k�����邩��j
	double scale;			//�g�嗦

public:
	DrawInfo(int windowSizeX, int windowSizeY, int defaultCellSize, int ORow, int OCol);
	int toDrawY(double cellY);
	int toDrawX(double cellX);
	int toCellY(int drawY);
	int toCellX(int drawX);
	void update(Keyboard *keyboard);
	int window_width();
	int window_height();
};