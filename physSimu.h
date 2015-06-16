#pragma once
#include "geometry.h"
#include "field.h"

class physSimu
{
public:
	Pt x; // ���W
	Pt v; // ���x
	double t; // ����
	physSimu(void);
	physSimu(Pt x, double t) : x(x), t(t) {}
	~physSimu(void);
	// Field��ǂݍ��񂾂Ƃ�,�{�[���̉^�����K�肷��
	void simulate(const Field& field, Real t);
};

