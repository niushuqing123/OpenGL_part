
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include<iostream>
using namespace std;
static int menu_id;

void menu_callback(int value)
{
    switch (value)
    {
    case 1:
        cout << ("Menu item 1 selected\n");
        break;
    case 2:
        printf("Menu item 2 selected\n");
        break;
    case 3:
        printf("Menu item 3 selected\n");
        break;
    default:
        break;
    }
}

void display()
{
    // �����Ļ
    glClear(GL_COLOR_BUFFER_BIT);

    // ����ͼ��

    // ˢ����Ļ
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    // ��ʼ��GLUT��
    glutInit(&argc, argv);

    // ����һ������
    glutCreateWindow("Menu Example");

    // ����һ���µĲ˵�
    menu_id = glutCreateMenu(menu_callback);

    // �ڲ˵��������
    glutAddMenuEntry("Menu item 1", 1);
    glutAddMenuEntry("Menu item 2", 2);
    glutAddMenuEntry("Menu item 3", 3);

    // Ϊ����Ҽ������¼�ע��˵�
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // ע��ص�����
    glutDisplayFunc(display);

    // ������ѭ��
    glutMainLoop();

    return 0;
}