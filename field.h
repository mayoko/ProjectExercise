#pragma once
#include "geometry.h"
#include <vector>
#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>			// arParamDisp()
#include <AR/ar.h>
#include <AR/gsub_lite.h>



/* �J��������󂯎�������Ńt�B�[���h���\������N���X */

class Field
{
public:
	// �eAR�}�[�J�[�̖������K�肷��N���X
	struct Board {
		enum {
			OBSTACLE = 0,
			CHANGE_DIRECTION = 1,
			EFFECT_NUM,
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
	void receiveData(int marker_num, ARMarkerInfo* marker_info);
	Field(void);
};