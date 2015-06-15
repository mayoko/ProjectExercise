#pragma once
#include "geometry.h"

class physSimu
{
public:
	Pt x; // ç¿ïW
	Pt v; // ë¨ìx
	double t; // éûä‘
	physSimu(void);
	physSimu(Pt x, double t) : x(x), t(t) {}
	~physSimu(void);
};

