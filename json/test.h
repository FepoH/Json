/*
 * @Author: fepo_h
 * @Date: 2023-01-08 14:36:48
 * @LastEditors: fepo_h
 * @LastEditTime: 2023-01-08 16:18:34
 * @FilePath: /fepoh/workspace/tests/json/json/test.h
 * @Description: 
 * 
 * Copyright (c) 2023 by FepoH Fepo_H@163.com, All Rights Reserved. 
 * @version: V1.0.0
 * @Mailbox: Fepo_H@163.com
 * @Descripttion: 
 */
#pragma once
#include <string.h>
#include <memory>
#include <sstream>

namespace fepoh{



class MStr{
public:
    //空构造函数
    MStr();
    //字符串构造
    MStr(const char* str);
    MStr(const std::string& str);
    //拷贝构造
    MStr(const MStr& oth);
    //析构函数
    ~MStr();
    //赋值构造
    MStr& operator=(const MStr& oth);
    //运算符重载
    bool operator==(const MStr& oth);
    bool operator!=(const MStr& oth);
    bool operator<(const MStr& oth);
    bool operator>(const MStr& oth);

    // MStr& operator+(const char* str);
    // MStr& operator+(const std::string& str);
    // MStr& operator+(const char* str);

    void resize(int size);
    void resizeAndRemain(int size);

    void insert(int index,char val);
    bool del(int index);
    bool isUnique();
    void clear();

    void dump(){
        std::stringstream ss;
        int i = 0;
        while(i < m_length){
            
            ss << m_str[i];
            ++i;
        }
        std::cout << ss.str() << std::endl;
    }
private:
    void alloc(size_t size);
    void privateClear();
private:
    int m_length;     //长度
    int m_capcity;  //内存大小
    char* m_str;    //字符串
    int* m_count;   //指针引用计数
};

}

//设计一个