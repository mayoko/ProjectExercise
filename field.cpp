#include "field.h"
#include "geometry.h"


using namespace std;

Field::Field(void)
{
}

void Field::receiveData(int marker_num, ARMarkerInfo* marker_info) {
	//認識したマーカーのidと座標
	//TODO ほかのファイルとの整合性をあわせる
		for(int i=0;i < Board::MARK_NUM;i++ ){
			for(int  j=0; j<marker_num; j++ ){
				if( i == marker_info[j].id ){//ほんとにiと==とればいいの?
				
					Board new_board;
					new_board.id = marker_info[j].id;//ここのIDはとりあえずもともとのIDにしているがOBSTACLEなどちゃんと設定する必要あり
					for(int vertex_num = 0; vertex_num < 4;vertex_num++){
						new_board.position.emplace_back(marker_info[j].vertex[vertex_num][0],marker_info[j].vertex[vertex_num][1]);
					}
					boards.push_back(new_board);

				}
			}
		}
}