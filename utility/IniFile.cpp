#include"IniFile.h"

#include<iostream>
#include<fstream>
#include <algorithm>
#include <utility>

namespace myserver{
namespace utility{


Value::Value(){
}

Value::Value(bool value){
    *this = value;
}

Value::Value(int value){
    *this = value;
}

Value::Value(double value){
    *this = value;
}

Value::Value(const string & value) : m_value(value){
}

Value::~Value(){
}

//重载赋值运算符
Value& Value::operator = (bool value){
    if(value)
        m_value = "true";
    else
        m_value = "false";
    return *this;
}
Value & Value::operator = (int value){
    //创建一个流，然后将int输入到流中，再从流中拿到
    ostringstream os;
    os << value;
    m_value = os.str();
    return *this;
}
Value & Value::operator = (double value){
    ostringstream os;
    os << value;
    m_value = os.str();
    return *this;
}
Value& Value::operator = (const string & value){
    m_value = value;
    return *this;
}

//类型转换
Value::operator bool(){
    if(m_value == "true")
        return true;
    else
        return false;
}
Value::operator int(){
    //使用atoi需要c类型的字符串
    return std::atoi(m_value.c_str());
}
Value::operator double(){
    return std::atof(m_value.c_str());
}
Value::operator string(){
    return m_value;
}
Value::operator string() const{
    return m_value;
}


//IniFile类函数的实现
IniFile::IniFile(){
}

IniFile::IniFile(const string &filename){
    load(filename);
}

IniFile::~IniFile(){
}

bool IniFile::load(const string &filename){
    m_iniFileName = filename;
    m_iniFileAttribute.clear();
    //打开文件进行读操作写入m_iniFileAttribute中
    ifstream fin(filename.c_str());
    if(fin.fail()){
        printf("loading file failed: %s is not found.\n", filename.c_str());
        return false;
    }
    //一行行读取数据到line中
    string line;
    string section;
    while(getline(fin, line)){
        line = trim(forward<string>(line));
        //空行直接跳过
        if(line == "")
            continue;
        //如果是[server]这样的就是一个section模块
        if(line[0] == '['){
            //找到第一个']'，返回的是位置信息
            int pos = line.find_first_of(']');
            if(pos != -1){
                //实现完美转发
                section = trim(forward<string>(line.substr(1, pos-1)));
                //标记一个section，为了后面的填充属性
                m_iniFileAttribute[section];
            }
        }
        //注释的话直接跳过
        else if(line[0] == '#')
            continue;
        //是一个key=value的行
        else{
            int pos = line.find_first_of('=');
            if(pos > 0){
                //添加新的key和value到section中
                string key = trim(forward<string>(line.substr(0, pos)));
                string value = trim(forward<string>(line.substr(pos+1, line.size() - pos -1)));
                //判断属性中是否含有section
                unordered_map<string, Section>::iterator it = m_iniFileAttribute.find(section);
                if(it == m_iniFileAttribute.end()){
                    printf("parsing error: section=%s key=%s\n", section.c_str(), key.c_str());
                    return false;
                }
                //添加
                m_iniFileAttribute[section][key] = value;
            }
        }
    }
    return true;
}

//将m_iniFileAttribute写入到一个文件中
void IniFile::save(const string& filename){
    //打开ini文件
    ofstream fout(filename.c_str());
    unordered_map<string, Section>::iterator it;
    for(it = m_iniFileAttribute.begin(); it != m_iniFileAttribute.end(); it++){
        //写section
        fout << "[" << it->first << "]" <<endl;
        for(Section::iterator iter = it->second.begin(); iter != it->second.end(); iter++){
            //写key=value
            fout << iter->first << " = " << (string)iter->second << endl;
        }
        fout << endl;
    }
}

//输出到屏幕上
void IniFile::show(){
    unordered_map<string, Section>::iterator it;
    for(it = m_iniFileAttribute.begin(); it != m_iniFileAttribute.end(); it++){
        //section
        cout << "[" << it->first << "]" <<endl;
        for(Section::iterator iter = it->second.begin(); iter != it->second.end(); iter++){
            //key=value
            cout << iter->first << " = " << (string)iter->second << endl;
        }
        cout << endl;
    }
}

//清除m_iniFileAttribute
void IniFile::clear(){
    m_iniFileAttribute.clear();
}

//是否含有某个section
bool IniFile::has(const string& section){
    return (m_iniFileAttribute.find(section) != m_iniFileAttribute.end());
}

//是否含有某个section下的key
bool IniFile::has(const string& section, const string& key){
    unordered_map<string, Section>::iterator it = m_iniFileAttribute.find(section);
    if(it != m_iniFileAttribute.end()){
        return (it->second.find(key) != it->second.end());
    }
    return false;
}

//拿到value
Value &IniFile::get(const string &section, const string &key)
{
    return m_iniFileAttribute[section][key];
}
//设置value
void IniFile::set(const string &section, const string &key, bool value)
{
    m_iniFileAttribute[section][key] = value;
}
void IniFile::set(const string &section, const string &key, int value)
{
    m_iniFileAttribute[section][key] = value;
}
void IniFile::set(const string &section, const string &key, double value)
{
    m_iniFileAttribute[section][key] = value;
}
void IniFile::set(const string &section, const string &key, const string &value)
{
    m_iniFileAttribute[section][key] = value;
}

}//namespace utility
}//namespace myserver
