#include "field.h"
#include "geometry.h"


using namespace std;

Field::Field(void)
{
}

void Field::receiveData(int marker_num, ARMarkerInfo* marker_info) {
	//�F�������}�[�J�[��id�ƍ��W
	//TODO �ق��̃t�@�C���Ƃ̐����������킹��
		for(int i=0;i < Board::MARK_NUM;i++ ){
			for(int  j=0; j<marker_num; j++ ){
				if( i == marker_info[j].id ){//�ق�Ƃ�i��==�Ƃ�΂�����?
				
					Board new_board;
					new_board.id = marker_info[j].id;//������ID�͂Ƃ肠�������Ƃ��Ƃ�ID�ɂ��Ă��邪OBSTACLE�Ȃǂ����Ɛݒ肷��K�v����
					for(int vertex_num = 0; vertex_num < 4;vertex_num++){
						new_board.position.emplace_back(marker_info[j].vertex[vertex_num][0],marker_info[j].vertex[vertex_num][1]);
					}
					boards.push_back(new_board);

				}
			}
		}
}