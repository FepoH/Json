/*
 * @Author: fepo_h
 * @Date: 2023-01-07 23:24:04
 * @LastEditors: fepo_h
 * @LastEditTime: 2023-01-08 23:26:08
 * @FilePath: /fepoh/workspace/tests/json/json/test.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by FepoH Fepo_H@163.com, All Rights Reserved. 
 * @version: V1.0.0
 * @Mailbox: Fepo_H@163.com
 * @Descripttion: 
 */
#include <iostream>
#include <unordered_map>
#include "json.h"
#include "json_parser.h"

using namespace fepoh;

void test_construct(){
    std::cout << "hello json" << std::endl;
    Json a;
    Json a1 (10);
    Json a2 (true);
    Json a3 (10.23);
    Json a4 ("asfdasdfs");
    a.append(a1);
    a.append(a3);
    a.append(a2);
    a.append(a1);
    a.dump(std::cout);
    a1.dump(std::cout);
    a2.dump(std::cout);
    a3.dump(std::cout);
    a4.dump(std::cout);
    
    int p = a1;
    double b = a3;
    bool flag = a2;
    std::cout << p << b << flag<< std::endl;
}

void test_arr(){
    std::cout << "-----arr-----" << std::endl;
    Json arr(Json::JSON_ARRAY);
    Json a1 (10);
    Json a2 (true);
    Json a3 (10.23);
    Json a4 ("asfdasdfs");
    arr.append(a1);
    arr.append(a2);
    arr.append(a3);
    arr.append(a4);
    arr.dump(std::cout);
}

void test_obj(){
    std::cout << "-----obj-----" << std::endl;
    Json obj;
    Json nu;
    obj["null"] = nu;
    obj["int"] = 10;
    obj["bool"] = true;
    obj["double"] = 10.23;
    obj["str"] = "asfdasdfs";
    Json arr = {"10","12","45","fdadsf"};
    arr.append(1);
    arr.remove(10);
    obj.remove("str");
    std::cout << obj.has("null") << std::endl;;
    obj["arr"] = arr;
    obj.dump(std::cout);
}

void test_parser(){
    std::cout << "------parser----" <<std::endl;
    JsonParser parser("{\"features\":[{\"id\":1,\"geometry\":{\"type\":\"Point\",\"coordinates\":[533958.52189999819,3123489.1460000016]}},{\"id\":1,\"geometry\":{\"type\":\"Point\",\"coordinates\":[533958.52189999819,3123489.1460000016]}}]}");
    Json j;
    bool rt = parser.parse(j);
    std::cout << rt << std::endl;
    std::cout << j.tostring() << std::endl;
    parser.reload("{\"employees\": [{ \"firstName\":\"Bill\" , \"lastName\":\"Gates\" },{ \"firstName\":\"George\" , \"lastName\":\"Bush\" },{ \"firstName\":\"Thomas\" , \"lastName\":\"Carter\" }]}");
    rt = parser.parse(j);
    std::cout << rt << std::endl;
    std::cout << j.tostring() << std::endl;
}

int main(int argc,char* argv[]){
    test_construct();
    test_arr();
    test_obj();
    test_parser();
}