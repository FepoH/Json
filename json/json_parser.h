/*
 * @Author: fepo_h
 * @Date: 2022-12-16 12:30:01
 * @LastEditors: fepo_h
 * @LastEditTime: 2023-01-08 22:48:49
 * @FilePath: /fepoh/workspace/tests/json/json/json_parser.h
 * @Description: 
 * 
 * Copyright (c) 2022 by FepoH Fepo_H@163.com, All Rights Reserved. 
 * @version: V1.0.0
 * @Mailbox: Fepo_H@163.com
 * @Descripttion: 
 */
#pragma once

#include <string>
#include <memory>
using namespace std;


namespace fepoh{

class Json;

class JsonParser{
public:
    typedef std::shared_ptr<JsonParser> ptr;
    JsonParser(const std::string& str = "");
    ~JsonParser(){}
    //重新加载一个字符串
    void reload(const string & str);
    bool parse(Json& val);
private:
    //跳过空白字符
    void skipWihte();
    //返回当前字符，m_idx指向当前字符的下一个字符
    char getNext();
    //递归
    Json realParse();
    //解析
    Json parseNull();
    Json parseBool();
    Json parseNumber();
    string parseString();
    Json parseArray();
    Json parseObject();

    inline bool inRange(int x, int lower, int upper){
        return (x >= lower && x <= upper);
    }

private:
    string m_str;   //字符串
    int m_idx;      //索引
};

}
