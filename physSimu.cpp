#include "physSimu.h"
#include "field.h"
#include "geometry.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};

// 減衰項
const Real damp = 5;
// 矢印パネルでの加速項
const Real cdAccel = 5;
// フィールドの幅
const Real width = 782;
// フィールドの高さ
const Real height = 530;

//ボールの半径
const Real radius = 10;
//ボールの初期速さ
const Real ballStartV = 50;

physSimu::physSimu() {
	ballIsMoving = false;
}

void physSimu::wallDetect() {
	if (real(circle.p) + circle.r + 0.01 < width) {
		Line l;
		l.first = Pt(width, 0); l.second = Pt(width, height);
		Pt V = vertical(l);
		Pt a = circle.p - l.first;
		if (dot(V, a) * dot(V, v) < 0) {
			v = reflection(v, l);
			return;
		}
	}
	if (real(circle.p) + circle.r - 0.01 > 0) {
		Line l;
		l.first = Pt(0, 0); l.second = Pt(0, height);
		Pt V = vertical(l);
		Pt a = circle.p - l.first;
		if (dot(V, a) * dot(V, v) < 0) {
			v = reflection(v, l);
			return;
		}
	}
	if (imag(circle.p) + circle.r + 0.01 < height) {
		Line l;
		l.first = Pt(0, height); l.second = Pt(width, height);
		Pt V = vertical(l);
		Pt a = circle.p - l.first;
		if (dot(V, a) * dot(V, v) < 0) {
			v = reflection(v, l);
			return;
		}
	}
	if (imag(circle.p) + circle.r - 0.01 > 0) {
		Line l;
		l.first = Pt(0, 0); l.second = Pt(width, 0);
		Pt V = vertical(l);
		Pt a = circle.p - l.first;
		if (dot(V, a) * dot(V, v) < 0) {
			v = reflection(v, l);
			return;
		}
	}
}

void physSimu::simulate(const Field& field, Real t) {
	// 以前規定されていた速度を元にオブジェクトを動かす
	Real dt = t-this->t;
	circle.p += Pt(real(v) * dt, imag(v)*dt);
	this->t = t;
	// 壁にぶつかるなら速度を反転させる
	wallDetect();
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
					// ボールが直線にぶつかったとき跳ね返る処理
					v = reflection(v, l);
					break;
				}
			}
		} else if (id == Field::Board::CHANGE_DIRECTION) {
			if (contains(board.position, circle.p) == GEOMETRY_IN) {
				// 矢印の向きに応じて速度を変化させる
				v += cdAccel*dt*Pt(dx[board.dir], dy[board.dir]);
			}
		}
	}
	// 任意のシミュレーションで行う処理:摩擦を受けて速度を微減させる
	Real length = abs(v);
	if (eq(length, 0)) return;
	Real minus = min(damp, length);
	v *= (length-minus) / length;
}

void physSimu::shootBall(const Field& field){
	// firldにおかれている各オブジェクトのなかでスタート用パネルを探し、処理する
	int n = field.boards.size();
	for (Field::Board board : field.boards) {
		int id = board.id;
		if (id == Field::Board::START) {
			//4頂点の重心が中心座標
			Pt ballStartPos = (board.position[0] +board.position[1] +board.position[2] +board.position[3]) / 4.0;
			Cir ball(ballStartPos,radius);
			circle = ball;
			//マーカの縦方向でのワールド座標系での単位ベクトルに初期速さをかけて初期速度に
			v = (board.position[(4-board.dir)%4] - board.position[(4-board.dir+3)%4]) /  abs(board.position[0] - board.position[2]) * ballStartV;
		}
	}
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