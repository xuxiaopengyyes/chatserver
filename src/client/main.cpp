#include "json.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <ctime>
#include <unordered_map>
#include <functional>
using namespace std;
using json = nlohmann::json;

#include <unistd.h>
#include <sys/socket.h>
#include <sys/type.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <atomic>

#include "group.hpp"
#include "user.hpp"
#include "public.hpp"

//记录当前系统登录的用户信息
User g_currentUser;
//记录当前登录好友的列表信息
vector<User> g_currentUserFriendList;
//记录当前登录用户的群组列表信息
vector<Group> g_currentUserGroupList;

//控制台主菜单页面程序
bool isMainMenuRunning = false;

//用于读写线程之间的通信
sem_t rwsem;
//记录登录状态
atomic_bool g_isLoginSuccess{false};

//接收线程
void readTaskHandler(int clientfd);
//获取系统时间
string getCurrentTime();
//主聊天页面程序
void mainMenu(int);
//显示当前登录成功用户的基本信息
void showCurrentUserData();

//main线程用作发送线程，子线程用作接收线程
int main(int argc, int** argv)
{
    if(argc < 3)
    {
        cerr << "command invalid!" << endl;
        exit(-1);
    }

    //解析命令行参数
    char* ip = argv[1];l
    uint16_t port = atoi(argv[2]);

    //创建client端的socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientfd)
    {
        cerr << "socket create error" <<endl;
        exit(-1);
    }

    sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    if(-1 == connect(clientfd, (sockaddr*)&server, sizeof(sockaddr_in)))
    {
        cerr << "connect server error" <<endl;
        close(clientfd);
        exit(-1);
    }

    //初始化读写线程通信用的信号量
    sem_init(&rwsem, 0, 0);

    std::thread readTask(readTaskHandler,clientfd);
    readTask.detach();

    //main线程用于接收用户输入，负责发送数据
    for(;;)
    {
        
    }
    return 0;
}