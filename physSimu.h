#pragma once
#include "geometry.h"

class physSimu
{
public:
	Pt x; // ���W
	Pt v; // ���x
	double t; // ����
	physSimu(void);
	physSimu(Pt x, double t) : x(x), t(t) {}
	~physSimu(void);
};

