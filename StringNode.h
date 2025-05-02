#pragma once
#ifndef STRING_NODE_H
#define STRING_NODE_H

#include <string>

class StringNode {
public:
    std::string data;
    StringNode* next;

    StringNode(const std::string& str = "") : data(str), next(nullptr) {}
    //构造函数的初始化列表
};


#endif