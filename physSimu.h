#pragma once
#include "geometry.h"
#include "field.h"

class physSimu
{
public:
	Pt x; // 座標
	Pt v; // 速度
	double t; // 時間
	physSimu(void);
	physSimu(Pt x, double t) : x(x), t(t) {}
	~physSimu(void);
	// Fieldを読み込んだとき,ボールの運動を規定する
	void simulate(const Field& field, Real t);
};

