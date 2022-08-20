// #pragma once

#include<iostream>
#include <string>
#include<vector>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include"Singleton.h"

using namespace std;

namespace myserver {
namespace utility {

#define __DEBUG__
#define __INFO__
#define __WARN__
#define __ERROR__
#define __FATAL__


#define Log(level, format, ...) \
    Singleton<Logger>::Get()->log(level, __FILE__, __LINE__, format, ##__VA_ARGS__)

//日志级别
class LogLevel {
public:
    //日志级别枚举
    enum Level {
        // 未知级别
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    //将日志级别转成文本输出
    static const string ToString(Level level);

    //将文本转换成日志级别
    static Level FromString(const string& str);
};

class Logger{
public:
    Logger();
    ~Logger();

    void open(const string &logfile);
    void close();
    //所有日志等级的日志都是统一调用了
    //模板在编译器期间确定，需要在头文件中实现
    template<class... Args>
    void log(const string& str_level, const string& file, int line, const string& format, const Args&... args);

    

private:
    static const vector<string> str_level;
    //文件指针
    FILE *m_logfile;
    
};

template<class... Args>
void Logger::log(const string& str_level, const string& file, int line, const string& format, const Args&... args){
    if (m_logfile == NULL){
        printf("open log file failed: m_fp == NULL\n");
        exit(1);
    }

    //取出枚举，再转成字符串
    // LogLevel::Level l1 = LogLevel::FromString(str_level);
    // string s = LogLevel::ToString(l1);

    //时间戳
    const time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    //获取的时间存到buf中
    //2022-08-20 13:16:14  
    char buf[32];
    memset(buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ptm);
    
    //文件锁
    flockfile(m_logfile);

    //写入日志
    fprintf(m_logfile, "%s ", buf);    //时间信息
    fprintf(m_logfile, "%s ", str_level.c_str());   //日志等级
    fprintf(m_logfile, "%s:%d ", file.c_str(), line);   //写入日志的位置
    fprintf(m_logfile, "%s ", format.c_str());
    fprintf(m_logfile, "\r\n"); //换行
    
    //刷新缓冲区
    fflush(m_logfile);

    //文件解锁
    funlockfile(m_logfile);
}

}}
