#include "field.h"
#include "geometry.h"
#include "Mat.h"

#include <map>
#include <utility>
#include <fstream>
#include <iostream>

using namespace std;

Field::Field(void)
{
	H_cw.resize(3, 3);
	ifstream ifs("Data/H_wc.txt");
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) ifs >> H_cw.mat[i][j];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << H_cw.mat[i][j] << " ";
		}
		cout << endl;
	}
}

void Field::receiveData(int marker_num, ARMarkerInfo* marker_info) {
	//�F�������}�[�J�[��id�ƍ��W
	//TODO �ق��̃t�@�C���Ƃ̐����������킹��
	for(int  j=0; j<marker_num; j++ ){
		if (marker_info[j].cf < 0.5) continue;
		Board new_board;
		new_board.id = trans[marker_info[j].id];//������ID�͂Ƃ肠�������Ƃ��Ƃ�ID�ɂ��Ă��邪OBSTACLE�Ȃǂ����Ɛݒ肷��K�v����
		for(int vertex_num = 0; vertex_num < 4;vertex_num++){
			Mat vec(3, 1);
			for (int i = 0; i < 2; i++) vec.mat[i][0] = marker_info[j].vertex[vertex_num][i];
			vec.mat[2][0] = 1;
			vec = mul(H_cw, vec);
			new_board.position.emplace_back(vec.mat[0][0],vec.mat[1][0]);
		}
		boards.push_back(new_board);
		cout << new_board.id << " " << real(new_board.position[0]) << " " << imag(new_board.position[0]) << endl;
	}
}

void Field::clear() {
	boards.clear();
}