#include"System.h"

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "Logger.h"
#include "IniFile.h"
#include "Singleton.h"

using namespace myserver::utility;

namespace myserver{
namespace utility{


System::System(){
}

System::~System(){
}


void System::init(){
    core_dump();
    //获取根路径
    m_root_path = get_root_path();

    const string logdir = m_root_path + "/log";
    //是否存在该log目录
    DIR * dp = opendir(logdir.c_str());
    if (dp == nullptr){
        //创建目录，权限为0755，当前用户可读可写，组内和其他组用户只可读
        mkdir(logdir.c_str(), 0755);
    }
    else{
        closedir(dp);
    }

    //打开main.log文件
    Singleton<Logger>::Get()->open(m_root_path + "/log/main.log");

    // 初始化inifile解析器
    Singleton<IniFile>::Get()->load(get_root_path() + "/config/main.ini");
}

//核心转储
void System::core_dump(){
    // core dump
    struct rlimit x;
    int ret = getrlimit(RLIMIT_CORE, &x);
    x.rlim_cur = x.rlim_max;
    ret = setrlimit(RLIMIT_CORE, &x);
    ret = getrlimit(RLIMIT_DATA, &x);
    x.rlim_cur = 768000000;
    ret = setrlimit(RLIMIT_DATA, &x);
}


string System::get_root_path(){
    if (m_root_path != "")
    {
        return m_root_path;
    }
    char path[1024];
    memset(path, 0, 1024);
    int cnt = readlink("/proc/self/exe", path, 1024);
    if (cnt < 0 || cnt >= 1024)
    {
        return "";
    }
    for (int i = cnt; i >= 0; --i)
    {
        if (path[i] == '/')
        {
            path[i] = '\0';
            break;
        }
    }
    return string(path);
}

}   // namespace utility
}   // namespace myserver
