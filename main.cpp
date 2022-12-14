#include <iostream>
#include"Singleton.h"
#include "Logger.h"
#include "System.h"
#include "IniFile.h"
#include "Socket.h"

using namespace std;
using namespace myserver::utility;
using namespace myserver::socket;

class A{
public:
    template<class T>
    void print(const T& t){
        cout<<t<<endl;
    }

    template<class T, class... Args>
    void print(const T& t, const Args&... args){
        cout<<t<<endl;
        print(args...);
    }
};


int main(){
    // 初始化单例
    Singleton<Logger>::New();
    Singleton<IniFile>::New();
    Singleton<System>::New();
    Singleton<Socket>::New();

    Singleton<System>::Get()->init();

    IniFile * ini = Singleton<IniFile>::Get();
    ini->load( + "/home/ubuntu/myserver/config/main.ini");
    const string & ip = (*ini)["server"]["ip"];
    int port = (*ini)["server"]["port"];
    int threads = (*ini)["server"]["threads"];
    int max_conn = (*ini)["server"]["max_conn"];

    cout<<max_conn<<endl;
    
    Log("error","shuchu");

    


    Singleton<IniFile>::Delete();
    Singleton<Logger>::Delete();
    Singleton<System>::Delete();
    return 0;
}