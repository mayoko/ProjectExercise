#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#include <GL/freeglut/freeglut.h>
#include <stdio.h>
#include <gl/glpng.h>

#define WIDTH 320
#define HEIGHT 240

pngInfo info;
GLuint texture;

void display(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);

 glEnable(GL_TEXTURE_2D);//�e�N�X�`���L��
    glEnable(GL_ALPHA_TEST);//�A���t�@�e�X�g�J�n
    glBegin(GL_POLYGON);
 glTexCoord2f(0.0f, 1.0f); glVertex2d(10 , 230);//����
 glTexCoord2f(0.0f, 0.0f); glVertex2d(10 ,  10);//����
 glTexCoord2f(1.0f, 0.0f); glVertex2d( 310 ,  10);//�E��
 glTexCoord2f(1.0f, 1.0f); glVertex2d( 310 , 230);//�E��
    glEnd();
    glDisable(GL_ALPHA_TEST);//�A���t�@�e�X�g�I��
    glDisable(GL_TEXTURE_2D);//�e�N�X�`������

 glutSwapBuffers();
}
void idle(void)
{
 glutPostRedisplay();
}
void Init(){
 glClearColor(0.0, 0.0, 0.0, 1.0);
 glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
 texture = pngBind ("sample.png", PNG_NOMIPMAP, PNG_ALPHA, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
}
int main(int argc, char *argv[])
{
 glutInitWindowPosition(100, 100);
 glutInitWindowSize(WIDTH, HEIGHT);
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
 glutCreateWindow("�摜��ǂݍ���ŕ\��");
 glutDisplayFunc(display);
 glutIdleFunc(idle);
 Init();
 glutMainLoop();
 return 0;
}