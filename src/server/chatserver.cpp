#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
            const InetAddress &listenAddr,
            const string &nameArg)
    :_server(loop, listenAddr, nameArg), _loop(loop)
{
    //注册链接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    
    //注册消息回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    
    //设置线程数量
    _server.setThreadNum(4);
}
// 启动服务
void ChatServer::start()
{
    _server.start();
}

// 设置连接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    //客户端断开连接
    if(!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 设置读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                Buffer *buffer,
                Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    //测试
    cout<< buf << endl;

    //数据的反序列化
    json js = json::parse(buf);

    //达到的目的：解耦合网络模块和业务模块
    //通过js["msgid"]获取--》业务handler
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());

    //回调消息绑定好的事件处理器，执行相应的业务处理
    msgHandler(conn, js, time);
}


