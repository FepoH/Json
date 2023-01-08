#include "json.h"
#include <exception>
#include <sstream>
#include <stdarg.h>

namespace fepoh{



std::string Json_Log(const char* fmt,...){
    va_list vl;
    va_start(vl,fmt);
    char* buf = nullptr;
    vasprintf(&buf,fmt,vl);
    va_end(vl);
    std::string str(buf);
    return str;
}

Json::Json(Type type):m_type(type){
    switch(m_type)
    {
    case JSON_NULL:
        break;
    case JSON_BOOL:
        m_value.m_bool = false;
        break;
    case JSON_INT:
        m_value.m_int = 0;
        break;
    case JSON_DOUBLE:
        m_value.m_double = 0;
        break;
    case JSON_STRING:
        m_value.m_str = new std::string();
        break;
    case JSON_ARRAY:
        m_value.m_arr = new std::vector<Json>();
        break;
    case JSON_OBJECT:
        m_value.m_obj = new Json_Obj();
        break;
    default:
        m_type = JSON_NULL;
        break;
    }
}

Json::Json():m_type(JSON_NULL){
}

Json::Json(bool val):m_type(JSON_BOOL){
    m_value.m_bool = val;
}

Json::Json(int val):m_type(JSON_INT){
    m_value.m_int = val;
}

Json::Json(double val):m_type(JSON_DOUBLE){
    m_value.m_double = val;
}

Json::Json(const std::string& val):m_type(JSON_STRING){
    m_value.m_str = new std::string(val);
}

Json::Json(const char* val):m_type(JSON_STRING){
    m_value.m_str = new std::string(val);
}

Json::Json(const Json& oth){
    copy(oth);
}

Json::~Json(){
    clear();
}

Json& Json::operator=(const Json&oth){
    clear();
    copy(oth);
    return *this;
}

void Json::copy(const Json& oth){
    m_type = oth.m_type;
    switch(m_type)
    {
    case JSON_NULL:
        break;
    case JSON_BOOL:
        m_value.m_bool = oth.m_value.m_bool;
        break;
    case JSON_INT:
        m_value.m_int = oth.m_value.m_int;
        break;
    case JSON_DOUBLE:
        m_value.m_double = oth.m_value.m_double;
        break;
    case JSON_STRING:
        m_value.m_str = new std::string(*(oth.m_value.m_str));
        break;
    case JSON_ARRAY:
        m_value.m_arr = new std::vector<Json>(oth.m_value.m_arr->begin(),oth.m_value.m_arr->end());
        break;
    case JSON_OBJECT:
        m_value.m_obj = new Json_Obj(oth.m_value.m_obj->begin(),oth.m_value.m_obj->end());
        break;
    default:
        m_type = JSON_NULL;
        break;
    }
}

Json::operator bool() const{
    if(m_type != JSON_BOOL){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return m_value.m_bool;
}

Json::operator int() const{
    if(m_type != JSON_INT){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return m_value.m_int;
}

Json::operator double() const{
    if(m_type != JSON_DOUBLE){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return m_value.m_double;
}

Json& Json::operator[](int index){
    if(m_type != JSON_ARRAY){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return m_value.m_arr->at(index);
}

Json& Json::operator[](const char* index){
    std::string tmp(index);
    return operator[](tmp);
}

Json& Json::operator[](const std::string& index){
    if(m_type != JSON_OBJECT){
        if(m_type == JSON_NULL){
            m_type = JSON_OBJECT;
            m_value.m_obj = new Json_Obj();
        }else{
            LOG("%s%d","error type:m_type = ",m_type);
            throw std::exception();
        }
    }
    return (*m_value.m_obj)[index];
}

void Json::append(const Json& val){
    if(m_type == JSON_ARRAY){
        m_value.m_arr->push_back(val);
    }else{
        std::vector<Json>* tmp = new std::vector<Json>();
        tmp->push_back(*this);
        tmp->push_back(val);
        clear();
        m_type = JSON_ARRAY;
        m_value.m_arr = tmp;
    }
}

bool Json::find(const char* key){
    std::string tmp(key);
    return find(tmp); 
}

bool Json::find(const std::string& key){
    if(m_type != JSON_OBJECT){
        return false;
    }
    return m_value.m_obj->find(key) != m_value.m_obj->end();
}

void Json::insert(const std::string& key,const Json& val){
    operator[](key) = val;
}

bool Json::operator==(const Json& oth) const{
    if(m_type != oth.m_type){
        return false;
    }
    switch (m_type)
    {
    case JSON_NULL:
        return true;
        break;
    case JSON_BOOL:
        return m_value.m_bool == oth.m_value.m_bool;
        break;
    case JSON_INT:
        return m_value.m_int == oth.m_value.m_int;
        break;
    case JSON_STRING:
        return *(m_value.m_str) == *(oth.m_value.m_str);
        break;
    case JSON_ARRAY:
        return *(m_value.m_arr) == *(oth.m_value.m_arr);
        break;
    case JSON_OBJECT: 
        return *(m_value.m_obj) == *(oth.m_value.m_obj);
        break;
    default:
        break;
    }
    return false;
}

bool Json::operator!=(const Json& oth) const{
    return !(*this == oth);
}

void Json::clear(){
    switch (m_type){
    case JSON_STRING:
        delete m_value.m_str;
        m_value.m_str = nullptr;
        break;
    case JSON_ARRAY:
        delete m_value.m_arr;
        m_value.m_arr = nullptr;
        break;
    case JSON_OBJECT:
        delete m_value.m_obj;
        m_value.m_obj = nullptr;
        break;
    default:
        break;
    }
    m_type = JSON_NULL;
}


std::string Json::tostring(){
    std::stringstream ss;
    switch (m_type)
    {
    case JSON_NULL:
        ss << "null";
        break;
    case JSON_BOOL:{
        if(m_value.m_bool){
            ss << "true";
        }else{
            ss << "false";
        }
        break;
    }  
    case JSON_INT:
        ss << m_value.m_int;
        break;
    case JSON_DOUBLE:
        ss << m_value.m_double;
        break;
    case JSON_STRING:
        ss << "\"" << *m_value.m_str << "\"";
        break;
    case JSON_ARRAY:{
        ss << "[";
        for(int i=0;i<(int)m_value.m_arr->size();++i){
            if(i != 0){
                ss << ",";
            }
            ss << (m_value.m_arr->at(i)).tostring();
        }
        ss << "]";
        break;
    }
    case JSON_OBJECT:{
        ss << "{";
        bool flag = false;
        for(auto& element : (*m_value.m_obj)){
            if(flag){
                ss << ",";
            }
            flag = true;
            ss << "\"" << element.first << "\"" << ":" << element.second.tostring();
        }
        ss << "}";
        break;
    }
    default:
        m_type = JSON_NULL;
        ss << "null";
        break;
    }
    return ss.str();
}

void Json::dump(std::ostream& os){
    os << tostring() << std::endl;;
}

bool Json::asBool() const{
    if(m_type != JSON_BOOL){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return m_value.m_bool;
}

int Json::asInt() const{
    if(m_type != JSON_INT){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return m_value.m_int;
}

double Json::asDouble() const{
    if(m_type != JSON_DOUBLE){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return m_value.m_double;
}

std::string Json::asString() const{
    if(m_type != JSON_STRING){
        LOG("%s%d","error type:m_type = ",m_type);
        throw std::exception();
    }
    return *m_value.m_str;
}

double Json::asNum() const{
    if(m_type == JSON_INT){
        return asInt();
    }else if(m_type == JSON_DOUBLE){
        return asDouble();
    }
    LOG("%s%d","error type:m_type = ",m_type);
    throw std::exception();
    return 0;
}

bool Json::has(int index) const{
    if(index < 0){
        return false;
    }
    if(m_type != JSON_ARRAY){
        return false;
    }
    return m_value.m_arr->size() > index;
}
bool Json::has(const char* key)const{
    std::string tmp(key);
    return has(tmp);
}

bool Json::has(const std::string& key) const{
    if(m_type != JSON_OBJECT){
        return false;
    }
    auto it = m_value.m_obj->find(key);
    return it != m_value.m_obj->end();
}

void Json::remove(int index){
    if(!has(index)){
        return ;
    }
    m_value.m_arr->erase(m_value.m_arr->begin() + index);
}

void Json::remove(const char* key){
    std::string tmp(key);
    remove(tmp);
}

void Json::remove(const std::string& key){
    if(!has(key)){
        return ;
    }
    auto it = m_value.m_obj->find(key);
    if(it != m_value.m_obj->end()){
        m_value.m_obj->erase(it);
    }
}


}
