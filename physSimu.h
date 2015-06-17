#pragma once
#include "geometry.h"
#include "field.h"

class physSimu
{
public:
	Cir circle; // ���W����є��a
	Pt v; // ���x
	Real t; // ����
	// �����V�~�����[�V�����֌W�Ȃ��ʒu,���x��ύX����
	void changeState(Real x, Real y, Real vx, Real vy);
	// Field����ю��Ԃ�ǂݍ��񂾂Ƃ�,�{�[���̉^�����K�肷��
	void simulate(const Field& field, Real t);
	// �{�[���̉^�����L�q����(�f�o�b�O�p)
	void print() const ;
};