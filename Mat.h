#pragma once
#include <vector>
class Mat
{
public:
	Mat(void);
	Mat(int n, int m);
	~Mat(void);
	std::vector<std::vector<double> > mat;
	Mat mul(const Mat& A);
};