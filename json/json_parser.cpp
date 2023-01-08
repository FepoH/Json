#include <string.h>
#include <stdexcept>
#include <cstdlib>

#include "json_parser.h"
#include "json.h"

namespace fepoh{


JsonParser::JsonParser(const std::string& str):m_str(str),m_idx(0){
}

void JsonParser::reload(const string & str){
    m_str = str;
    m_idx = 0;
}

void JsonParser::skipWihte(){
    while ((m_idx < m_str.size()) && 
            (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' 
             || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')){
        ++m_idx;
    }
}

char JsonParser::getNext(){
    skipWihte();
    if(m_idx >= m_str.size()){
        throw std::exception();
    } 
    return m_str[m_idx++];
}

bool JsonParser::parse(Json& val){
    bool flag = true;
    try{
        val = realParse();
    }catch(...){
        LOG("%s%d","parse error.idx = ",m_idx);
        flag = false;
    }
    return flag;
}

Json JsonParser::realParse(){
    char ch = getNext();
    switch (ch){
        case 'n':
            --m_idx;
            return parseNull();
            break;
        case 't':
        case 'f':
            --m_idx;
            return parseBool();
            break;
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            --m_idx;
            return parseNumber();
            break;
        case '"':
            return parseString();
            break;
        case '[':
            return parseArray();
            break;
        case '{':
            return parseObject();
            break;
        default:
            break;
    }
    LOG("%s%d","parse error.idx = ",m_idx);
    throw std::exception();
}

Json JsonParser::parseNull(){
    if(m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    LOG("%s%d","parse error.idx = ",m_idx);
    throw std::exception();
}

Json JsonParser::parseBool(){
    if(m_str.compare(m_idx, 4, "true") == 0){
        m_idx += 4;
        return Json(true);
    }else if(m_str.compare(m_idx, 5, "false") == 0){
        m_idx += 5;
        return Json(false);
    }
    LOG("%s%d","parse error.idx = ",m_idx);
    throw std::exception();
}

Json JsonParser::parseNumber(){
    size_t pos = m_idx;
    //符号
    if(m_str[m_idx] == '-'){
        ++m_idx;
    } 
    //整数
    if(inRange(m_str[m_idx], '0', '9')){
        ++m_idx;
        while (inRange(m_str[m_idx], '0', '9'))
        {
            ++m_idx;
        }
    }else{
        LOG("%s%d","parse error.idx = ",m_idx);
        throw std::exception();
    }
    //小数
    if(m_str[m_idx] != '.'){
        std::string i = m_str.substr(pos,m_idx - pos);
        return Json(std::atoi(i.c_str()));
    }
    ++m_idx;
    if(!inRange(m_str[m_idx], '0', '9')){
        LOG("%s%d","parse error.idx = ",m_idx);
        throw std::exception();
    }
    while (inRange(m_str[m_idx], '0', '9')){
        ++m_idx;
    }
    std::string f = m_str.substr(pos,m_idx - pos);
    return Json(std::atof(f.c_str()));
}

string JsonParser::parseString(){
    int pos = m_idx;
    while (true){
        if(m_idx >= m_str.size()){
            LOG("%s%d","parse error.idx = ",m_idx);
            throw std::exception();
        }

        char ch = m_str[m_idx++];
        if(ch == '"'){
            break;
        }

        if(ch == '\\'){
            if(m_idx >= m_str.size()){
                LOG("%s%d","parse error.idx = ",m_idx);
                throw std::exception();
            }
            ch = m_str[m_idx++];
            switch (ch){
                case 'b':
                case 't':
                case 'n':
                case 'f':
                case 'r':
                case '"':
                case '\\':
                    break;
                case 'u':
                    m_idx += 4;
                    break;
                default:
                    break;
            }
        }
    }
    //"xxx"a
    //跳出循环时m_idx指向a
    return m_str.substr(pos, m_idx - pos - 2);
}

Json JsonParser::parseArray(){
    Json arr(Json::JSON_ARRAY);
    char ch = getNext();
    if(ch == ']'){
        return arr;
    }
    --m_idx;
    while (true){
        arr.append(realParse());
        ch = getNext();
        if(ch == ']')
        {
            break;
        }
        if(ch != ',')
        {
            LOG("%s%d","parse error.idx = ",m_idx);
            throw std::exception();
        }
    }
    return arr;
}

Json JsonParser::parseObject()
{
    Json obj(Json::JSON_OBJECT);
    char ch = getNext();
    if(ch == '}')
    {
        return obj;
    }
    --m_idx;
    while(true){
        ch = getNext();
        if(ch != '"')
        {
            LOG("%s%d","parse error.idx = ",m_idx);
            throw std::exception();
        }
        string key = parseString();
        ch = getNext();
        if(ch != ':'){
            LOG("%s%d","parse error.idx = ",m_idx);
            throw std::exception();
        }
        obj[key] = realParse();
        ch = getNext();
        if(ch == '}'){
            break;
        }
        if(ch != ','){
            LOG("%s%d","parse error.idx = ",m_idx);
            throw std::exception();
        }
    }
    return obj;
}

}