#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include <opencv2/opencv.hpp>

#include<iostream>

#include <chrono>
#include <thread>

#include "time.hpp"
#include "oldbuffer.hpp"
#include "buffer������дʵװ����.hpp"


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
    //��ȡ��ʱ���������Ⱦ��ʱ��ҲҪ��������Ϊ��ȡֻ֪����ȡ����Ⱦ����ֻ֪����Ⱦ����ֻ�а��ˣ��������Ŷ��ض���������в���
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
    //����glBegin��glend���÷����ۣ�Ϊʲô�����µĶ��㻺��ģ�ͣ��ֶ���Ⱦ���ߺ͹̶���Ⱦ���ߡ���Ⱦ����
    /* Render here */
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
    render_Texture(800, 100, 500, 500, texture2);
    //cout << "texture" << texture1 << texture2 << endl;



    //�ɱ�����,����Ϣ������ʾ,r1/r2/r3
    //��һ��Ҫ��Event_Message��ʾ��������Ҫ�ȴ����ܺ�����
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



struct videothread_img_size
{
    int img_cows;
    int img_rows;

};

//����ʵ����һ���߳̿�����Ƶ���֡��
std::atomic<bool> run_videothread(false);//д��ʱ=�ѱ����أ�����ȡʱ��ȷ����������load

std::condition_variable condition_variable_videothread; // ��������
std::mutex mtx_videothread;

bool ready = false; // ��Ǳ���


void videothread(NoWaitBuffer<4>& buffer, videothread_img_size& videothread_img_size)
{
    MYtime time;
    //cv
    cv::VideoCapture video("resource\\video_time.mp4");
    double fps = video.get(cv::CAP_PROP_FPS);
    cout << "videothread fps:" << fps << endl;
    const double target_fps = fps;
    double  target_frame_time = 1000000000 / target_fps;//��λ������
    cout << "videothread target_frame_time" << target_frame_time / 1000000 << endl;

    cv::Mat img;
    video.read(img);
    videothread_img_size = { img.cols, img.rows };
    cout << "----------" << videothread_img_size.img_cows << "--" << videothread_img_size.img_rows << endl;

    //����֮ǰ������Ϊ��ʼ������
    {
        //����������
        //std::unique_lock<std::mutex> lock(mtx_videothread); // ����������
        std::unique_lock<std::mutex> lock(mtx_videothread);
        Sleep(100);

        condition_variable_videothread.wait(lock, [] { return ready; });
    }

    double sleep_t_c = 0;

    cv::Mat kkk;

    if (run_videothread.load())cout << "videothread is ready" << endl;
    while (run_videothread.load())
    {
        time.locality_Start();
        //cout << "sleep_t_c(ms)" << sleep_t_c * 1000 << endl;
        for (int i = 0; i < 10; i++)
        {
            kkk = img.clone();
        }

        //cout << "frame++++++++++++++++++++++" << endl;
        //Sleep(500);
        video.read(img);
        //��ͼƬд�뻺����
        //...
        //std::cin.get();
        {
            buffer.update(img);
        }

        double using_time = time.locality_End();//locality_End�Ľ����λ����

        if (using_time * 1000000000 < target_frame_time) {
            double sleep_time_n = target_frame_time - (using_time * 1000000000);
            //cout << "using_time(ms)" << using_time/1000000 << endl;
            //cout << "sleep_time(ms)" << sleep_time_n / 1000000 << endl;
            time.locality_Start();
            std::this_thread::sleep_for(std::chrono::nanoseconds((int)sleep_time_n));
            sleep_t_c = time.locality_End();//locality_End�Ľ����λ����
            //����ʱ���֣�ʵ��˯��ʱ����񶼱���Ҫ˯��ʱ�䳤һ�㣬�����Ļ���Ƶ�Ქ�ŵ���һ��

        }

    }

    return;
}

std::atomic<bool> run_procthread(false);
std::mutex mtx_procthread;

void proc(NoWaitBuffer<4>& buffer1, NoWaitBuffer<3>& buffer)
{

    cv::Mat img;
    //����֮ǰ������Ϊ��ʼ������
    {
        //����������
        std::unique_lock<std::mutex> lock(mtx_procthread); // ����������
        //std::unique_lock<std::mutex> lock(mtx_videothread); 
        //���������ͬһ��������Ҳ��ִ�У�����Ϊwait�����ԣ�wait�ڸ�ִ��ʱ�ͻ��������Ƿ��������������Ͳ������������Ծ��㱻ͬһ���������پã���Ϊready�������߳��б�Ϊ�棬���Ե����е�waitʱҲֱ��˳��ͨ����
        //����Ϊ�˱�̹淶�ԣ�����Ҫѡ����������ͬ�Ļ������˱�ʾ��������Ӵ���ٿ���
        Sleep(100);
        condition_variable_videothread.wait(lock, [] { return ready; });
    }
    if (run_procthread.load())cout << "proc is ready" << endl;

    while (run_procthread.load())
    {
        buffer1.ready(img, 2);
        Sleep(1000);
        cout << "run_procthread" << endl;
        cout << "----------" << img.rows << "--" << img.cols << endl;

        buffer.update(img);
    }
    return;
}




int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glewInit();
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())return -1;

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


    //cv��Ҳ����Զ�ȡ����ͷ
    cv::VideoCapture video("resource\\video.mp4");
    double fps = video.get(cv::CAP_PROP_FPS);
    cout << "fps:" << fps << endl;


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
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1.cols, img1.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img1.data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 576, 1280, 0, GL_BGR, GL_UNSIGNED_BYTE, img1.data);

    //������Ƶ�����̣߳�������׼������
    videothread_img_size videothread_img_size = { 0,0 };
    //Buffer buff;
    NoWaitBuffer<4> NWbuffer(576, 1280, CV_8UC3);
    NoWaitBuffer<3> NWbuffer_for_proc(576, 1280, CV_8UC3);
    thread video_tsak(videothread, ref(NWbuffer), ref(videothread_img_size));
    thread pro_tsak(proc, ref(NWbuffer), ref(NWbuffer_for_proc));
    Sleep(1000);

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, videothread_img_size.img_cows, videothread_img_size.img_rows, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
    //�������Ҫ��ͼƬ��Ŀ����Ϊ�˰��ճߴ�������ʼ��
    cout << "----------" << videothread_img_size.img_cows << "--" << videothread_img_size.img_rows << endl;

    //�٣��Ѿ���ǰ�ˣ�Ӧ����ǰ��������ܻᵼ����Ƶ�߳��и���ֱ������ѭ��
    run_videothread = true;//�����ǰһ�㣬Ӧ��Ҳ���Ԣ�
    run_procthread = true;
    {
        //std::unique_lock<std::mutex> lock_video(mtx_videothread);
        //std::unique_lock<std::mutex> lock_proc(mtx_procthread);
        ready = true;
    }// �����������read_lock �������������Զ��ͷ���
    condition_variable_videothread.notify_all();




    cv::Mat img2;

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
            //std::cout << "buff.testnum: " << buff.test_num << std::endl;
        }

        /*
        video.read(img1);//read��������һ�������Ĳ��������б�Ҫ����߳�
        // �ڴ����°�����1
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img1.cols, img1.rows, GL_BGR, GL_UNSIGNED_BYTE, img1.data);
        */

        //��ȡ������
        if (NWbuffer.ready(img1, 1))
        {
            //cout << "Buffer_ready" << endl;
            //buff.consume(img2);
            //cout << "img3.cols" << img3.cols << endl;
            //cout << "img3.total" << img3.total() << endl;

            glBindTexture(GL_TEXTURE_2D, texture1);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img1.cols, img1.rows, GL_BGR, GL_UNSIGNED_BYTE, img1.data);

        }

        if (NWbuffer_for_proc.ready(img2, 1))
        {
            //cout << "NWbuffer_for_proc.ready" << endl;

            glBindTexture(GL_TEXTURE_2D, texture2);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img2.cols, img2.rows, GL_BGR, GL_UNSIGNED_BYTE, img2.data);
        }




        //���е���Ⱦ����
        render(texture1, texture2);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);//�����������fps�ﵽ����
        /* Poll for and process events */
        glfwPollEvents();

    }


    glutMainLoopEvent();
    glfwTerminate();

    run_videothread = false;
    run_procthread = false;
    video_tsak.join();
    pro_tsak.join();
    return 0;
}
