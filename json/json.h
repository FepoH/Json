#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <initializer_list>

namespace fepoh{

#define LOG(fmt,...) \
    std::string log_info = fepoh::Json_Log(fmt,__VA_ARGS__);  \
    std::cout << __FILE__ << ":" << __LINE__ << "\t" << log_info << std::endl;

std::string Json_Log(const char* fmt,...);


class Json{
public:
    //对象类型
    typedef std::map<std::string,Json> Json_Obj;
    typedef std::shared_ptr<Json> ptr;
    //数据类型
    enum Type{
        JSON_NULL = 0,
        JSON_BOOL,
        JSON_INT,
        JSON_DOUBLE,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT
    };
    //构造
    Json();
    Json(Type type);
    Json(bool val);
    Json(int val);
    Json(double val);
    Json(const std::string& val);
    Json(const char* val);
    Json(const Json& oth);
    Json& operator=(const Json&oth);
    ~Json();
    //初始化列表，bool,int,double,string
    template<class T>
    Json(std::initializer_list<T> list){
        m_type = JSON_ARRAY;
        m_value.m_arr = new std::vector<Json>();
        for(auto& element : list){
            append(element);
        }
    }

    //隐式转换
    operator bool() const;
    operator int() const;
    operator double() const;
    //重载运算符
    bool operator==(const Json& oth) const;
    bool operator!=(const Json& oth) const;
    
    //array
    Json& operator[](int index);
    void append(const Json& val);
    

    template<class T>
    void append(const T& val){
        Json tmp(val);
        append(tmp);
    }

    //object
    Json& operator[](const char* index);
    Json& operator[](const std::string& index);

    bool find(const char* key);
    bool find(const std::string& key);
    void insert(const std::string& key,const Json& val);

    bool has(int index) const;
    bool has(const char* key)const;
    bool has(const std::string& key) const;

    void remove(int index);
    void remove(const char* key);
    void remove(const std::string& key);

    std::string tostring();
    void dump(std::ostream& os);
public:
    bool isNull() const {return m_type == JSON_NULL;}
    bool isBool() const {return m_type == JSON_BOOL;}
    bool isNum() const {return m_type == JSON_INT || m_type == JSON_DOUBLE;}
    bool isInt() const {return m_type == JSON_INT;}
    bool isDouble() const {return m_type == JSON_DOUBLE;}
    bool isStr() const {return m_type == JSON_STRING;}
    bool isArray() const {return m_type == JSON_ARRAY;}
    bool isObject() const {return m_type == JSON_OBJECT;}

    //此处都处理为抛出异常
    /*
        用之前务必进行类型判断
    */
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    double asNum() const;
    std::string asString() const;

    Type getType() const { return m_type; }
private:
    void clear();
    void copy(const Json& oth);
private:
    union Value{
        bool m_bool;
        int m_int;
        double m_double;
        std::string* m_str;
        std::vector<Json>* m_arr;
        Json_Obj* m_obj;
    };
    Value m_value;
    Type m_type;
};

}