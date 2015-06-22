#pragma once
#include "geometry.h"
#include "field.h"

// およそ530*782らしいです
class physSimu
{
public:
	Cir circle; // 座標および半径
	Pt v; // 速度
	Real t; // 時間
	const Real damp; // 毎周期落ちる速度
	const Real cdAccel; // CHANGE_DIRECTIONで加速するやつ
	const Real width; // 幅
	const Real height; // 高さ
	// 物理シミュレーション関係なく位置,速度を変更する
	void changeState(Real x, Real y, Real vx, Real vy);
	// Fieldおよび時間を読み込んだとき,ボールの運動を規定する
	void simulate(const Field& field, Real t);
	// ボールの運動を記述する(デバッグ用)
	void print() const ;
};