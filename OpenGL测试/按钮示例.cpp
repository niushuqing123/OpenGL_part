
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
    // 清空屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制图形

    // 刷新屏幕
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    // 初始化GLUT库
    glutInit(&argc, argv);

    // 创建一个窗口
    glutCreateWindow("Menu Example");

    // 创建一个新的菜单
    menu_id = glutCreateMenu(menu_callback);

    // 在菜单中添加项
    glutAddMenuEntry("Menu item 1", 1);
    glutAddMenuEntry("Menu item 2", 2);
    glutAddMenuEntry("Menu item 3", 3);

    // 为鼠标右键单击事件注册菜单
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // 注册回调函数
    glutDisplayFunc(display);

    // 进入主循环
    glutMainLoop();

    return 0;
}