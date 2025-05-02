#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "StringNode.h"

class StringList {
    private:
        StringNode* head;
        // 辅助方法，用于释放从指定节点开始的链表内存
        void freeNodes(StringNode* node) {
            while (node) {
                StringNode* next = node->next;
                delete node;
                node = next;
            }
        }
        // 辅助函数：获取链表长度
        int getListLength() const;
        // 辅助函数：查找子链表起始节点位置
        StringNode* findSubListNode(const StringList& sub) const;

    public:
        StringList();
        StringList(const StringList& other);  // ADD: 深拷贝构造函数
        StringList& operator=(const StringList& other);  // ADD: 深拷贝赋值运算符
        ~StringList();
        void buildFromString(const std::string& s);
        bool equals(const StringList& other) const;//判断相等
        void concatenate(const StringList& other);
        int getLength() const;//获取长度
        StringList getSubList(int start, int len) const;
        int findSubList(const StringList& sub) const;
        bool replaceSubList(const StringList& oldSub, const StringList& newSub);
        bool saveToFile(const std::string& filename) const;//保存到文件
        bool loadFromFile(const std::string& filename);//文件获取
        void print() const;//输出当前
};

#endif
