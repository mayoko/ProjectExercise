#pragma once
#include "geometry.h"
#include <vector>

/* カメラから受け取った情報でフィールドを構成するクラス */

class Field
{
public:
	// 各ARマーカーの役割を規定するクラス
	struct Board {
		enum {
			OBSTACLE = 0,
			CHANGE_DIRECTION = 1,
		};
		// 何のボードか(いまのところ障害物と方向変換のみ)
		// 上で定義したenumで管理する
		int id;
		// 向き
		int dir;
		// 各頂点の位置
		Poly position;
	};
	std::vector<Board> boards;
	// カメラから受け取った情報でFieldを構成する
	void receiveData();
	Field(void);
};