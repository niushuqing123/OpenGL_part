#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include <opencv2/opencv.hpp>

#include<iostream>

#include <chrono>
#include <thread>

#include "time.hpp"

//include���������ݽṹ
//ͷ�ļ�����֮��ϲ���һ��.hpp�ŵ�ͷ�ļ���
using namespace std;

#define Window_width 1600
#define Window_height 800
//����ϵת��
#define SCREEN_TO_NDC_X(x) ((2.0f * (x) / Window_width) - 1.0f)
#define SCREEN_TO_NDC_Y(y) (1.0f - (2.0f * (y) / Window_height))
#define NDC_TO_SCREEN_X(x) (((x) + 1.0f) / 2.0f * Window_width)
#define NDC_TO_SCREEN_Y(y) (((1.0f - (y)) / 2.0f) * Window_height)


float BUTTON_COLOR[] = { 0.5f, 0.5f, 0.5f };
int BUTTON_pos[][2] = { {50,50},{50,150} ,{50,250},{50,350} ,{50,450} };
int BUTTON_size[] = { 100,50 };

int Event_Message = 0;

void drawRectangle(int x, int y, int width, int height)
{
    glBegin(GL_QUADS);
    glColor3fv(BUTTON_COLOR);
    glVertex2f(SCREEN_TO_NDC_X(x), SCREEN_TO_NDC_Y(y));    // bottom left corner
    glVertex2f(SCREEN_TO_NDC_X(x), SCREEN_TO_NDC_Y(y + height));    // top left corner
    glVertex2f(SCREEN_TO_NDC_X(x + width), SCREEN_TO_NDC_Y(y + height));    // top right corner
    glVertex2f(SCREEN_TO_NDC_X(x + width), SCREEN_TO_NDC_Y(y));    // bottom right corner
    glEnd();
}
void reader_button(int x, int y, int width, int height, const char* str)
{
    drawRectangle(x, y, width, height);
    // add code to render text
    glColor3f(1.0f, 1.0f, 1.0f); // set text color to white
    glRasterPos2f(SCREEN_TO_NDC_X(x + 10), SCREEN_TO_NDC_Y(y + 30)); // set text position
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str); // draw the text
}

void render_Texture(float x, float y, float w, float h, GLuint& texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);//��������
    //glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(SCREEN_TO_NDC_X(x), SCREEN_TO_NDC_Y(y));
    glTexCoord2f(0.0f, 1.0f); glVertex2f(SCREEN_TO_NDC_X(x), SCREEN_TO_NDC_Y(y + h));
    glTexCoord2f(1.0f, 1.0f); glVertex2f(SCREEN_TO_NDC_X(x + w), SCREEN_TO_NDC_Y(y + h));
    glTexCoord2f(1.0f, 0.0f); glVertex2f(SCREEN_TO_NDC_X(x + w), SCREEN_TO_NDC_Y(y));
    glEnd();
    glDisable(GL_TEXTURE_2D);//�ر�����
}


void render(GLuint& texture1, GLuint& texture2)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // button
    reader_button(BUTTON_pos[0][0], BUTTON_pos[0][1], BUTTON_size[0], BUTTON_size[1], "aaa");
    reader_button(BUTTON_pos[1][0], BUTTON_pos[1][1], BUTTON_size[0], BUTTON_size[1], "aaa");
    reader_button(BUTTON_pos[2][0], BUTTON_pos[2][1], BUTTON_size[0], BUTTON_size[1], "aaa");
    reader_button(BUTTON_pos[3][0], BUTTON_pos[3][1], BUTTON_size[0], BUTTON_size[1], "aaa");

    // ��������
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_TEXTURE_2D);


    // ��������
    render_Texture(200, 100, 500, 500, texture1);
    render_Texture(800, 100, 576, 1080, texture2);
    //cout << "texture" << texture1 << texture2 << endl;


    switch (Event_Message)
    {
    case 0:break;
    case 1:drawRectangle(200, 200, 100, 100); break;
    case 2:drawRectangle(300, 300, 100, 100); break;
    case 3:drawRectangle(400, 400, 100, 100); break;
    case 4:drawRectangle(500, 500, 100, 100); break;
    default:break;
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    //����ť�¼�
    //���㰴ť�����������հ�ť��λ��������
    int len = sizeof(BUTTON_pos) / sizeof(BUTTON_pos[0]);
    for (int i = 0; i < len; i++)
    {
        if (xpos >= BUTTON_pos[i][0] && xpos <= BUTTON_pos[i][0] + BUTTON_size[0] && ypos >= BUTTON_pos[i][1] && ypos <= BUTTON_pos[i][1] + BUTTON_size[1])
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            {
                Event_Message = i + 1;
                cout << "Event_Message" << Event_Message << endl;
            }
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glewInit();
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Window_width, Window_height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);//��ֱͬ����,��ֱͬ�����Զ�������ѭ���ٶȣ��������п���
    cout << "glGetString(GL_VERSION):" << glGetString(GL_VERSION) << endl;
    // set mouse button callback
    //���ص�����ֻ����һ����ע�����εĻ������һ�ε�Ϊ׼
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //time�������Ҫ��time�ڵ�ʵ�ּ�
    MYtime time;

    //cv��Ҳ����Զ�ȡ����ͷ
    cv::VideoCapture video("resource\\video.mp4");
    cv::VideoCapture video2("resource\\video_time.mp4");
    double fps = video.get(cv::CAP_PROP_FPS);
    double fps2 = video2.get(cv::CAP_PROP_FPS);
    cout << "fps:" << fps << endl;
    cout << "fps2:" << fps2 << endl;


    cv::Mat img1;
    video.read(img1);
    //�ڴ��������img
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1.cols, img1.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img1.data);

    cv::Mat img2;
    video2.read(img2);//!!!��ӣ������������һ����Ƶ��ȡ��ʵ������Ϊ�˸���������ǰ����󶨵�ͼƬ�ĳߴ��Ƕ��١�����޷���ȡ�ߴ���Ϣ�������ڴ洢���ݵĴ�С��Ϊ0*0�����������Ļ���Ҳ�޷�����������
    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img2.cols, img2.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img2.data);



    cout << "img1.cols,img1.rows" << img1.cols << "," << img1.rows << endl;
    cout << "img1.cols,img1.rows" << img2.cols << "," << img2.rows << endl;

    cv::Mat img3;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // ����֡��
        static double prevTime = 0.0;
        static int frameCount = 0;
        double currentTime = glfwGetTime();
        frameCount++;
        if (currentTime - prevTime >= 1.0) {
            double fps = frameCount / (currentTime - prevTime);
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            prevTime = currentTime;
        }


        video.read(img1);//read��������һ�������Ĳ��������б�Ҫ����߳�
        // �ڴ����°�����1
        //cv::resize(img1, img1, cv::Size(512, 512)); // ��ͼ�������512x512

        cout << "img1.cols,img1.rows" << img1.cols << "," << img1.rows << endl;

        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img1.cols, img1.rows, GL_BGR, GL_UNSIGNED_BYTE, img1.data);


        if (1)
        {
            video2.read(img2);

            cout << "img1.cols,img1.rows" << img2.cols << "," << img2.rows << endl;

            //cv::resize(img2, img2, cv::Size(512, 512)); // ��ͼ�������512x512


            // �ڴ����°�����2
            glBindTexture(GL_TEXTURE_2D, texture2);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img2.cols, img2.rows, GL_BGR, GL_UNSIGNED_BYTE, img2.data);

        }

        //���е���Ⱦ����
        render(texture1, texture2);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();



    }
    glutMainLoopEvent();
    glfwTerminate();

    return 0;
}
