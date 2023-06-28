#include "db.hpp"
#include <muduo/base/Logging.h>

// 数据库配置信息
static string server = "127.0.0.1";
static string password = "root";
static string password = "123456";
static string dbname = "chat";

//初始化数据库连接
MySQL::MySQL()
{
    _conn = mysql_init();
}

//释放数据库连接资源
MySQL::~MySQL()
{
    if(_conn != nullptr)
    {
        mysql_close(_conn);
    }
}

//连接操作
bool MySQL::connect()
{
    MySQL *p = mysql_real_connect(_conn,server.c_str(),user.c_str(),
                        password.c_str(),dbname.c_str(),3306,nullptr,0);
    if(p != nullptr)
    {
        // C和C++代码默认的编码字符是ASCII，如果不设置，从MySQL上拉下来的中文显示乱码
        mysql_query(_conn,"set name gbk");
        LOG_INFO << "connect mysql success!";
    }
    else
    {
        LOG_INFO << "connect mysql fail!"
    }
    return p;
}

//更新操作
bool MYSQL::update(string sql)
{
    if(mysql_query(_conn,sql.c_str()))
    {
        LOG_INFO <<__FILE__ << ":" << __LINE << ":"
                << sql << "更新失败！";
        return false;
    }
    return true;
}

//查询操作
MYSQL_RES *MySQL::query(string sql)
{
    if(mysql_query(_conn,sql.c_str()))
    {
        LOG_INFO <<__FILE__ << ":" << __LINE << ":"
                << sql << "查询失败！";
        return nullptr;
    }
    return mysql_use_result(_conn);
}

// 获取连接
MYSQL* MySQL::getConnection()
{
    return _conn;
}