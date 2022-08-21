#pragma once
#include <utility>

namespace myserver{
namespace utility{

template<class T>
class Singleton{
public:
    static T* Get(){
        return *GetPPtr();
    }

    //可变参模板，为了解决多个单例之间的依赖问题
    template<class... Args>
    static void New(Args&&... args){
        if(Get() == nullptr){
            *GetPPtr() = new T(std::forward<Args>(args)...);
        }
    }

    static void Delete(){
        if(Get() != nullptr){
            delete Get();
            *GetPPtr() = nullptr;
        }
    }

private:
    Singleton(){}
    Singleton(const Singleton&) = delete;
    Singleton operator=(const Singleton&) = delete;

    //私有获取二级指针方法，为了类内调用
    static T** GetPPtr(){
        static T* ptr = nullptr;
        return &ptr;
    }
};


} // namespace utility
} // namespace myserver
