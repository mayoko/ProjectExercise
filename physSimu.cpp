#include "physSimu.h"
#include "field.h"
#include "geometry.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};

const Real damp = 5;
const Real cdAccel = 5;
const Real width = 782;
const Real height = 530;

void physSimu::simulate(const Field& field, Real t) {
	// �ȑO�K�肳��Ă������x�����ɃI�u�W�F�N�g�𓮂���
	Real dt = t-this->t;
	circle.p += Pt(real(v) * dt, imag(v)*dt);
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
				if (circle.r + 0.01 > LPdist(l, circle.p)) {
					{
						Pt V = vertical(l);
						Pt a = circle.p - l.first;
						if (dot(V, a) * dot(V, v) > 0) continue;
					}
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
					v = reflection(v, l);
					break;
				}
			}
		} else if (id == Field::Board::CHANGE_DIRECTION) {
			if (contains(board.position, circle.p) == GEOMETRY_IN) {
				// ���̌����ɉ����đ��x��ω�������
				v += cdAccel*dt*Pt(dx[board.dir], dy[board.dir]);
			}
		}
	}
	// �C�ӂ̃V�~�����[�V�����ōs������:���C���󂯂đ��x�����������
	Real length = abs(v);
	if (eq(length, 0)) return;
	Real minus = min(damp, length);
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