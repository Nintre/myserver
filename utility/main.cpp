#include <iostream>
#include "Singleton.h"
#include "Logger.h"
#include "System.h"

using namespace std;
using namespace myserver::utility;

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
    // Singleton<IniFile>::New();
    // IniFile * ini = Singleton<IniFile>::Get();
    // ini->load( + "/home/ubuntu/myserver/config/main.ini");
    // const string & ip = (*ini)["server"]["ip"];
    // int port = (*ini)["server"]["port"];
    // int threads = (*ini)["server"]["threads"];
    // int max_conn = (*ini)["server"]["max_conn"];

    Singleton<System>::New();
    Singleton<Logger>::New();


    System * sys = Singleton<System>::Get();
    
    cout<<sys->get_root_path()<<endl;
    

    Logger* logger = Singleton<Logger>::Get();
    Log("error","shuchu");


    Singleton<Logger>::Delete();
    Singleton<Logger>::Delete();

    return 0;
}