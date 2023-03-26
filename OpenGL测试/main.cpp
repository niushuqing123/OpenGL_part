#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include <opencv2/opencv.hpp>

#include<iostream>

#include <chrono>
#include <thread>

#include "time.hpp"
#include "oldbuffer.hpp"
#include "buffer单读单写实装测试.hpp"


//include缓冲区数据结构
//头文件多了之后合并成一个.hpp放到头文件里
using namespace std;

#define Window_width 1600
#define Window_height 800
//坐标系转换
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
    //读取的时候绑定纹理，渲染的时候也要绑定纹理，因为读取只知道读取，渲染纹理只知道渲染纹理，只有绑定了，接下来才对特定的纹理进行操作
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);//开启纹理
    //glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(SCREEN_TO_NDC_X(x), SCREEN_TO_NDC_Y(y));
    glTexCoord2f(0.0f, 1.0f); glVertex2f(SCREEN_TO_NDC_X(x), SCREEN_TO_NDC_Y(y + h));
    glTexCoord2f(1.0f, 1.0f); glVertex2f(SCREEN_TO_NDC_X(x + w), SCREEN_TO_NDC_Y(y + h));
    glTexCoord2f(1.0f, 0.0f); glVertex2f(SCREEN_TO_NDC_X(x + w), SCREEN_TO_NDC_Y(y));
    glEnd();
    glDisable(GL_TEXTURE_2D);//关闭纹理
}


void render(GLuint& texture1, GLuint& texture2)
{
    //对于glBegin和glend的用法讨论，为什么不用新的顶点缓冲模型，手动渲染管线和固定渲染管线、渲染批次
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    // button
    reader_button(BUTTON_pos[0][0], BUTTON_pos[0][1], BUTTON_size[0], BUTTON_size[1], "aaa");
    reader_button(BUTTON_pos[1][0], BUTTON_pos[1][1], BUTTON_size[0], BUTTON_size[1], "aaa");
    reader_button(BUTTON_pos[2][0], BUTTON_pos[2][1], BUTTON_size[0], BUTTON_size[1], "aaa");
    reader_button(BUTTON_pos[3][0], BUTTON_pos[3][1], BUTTON_size[0], BUTTON_size[1], "aaa");

    // 开启纹理
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_TEXTURE_2D);


    // 绘制纹理
    render_Texture(200, 100, 500, 500, texture1);
    render_Texture(800, 100, 500, 500, texture2);
    //cout << "texture" << texture1 << texture2 << endl;



    //可变区域,按消息类型显示,r1/r2/r3
    //不一定要按Event_Message显示，可能需要先处理功能和数据
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

    //处理按钮事件
    //计算按钮的总数，按照按钮的位置逐个检测
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

//首先实现用一个线程控制视频输出帧率
std::atomic<bool> run_videothread(false);//写入时=已被重载，但读取时不确定，所以用load

std::condition_variable condition_variable_videothread; // 条件变量
std::mutex mtx_videothread;

bool ready = false; // 标记变量


void videothread(NoWaitBuffer<4>& buffer, videothread_img_size& videothread_img_size)
{
    MYtime time;
    //cv
    cv::VideoCapture video("resource\\video_time.mp4");
    double fps = video.get(cv::CAP_PROP_FPS);
    cout << "videothread fps:" << fps << endl;
    const double target_fps = fps;
    double  target_frame_time = 1000000000 / target_fps;//单位是纳秒
    cout << "videothread target_frame_time" << target_frame_time / 1000000 << endl;

    cv::Mat img;
    video.read(img);
    videothread_img_size = { img.cols, img.rows };
    cout << "----------" << videothread_img_size.img_cows << "--" << videothread_img_size.img_rows << endl;

    //在锁之前的内容为初始化操作
    {
        //用锁来阻塞
        //std::unique_lock<std::mutex> lock(mtx_videothread); // 锁定互斥量
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
        //将图片写入缓冲区
        //...
        //std::cin.get();
        {
            buffer.update(img);
        }

        double using_time = time.locality_End();//locality_End的结果单位是秒

        if (using_time * 1000000000 < target_frame_time) {
            double sleep_time_n = target_frame_time - (using_time * 1000000000);
            //cout << "using_time(ms)" << using_time/1000000 << endl;
            //cout << "sleep_time(ms)" << sleep_time_n / 1000000 << endl;
            time.locality_Start();
            std::this_thread::sleep_for(std::chrono::nanoseconds((int)sleep_time_n));
            sleep_t_c = time.locality_End();//locality_End的结果单位是秒
            //经计时发现，实际睡眠时间好像都比需要睡眠时间长一点，这样的话视频会播放的慢一点

        }

    }

    return;
}

std::atomic<bool> run_procthread(false);
std::mutex mtx_procthread;

void proc(NoWaitBuffer<4>& buffer1, NoWaitBuffer<3>& buffer)
{

    cv::Mat img;
    //在锁之前的内容为初始化操作
    {
        //用锁来阻塞
        std::unique_lock<std::mutex> lock(mtx_procthread); // 锁定互斥量
        //std::unique_lock<std::mutex> lock(mtx_videothread); 
        //这里就算用同一个互斥量也能执行，是因为wait的特性，wait在刚执行时就会检查条件是否成立，如果成立就不会阻塞，所以就算被同一个锁耽误再久，因为ready早在主线程中变为真，所以当进行到wait时也直接顺利通过了
        //但是为了编程规范性，还是要选择用两个不同的互斥量了表示，并不会加大多少开销
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
    glfwSwapInterval(0);//垂直同步关,垂直同步会自动限制主循环速度，无需自行控制
    cout << "glGetString(GL_VERSION):" << glGetString(GL_VERSION) << endl;
    // set mouse button callback
    //鼠标回调函数只能有一个，注册两次的话以最后一次的为准
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    //cv，也许可以读取摄像头
    cv::VideoCapture video("resource\\video.mp4");
    double fps = video.get(cv::CAP_PROP_FPS);
    cout << "fps:" << fps << endl;


    cv::Mat img1;
    video.read(img1);
    //在此纹理绑定了img
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1.cols, img1.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img1.data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 576, 1280, 0, GL_BGR, GL_UNSIGNED_BYTE, img1.data);

    //启动视频控制线程，并且做准备工作
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
    //这里必须要绑定图片，目的是为了按照尺寸给纹理初始化
    cout << "----------" << videothread_img_size.img_cows << "--" << videothread_img_size.img_rows << endl;

    //①，已经提前了，应该提前，否则可能会导致视频线程有概率直接跳出循环
    run_videothread = true;//这句提前一点，应该也可以①
    run_procthread = true;
    {
        //std::unique_lock<std::mutex> lock_video(mtx_videothread);
        //std::unique_lock<std::mutex> lock_proc(mtx_procthread);
        ready = true;
    }// 作用域结束，read_lock 的析构函数会自动释放锁
    condition_variable_videothread.notify_all();




    cv::Mat img2;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // 计算帧率
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
        video.read(img1);//read在这里是一个大开销的操作，很有必要拆分线程
        // 在此重新绑定纹理1
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img1.cols, img1.rows, GL_BGR, GL_UNSIGNED_BYTE, img1.data);
        */

        //读取缓冲区
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




        //所有的渲染任务
        render(texture1, texture2);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);//撤掉这句能让fps达到几万
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
