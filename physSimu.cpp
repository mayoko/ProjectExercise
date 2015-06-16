#include "physSimu.h"
#include "field.h"
#include "geometry.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

void physSimu::simulate(const Field& field, Real t) {
	// 以前規定されていた速度を元にオブジェクトを動かす
	circle.p += Pt(real(v) * (t-this->t), imag(v)*(t-this->t));
	this->t = t;
	// firldにおかれている各オブジェクトに対して作用するものがあればそのように動かす
	int n = field.boards.size();
	for (Field::Board board : field.boards) {
		int id = board.id;
		if (id == Field::Board::OBSTACLE) {
			// 円と直線が交わっていたら跳ね返る
			//! 頂点が時計回り,または反時計回りに並んでいることを前提としている
			for (int i = 0; i < 4; i++) {
				bool flag = false;
				Line l;
				l.first = board.position[i];
				l.second = board.position[(i+1)%4];
				if (circle.r + 0.01 > LPdist(l, circle.p)) {
					{
						Pt V = vertical(l);
						Pt a = circle.p - l.first;
						if (dot(V, a) * dot(v, v) > 0) continue;
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
					// ボールが直線にぶつかったとき跳ね返る処理
					v = reflection(v, l);
					break;
				}
			}
		} else if (id == Field::Board::CHANGE_DIRECTION) {
			if (contains(board.position, circle.p) == GEOMETRY_IN) {
				// 矢印の向きに応じて速度を変化させる
				v += 0.1*Pt(dx[board.dir], dy[board.dir]);
			}
		}
	}
	// 任意のシミュレーションで行う処理:摩擦を受けて速度を微減させる
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