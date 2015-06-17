#include "physSimu.h"
#include "field.h"
#include "geometry.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

void physSimu::simulate(const Field& field, Real t) {
	// �ȑO�K�肳��Ă������x�����ɃI�u�W�F�N�g�𓮂���
	circle.p += Pt(real(v) * (t-this->t), imag(v)*(t-this->t));
	this->t = t;
	// firld�ɂ�����Ă���e�I�u�W�F�N�g�ɑ΂��č�p������̂�����΂��̂悤�ɓ�����
	int n = field.boards.size();
	for (Field::Board board : field.boards) {
		int id = board.id;
		if (id == Field::Board::OBSTACLE) {
			// �~�ƒ�����������Ă����璵�˕Ԃ�
			//! ���_�����v���,�܂��͔����v���ɕ���ł��邱�Ƃ�O��Ƃ��Ă���
			for (int i = 0; i < 4; i++) {
				bool flag = false;
				Line l;
				l.first = board.position[i];
				l.second = board.position[(i+1)%4];
				if (circle.r > LPdist(l, circle.p)) {
					vector<Pt> intersect = circle_line_intersect(l, circle);
					for (Pt p : intersect) {
						if (eq(0, SPdist(l, p))) {
							flag = true;
							break;
						}
					}
				}
				if (flag) {
					// �{�[���������ɂԂ������Ƃ����˕Ԃ鏈��

					break;
				}
			}
		} else if (id == Field::Board::CHANGE_DIRECTION) {
			// ���̌����ɉ����đ��x��ω�������
		}
	}
	// �C�ӂ̃V�~�����[�V�����ōs������:���C���󂯂đ��x�����������
	Real length = abs(v);
	if (eq(length, 0)) return;
	Real minus = min(0.1, length);
	v *= (length-minus) / length;
}

void physSimu::changeState(Real x, Real y, Real vx, Real vy) {
	circle.p = Pt(x, y);
	v = Pt(vx, vy);
}

void physSimu::print() const {
	cout << t << " " << circle.r << " #t, r" << endl;
	cout << real(circle.p) << " " << imag(circle.p) << " #x, y" << endl;
	cout << real(v) << " " << imag(v) << " #vx, vy" << endl;
}