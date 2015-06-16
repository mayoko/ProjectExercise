#pragma once
#include "geometry.h"
#include <vector>

/* �J��������󂯎�������Ńt�B�[���h���\������N���X */

class Field
{
public:
	// �eAR�}�[�J�[�̖������K�肷��N���X
	struct Board {
		enum {
			OBSTACLE = 0,
			CHANGE_DIRECTION = 1,
		};
		// ���̃{�[�h��(���܂̂Ƃ����Q���ƕ����ϊ��̂�)
		// ��Œ�`����enum�ŊǗ�����
		int id;
		// ����
		int dir;
		// �e���_�̈ʒu
		Poly position;
	};
	std::vector<Board> boards;
	// �J��������󂯎��������Field���\������
	void receiveData();
	Field(void);
};