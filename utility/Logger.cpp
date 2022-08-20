#include "Logger.h"

namespace myserver {
namespace utility {


const string LogLevel::ToString(LogLevel::Level level){
    switch(level){
        #define XX(name) case LogLevel::name: return #name; break;
        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
        #undef XX

        default:
            return "UNKNOW";
    }
    return "UNKNOW";
}

LogLevel::Level LogLevel::FromString(const std::string& str) {
#define XX(level, v) \
    if(str == #v) { \
        return LogLevel::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
    return LogLevel::UNKNOW;
#undef XX
}

//构造函数,先不打开文件
Logger::Logger() : m_logfile(nullptr){}

Logger::~Logger(){
    close();
}

//这里的做法是当项目启动就在init中调用了open函数
//也就是说日志文件是伴随项目的一生的
//所以析构函数就是关闭文件，后面还需优化
void Logger::open(const string &logfile){
    close();
    m_logfile = fopen(logfile.c_str(), "a+");
    if (m_logfile == nullptr){
        printf("open log file failed: %s\n", logfile.c_str());
        exit(1);
    }
}

void Logger::close(){
    if (m_logfile != nullptr){
        fclose(m_logfile);
        m_logfile = nullptr;
    }
}

}}
