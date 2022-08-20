#pragma once

#include <string.h>
#include <string>
#include <sstream>
#include<unordered_map>

using namespace std;
using std::ostringstream;

namespace myserver {
namespace utility {

class Value{
public:
    Value();
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const string & value);
    ~Value();

    Value & operator = (bool value);
    Value & operator = (int value);
    Value & operator = (double value);
    Value & operator = (const string & value);
    
    operator bool();
    operator int();
    operator double();
    operator string();
    operator string() const;

private:
    string m_value;
};

class IniFile{
private:
    //使用unordered_map,因为只是一次性能的存储，而map底层使用红黑树会占用更多的内存空间
    typedef unordered_map<string, Value> Section;

public:
    IniFile();
    IniFile(const string &filename);
    ~IniFile();

    bool load(const string &filename);
    void save(const string &filename);
    void show();
    void clear();

    // read values in different formats
    Value & get(const string &section, const string &key);

    // set values in different formats
    void set(const string &section, const string &key, bool value);
    void set(const string &section, const string &key, int value);
    void set(const string &section, const string &key, double value);
    void set(const string &section, const string &key, const string &value);

    bool has(const string &section);
    bool has(const string &section, const string &key);

    void remove(const string &section);
    void remove(const string &section, const string &key);

    Section & operator [] (const string & key)
    {
        return m_iniFileAttribute[key];
    }

private:
    //去除字符串首尾的空格,实现完美转发
    template<class T>
    T& trim(T&& s){
        if (s.empty()){
            return s;
        }
        s.erase(0, s.find_first_not_of(" \r\n"));
        s.erase(s.find_last_not_of(" \r\n") + 1);
        return s;
    }

private:
    string m_iniFileName;
    //两层map其实
    unordered_map<string, Section> m_iniFileAttribute;
};



}}
