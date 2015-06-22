/**************************************--�d�l--***************************************
������
�}�[�J�uHiro�v�uSample1�v�uKanji�v��F�����ĕʂ̃I�u�W�F�N�g��\��������v���O����
�}�[�J�Ԃ̋����E�p�x�̍����R���\�[���Ō��邱�Ƃ��ł���B
*************************************************************************************/

#pragma warning(disable:4819)

#include "field.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <map>

#define _USE_MATH_DEFINES	// math.h��M_PI���g������
#include <math.h>			// �p�x�v�Z�p

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <AR/ar.h>
#include <AR/param.h>
#include <AR/video.h>
#include <AR/gsub.h>

// �O���[�o���ϐ�
/* �J�����\�� */
char *vconf_name  = "Data/WDM_camera_flipV.xml";	// �r�f�I�f�o�C�X�̐ݒ�t�@�C��
int  xsize;											// �E�B���h�E�T�C�Y
int  ysize;											// �E�B���h�E�T�C�Y
int  thresh = 100;									// 2�l����臒l
int  count = 0;										// �����t���[����

// �t�B�[���h
Field gfield;

/* �J�����p�����[�^ */
char *cparam_name = "Data/calib_params_0610.dat";			// �J�����p�����[�^�t�@�C��
ARParam cparam;										// �J�����p�����[�^

/* �p�^�[���t�@�C�� */
/*! �p�^�[���t�@�C����Field�N���X�ɒ�`����Ă��鏇�Ԃɒ�`���邱�� !*/
#define MARK_NUM		3						// �g�p����}�[�J�[�̌�
//-----
#define MARK1_MARK_ID	1						// �}�[�J�[ID
#define MARK1_PATT_NAME	"Data\\multi\\patt.a"		// �p�^�[���t�@�C����
#define MARK1_SIZE		80.0					// �p�^�[���̕��i80mm�j
//-----
#define MARK2_MARK_ID	2						// �}�[�J�[ID
#define MARK2_PATT_NAME	"Data\\multi\\patt.b"	// �p�^�[���t�@�C����
#define MARK2_SIZE		80.0					// �p�^�[���̕��i80mm�j
//-----
#define MARK3_MARK_ID	3						// �}�[�J�[ID
#define MARK3_PATT_NAME	"Data\\multi\\patt.c"		// �p�^�[���t�@�C����
#define MARK3_SIZE		80.0					// �p�^�[���̕��i80mm�j
//-----
typedef struct {
	char   *patt_name;			// �p�^�[���t�@�C��
	int    patt_id;				// �p�^�[����ID
	int    mark_id;				// �}�[�J�[ID
	int    visible;				// ���o�t���O
	double patt_width;			// �p�^�[���̃T�C�Y�i�P�ʁF�����j
	double patt_center[2];		// �p�^�[���̒��S���W
	double patt_trans[3][4];	// ���W�ϊ��s��
} MARK_T;
//-----
MARK_T   marker[MARK_NUM] = {
	{MARK1_PATT_NAME, -1, MARK1_MARK_ID, 0, MARK1_SIZE, {0.0, 0.0}},
	{MARK2_PATT_NAME, -1, MARK2_MARK_ID, 0, MARK2_SIZE, {0.0, 0.0}},
	{MARK3_PATT_NAME, -1, MARK3_MARK_ID, 0, MARK3_SIZE, {0.0, 0.0}}
};


// �v���g�^�C�v�錾
void Init(void);
void MainLoop(void);
void SetupLighting1(void);
void SetupLighting2(void);
void SetupMaterial1(void);
void SetupMaterial2(void);
void KeyEvent( unsigned char key, int x, int y );
void MouseEvent( int button, int state, int x, int y );
void Cleanup(void);
void DrawObject( int mark_id, double patt_trans[3][4] );


//=======================================================
// main�֐�
//=======================================================
int main( int argc, char **argv )
{

	// GLUT�̏�����
	glutInit( &argc, argv );

	// AR�A�v���P�[�V�����̏�����
	Init();

	// �r�f�I�L���v�`���̊J�n
	arVideoCapStart();

	// ���C�����[�v�̊J�n
	argMainLoop( MouseEvent, KeyEvent, MainLoop );

	return 0;
}


//=======================================================
// �������֐�
//=======================================================
void Init(void)
{
	ARParam wparam;		// �J�����p�����[�^

	// �r�f�I�f�o�C�X�̐ݒ�
	if( arVideoOpen( vconf_name ) < 0 ){
		printf("�r�f�I�f�o�C�X�̃G���[\n");
		while(1);	
		exit(0);
	}

	// �E�B���h�E�T�C�Y�̎擾
	if( arVideoInqSize( &xsize, &ysize ) < 0 ) exit(0);
	printf("Image size (x,y) = (%d,$d)\n", xsize, ysize);

	// �J�����p�����[�^�̐ݒ�
	if( arParamLoad( cparam_name, 1, &wparam ) < 0 ){
		printf("�J�����p�����[�^�̓ǂݍ��݂Ɏ��s���܂���\n");
		while(1);
		exit(0);
	}

	// �J�����p�����[�^�̃T�C�Y����
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
	// �J�����p�����[�^�̏�����
	arInitCparam( &cparam );
	printf("*** Camera Parameter ***\n");
	arParamDisp( &cparam );

	// �p�^�[���t�@�C���̃��[�h
	for( int i=0; i<Field::Board::EFFECT_NUM; i++ ){
		if( (marker[i].patt_id = arLoadPatt(marker[i].patt_name)) < 0){
			gfield.trans.insert(std::pair<int, int>(marker[i].patt_id, i));
			printf("�p�^�[���t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n");
			printf("%s\n", marker[i].patt_name);
			while(1);
			exit(0);
		}
	}

	// gsub���C�u�����̏�����
	argInit( &cparam, 1.0, 0, 0, 0, 0 );

	// �E�B���h�E�^�C�g���̐ݒ�
	glutSetWindowTitle("ARTK_basic");
}


//=======================================================
// ���C�����[�v�֐�
//=======================================================
void MainLoop(void)
{
	ARUint8          *image;			// �J�����L���v�`���摜
	ARMarkerInfo     *marker_info;		// �}�[�J���
	int              marker_num;		// ���o���ꂽ�}�[�J�̐�
	int              i, j, k;

	// �t�B�[���h�̏�����
	gfield.clear();
	// �J�����摜�̎擾
	if( (image = (ARUint8 *)arVideoGetImage()) == NULL ){
		arUtilSleep( 2 );
		return;
	}
	if( count == 0 ) arUtilTimerReset();
	count++;

	// �J�����摜�̕`��
	/*argDrawMode2D();
	argDispImage( image, 0, 0 );
	*/
	// �}�[�J�̌��o�ƔF��
	if( arDetectMarker( image, thresh, &marker_info, &marker_num ) < 0 ){
		Cleanup();
		exit(0);
	}
	gfield.receiveData(marker_num, marker_info);

	// ���̉摜�̃L���v�`���w��
	arVideoCapNext();

	//// 3D�I�u�W�F�N�g��`�悷�邽�߂̏���
	//argDrawMode3D();
	//argDraw3dCamera( 0, 0 );
	//glClearDepth(1.0);					// �f�v�X�o�b�t�@�̏����l
	//glClear( GL_DEPTH_BUFFER_BIT );		// �f�v�X�o�b�t�@�̏�����

	// �}�[�J�̈�v�x�̔�r
	for( i=0; i<marker_num; i++ ){
		k = -1;
		for( j=0; j<marker_num; j++ ){
			if( marker[i].patt_id == marker_info[j].id ){
				if( k == -1 ) k = j;
				else if( marker_info[k].cf < marker_info[j].cf ) k = j;
			}
		}

		// �}�[�J�[��������Ȃ������Ƃ�
		if( k == -1 ){
			marker[i].visible = 0;
			continue;
		}

		// ���W�ϊ��s����擾
		if( marker[i].visible == 0 ) {
			// 1�t���[�����g���ă}�[�J�̈ʒu�E�p���i���W�ϊ��s��j�̌v�Z
			arGetTransMat( &marker_info[k], marker[i].patt_center, marker[i].patt_width, marker[i].patt_trans );
		} else {
			// �O�̃t���[�����g���ă}�[�J�̈ʒu�E�p���i���W�ϊ��s��j�̌v�Z
			arGetTransMatCont( &marker_info[k], marker[i].patt_trans, marker[i].patt_center, marker[i].patt_width, marker[i].patt_trans );
		}
			marker[i].visible = 1;

			// 3D�I�u�W�F�N�g�̕`��
			//DrawObject( marker[i].mark_id, marker[i].patt_trans );
		}

		// �o�b�t�@�̓��e����ʂɕ\��
		argSwapBuffers();
}


//=======================================================
// 3D�I�u�W�F�N�g�̕`����s���֐�
//=======================================================
//void DrawObject( int mark_id, double patt_trans[3][4] )
//{
//	double gl_para[16];	// ARToolKit->OpenGL�ϊ��s��
//
//	// �A�ʏ���
//	glEnable( GL_DEPTH_TEST );			// �A�ʏ����E�L��
//	glDepthFunc( GL_LEQUAL );			// �f�v�X�e�X�g
//
//	// �ϊ��s��̓K�p
//	argConvGlpara( patt_trans, gl_para );	// ARToolKit����OpenGL�̍s��ɕϊ�
//	glMatrixMode( GL_MODELVIEW );			// �s��ϊ����[�h�E���f���r���[
//	glLoadMatrixd( gl_para );				// �ǂݍ��ލs����w��
//
//	switch( mark_id ){
//		case MARK1_MARK_ID:
//			// ���C�e�B���O
//			SetupLighting1();			// ���C�g�̒�`
//			glEnable( GL_LIGHTING );	// ���C�e�B���O�E�L��
//			glEnable( GL_LIGHT0 );		// ���C�g0�E�I��
//			// �I�u�W�F�N�g�̍ގ�
//			SetupMaterial1();
//
//			// 3D�I�u�W�F�N�g�̕`��
//			glTranslatef( 0.0, 0.0, 25.0 );	// �}�[�J�̏�ɍڂ��邽�߂�Z�����i�}�[�J����j��25.0[mm]�ړ�
//			glutSolidCube( 150.0 );			// �\���b�h�L���[�u��`��i1�ӂ̃T�C�Y50[mm]�j
//			break;
//
//		case MARK2_MARK_ID:
//			// ���C�e�B���O
//			SetupLighting2();			// ���C�g�̒�`
//			glEnable( GL_LIGHTING );	// ���C�e�B���O�E�L��
//			glEnable( GL_LIGHT0 );		// ���C�g0�E�I��
//			// �I�u�W�F�N�g�̍ގ�
//			SetupMaterial2();
//
//			// 3D�I�u�W�F�N�g�̕`��
//			glTranslatef( 0.0, 0.0, 25.0 );		// �}�[�J�̏�ɍڂ��邽�߂�Z�����i�}�[�J����j��25.0[mm]�ړ�
//			glutSolidSphere( 50.0, 10, 10 );	// �\���b�h�X�t�B�A��`��i1�ӂ̃T�C�Y50[mm]�j
//			break;
//
//		case MARK3_MARK_ID:
//			// ���C�e�B���O
//			SetupLighting1();			// ���C�g�̒�`
//			glEnable( GL_LIGHTING );	// ���C�e�B���O�E�L��
//			glEnable( GL_LIGHT0 );		// ���C�g0�E�I��
//			// �I�u�W�F�N�g�̍ގ�
//			SetupMaterial2();
//
//			// 3D�I�u�W�F�N�g�̕`��
//			glTranslatef( 0.0, 0.0, 25.0 );	// �}�[�J�̏�ɍڂ��邽�߂�Z�����i�}�[�J����j��25.0[mm]�ړ�
//			glRotated( 90, 1.0, 0.0, 0.0);	// �e�B�[�|�b�g���}�[�J��ɍڂ��邽�߂�90����]
//			glutSolidTeapot( 50.0 );		// �\���b�h�e�B�[�|�b�g��`��i�T�C�Y50[mm]�j
//			break;
//	}
//
//
//	// �I������
//	glDisable( GL_LIGHTING );		// ���C�e�B���O�E����
//	glDisable( GL_DEPTH_TEST );		// �f�v�X�e�X�g�E����
//}

//
////=======================================================
//// ���C�e�B���O
////=======================================================
//void SetupLighting1(void)
//{
//	// ���C�g�̒�`
//	GLfloat lt0_position[] = {100.0, -200.0, 200.0, 0.0};	// ���C�g0�̈ʒu
//	GLfloat lt0_ambient[]  = {0.1, 0.1, 0.1, 1.0};			// �@�@�@�@ ����
//	GLfloat lt0_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			// �@�@�@�@ �g�U��
//
//	// ���C�g�̐ݒ�
//	glLightfv( GL_LIGHT0, GL_POSITION, lt0_position );
//	glLightfv( GL_LIGHT0, GL_AMBIENT, lt0_ambient );
//	glLightfv( GL_LIGHT0, GL_DIFFUSE, lt0_diffuse );
//}
//
//void SetupLighting2(void)
//{
//	// ���C�g�̒�`
//	GLfloat lt0_position[] = {100.0, 200.0, 200.0, 0.0};	// ���C�g0�̈ʒu
//	GLfloat lt0_ambient[]  = {0.2, 0.2, 0.2, 1.0};			// �@�@�@�@ ����
//	GLfloat lt0_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			// �@�@�@�@ �g�U��
//
//	// ���C�g�̐ݒ�
//	glLightfv( GL_LIGHT0, GL_POSITION, lt0_position );
//	glLightfv( GL_LIGHT0, GL_AMBIENT, lt0_ambient );
//	glLightfv( GL_LIGHT0, GL_DIFFUSE, lt0_diffuse );
//}

//
////=======================================================
//// �}�e���A���̐ݒ�
////=======================================================
//void SetupMaterial1(void)
//{
//	// �I�u�W�F�N�g�̍ގ�
//	GLfloat mat_ambient[] = {0.0, 1.0, 1.0, 1.0};	// �ގ��̊���
//	GLfloat mat_specular[] = {0.0, 0.0, 1.0, 1.0};	// ���ʌ�
//	GLfloat mat_shininess[] = {50.0};				// ���ʌW��
//
//	// �}�e���A���̐ݒ�
//	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
//}
//
//void SetupMaterial2(void)
//{
//	// �I�u�W�F�N�g�̍ގ�
//	GLfloat mat_ambient[] = {0.0, 0.0, 1.0, 1.0};	// �ގ��̊���
//	GLfloat mat_specular[] = {0.0, 0.0, 1.0, 1.0};	// ���ʌ�
//	GLfloat mat_shininess[] = {50.0};				// ���ʌW��
//
//	// �}�e���A���̐ݒ�
//	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
//}
//

//=======================================================
// �L�[�{�[�h���͏����֐�
//=======================================================
void KeyEvent( unsigned char key, int x, int y )
{
	// ESC�L�[����͂�����A�v���P�[�V�����I��
	if (key == 0x1b ){
		printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
		Cleanup();
		exit(0);
	}
}


//=======================================================
// �}�E�X���͏����֐�
//=======================================================
void MouseEvent( int button, int state, int x, int y )
{
	// ���͏�Ԃ�\��
	printf("�{�^���F%d ��ԁF%d ���W�F(x,y)=(%d,%d) \n", button, state, x, y );
}


//=======================================================
// �I�������֐�
//=======================================================
void Cleanup(void)
{
	arVideoCapStop();	// �r�f�I�L���v�`���̒�~
	arVideoClose();		// �r�f�I�f�o�C�X�̏I��
	argCleanup();		// ARToolKit�̏I������
}