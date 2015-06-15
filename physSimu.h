#pragma once
#include "geometry.h"

class physSimu
{
public:
	Pt x; // 座標
	Pt v; // 速度
	double t; // 時間
	physSimu(void);
	physSimu(Pt x, double t) : x(x), t(t) {}
	~physSimu(void);
};

