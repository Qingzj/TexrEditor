#include "StringList.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>

// 构造函数，初始化链表头为空
StringList::StringList() : head(nullptr) {}

// MODIFY: 删除重复释放逻辑
StringList::~StringList() {
    freeNodes(head);  // 只需调用一次释放函数
    head = nullptr;   // 确保 head 置空
}

// ADD: 深拷贝构造函数
StringList::StringList(const StringList& other) : head(nullptr) {
    StringNode* otherCurrent = other.head;
    StringNode** currentPtr = &head;  // 指向当前链表最后一个节点的 next 指针
    while (otherCurrent != nullptr) {
        *currentPtr = new StringNode(otherCurrent->data);
        currentPtr = &((*currentPtr)->next);
        otherCurrent = otherCurrent->next;
    }
}

// ADD: 深拷贝赋值运算符
StringList& StringList::operator=(const StringList& other) {
    if (this != &other) {
        // 清空当前链表
        freeNodes(head);
        head = nullptr;

        // 深拷贝新链表
        StringNode* otherCurrent = other.head;
        StringNode** currentPtr = &head;
        while (otherCurrent != nullptr) {
            *currentPtr = new StringNode(otherCurrent->data);
            currentPtr = &((*currentPtr)->next);
            otherCurrent = otherCurrent->next;
        }
    }
    return *this;
}


// MODIFY: 将整个字符串作为单个节点
void StringList::buildFromString(const std::string& s) {
    // 清空链表
    freeNodes(head);
    head = nullptr;

    //if (!s.empty()) {
    //    // 直接创建包含整个字符串的节点（不再按空格分割）
    //    head = new StringNode(s);
    //}

    if (!s.empty()) {
        try {
            head = new StringNode(s);
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "内存分配失败: " << e.what() << std::endl;
        }
    }
}

// 比较两个链表是否相等
bool StringList::equals(const StringList& other) const {//接收一个 const StringList& 类型的引用 other
    StringNode* current1 = head;
    StringNode* current2 = other.head;
    // 处理链表为空的情况
    if ((current1 == nullptr && current2 != nullptr) || (current1 != nullptr && current2 == nullptr)) {
        return false;
    }
    while (current1 && current2) {
        std::cout << "比较: " << current1->data << " 和 " << current2->data << std::endl;
        if (current1->data != current2->data) {
            return false;
        }
        current1 = current1->next;
        current2 = current2->next;
    }
    return (!current1 && !current2);//如果两个指针都为空，即两个链表同时遍历完，说明两个链表长度相等且所有对应节点的数据都相等，函数返回 true
}

// 连接另一个链表到当前链表
void StringList::concatenate(const StringList& other) {
    /*if (!head) {
        head = other.head; // 这里直接赋值，会导致所有权问题
    }
    else {
        StringNode* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = other.head;
    }*/
    //StringNode* current = head;
    std::mutex listMutex;  // 添加互斥锁成员
    std::lock_guard<std::mutex> guard(listMutex);  // 加锁
    if (!head) {
        // 如果当前链表为空，直接复制 other 链表
        if (other.head) {
            head = new StringNode(other.head->data);
            StringNode* current = head;
            StringNode* otherCurrent = other.head->next;
            while (otherCurrent) {
                current->next = new StringNode(otherCurrent->data);
                current = current->next;
                otherCurrent = otherCurrent->next;
            }
        }
    }
    else {
        // 找到当前链表的尾节点
        StringNode* current = head;
        while (current->next) {
            current = current->next;
        }
        // 复制 other 链表的节点
        StringNode* otherCurrent = other.head;
        while (otherCurrent) {
            current->next = new StringNode(otherCurrent->data);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
    }
}
// 获取链表表示字符串的长度
//int StringList::getLength() const {
//    int length = 0;
//    StringNode* current = head;
//    while (current) {
//        //length += current->data.size();
//        if (current) {  // 再次确认指针有效
//            length += current->data.size();
//        }
//        current = current->next;
//    }
//    return length;
//}
int StringList::getLength() const {
    int length = 0;
    StringNode* current = head;
    while (current) {
        length += current->data.size();
        current = current->next;
    }
    return length;
}

// MODIFY: 修复起始位置计算和类型转换,子串截取逻辑,获取子链表
StringList StringList::getSubList(int start, int len) const {
    StringList subList;
    if (start < 0 || len <= 0) return subList;

    StringNode* current = head;
    int totalChars = 0;
    while (current) {
        totalChars += current->data.size();
        current = current->next;
    }
    if (start >= totalChars) return subList;

    // 定位起始节点
    current = head;
    int accumulated = 0;
    while (current && accumulated + current->data.size() < start) {
        accumulated += current->data.size();
        current = current->next;
    }
    if (!current) return subList;

    // 计算起始节点内的偏移
    int nodeStart = start - accumulated;
    if (nodeStart < 0 || nodeStart >= current->data.size()) {
        return subList;
    }

    int remaining = len;
    StringNode* subCurrent = nullptr;

    // 截取子串
    while (current && remaining > 0) {
        size_t available = current->data.size() - nodeStart;
        size_t nodeTake = std::min(available, static_cast<size_t>(remaining));

        if (nodeTake == 0) break; // 无字符可截取时退出

        std::string subStr = current->data.substr(nodeStart, nodeTake);
        StringNode* newNode = new StringNode(subStr);

        if (!subList.head) {
            subList.head = newNode;
            subCurrent = newNode; // subCurrent 初始化为首个节点
        }
        else {
            // 确保 subCurrent 不为 NULL
            if (subCurrent != nullptr) {
                subCurrent->next = newNode;
                subCurrent = newNode;
            }
            else {
                // 异常处理：若 subCurrent 意外为 NULL，重置链表头
                subList.head = newNode;
                subCurrent = newNode;
            }
        }

        remaining -= nodeTake;
        nodeStart = 0;
        current = current->next;
    }

    return subList;
}


int StringList::findSubList(const StringList& sub) const {


    if (sub.head == nullptr) return -1;  // 空子串直接返回

    StringNode* current = head;
    int globalPos = 0;  // 全局字符位置

    while (current != nullptr) {
        std::string currentData = current->data;

        size_t dataLength = currentData.size();

        // 在当前节点的数据中搜索子串
        for (size_t i = 0; i <= dataLength - sub.head->data.size(); ++i) {
            bool match = true;
            StringNode* subCurrent = sub.head;
            size_t tempPos = i;
            StringNode* tempNode = current;

            // 匹配子链表的每个节点
            while (subCurrent != nullptr && tempNode != nullptr) {
                std::string subStr = subCurrent->data;
                // 确保tempNode不为空后再访问data
                if (tempNode == nullptr) {
                    match = false;
                    break;
                }
                // 检查当前节点剩余部分是否匹配子串
                if (tempPos + subStr.size() > tempNode->data.size()) {
                    match = false;
                    break;
                }

                if (tempNode->data.substr(tempPos, subStr.size()) != subStr) {
                    match = false;
                    break;
                }
                tempPos = 0;  // 后续节点从开头开始匹配
                subCurrent = subCurrent->next;
                tempNode = tempNode->next;
            }

            if (match && subCurrent == nullptr) {
                return globalPos + i;  // 返回全局位置
            }
        }

        globalPos += dataLength;
        current = current->next;
    }

    return -1;
}
//替换子链表
//int StringList::findSubList(const StringList& sub) const {
//    if (sub.head == nullptr) return -1;
//
//    StringNode* current = head;
//    int globalPos = 0;
//
//    while (current != nullptr) {
//        std::string currentData = current->data;
//        size_t dataLength = currentData.size();
//
//        // 遍历所有可能的起始位置（包括跨节点）
//        for (size_t i = 0; i < dataLength; ++i) {  // 修改循环条件
//            bool match = true;
//            StringNode* subCurrent = sub.head;
//            size_t tempPos = i;
//            StringNode* tempNode = current;
//
//            while (subCurrent != nullptr && tempNode != nullptr) {
//                std::string subStr = subCurrent->data;
//                if (tempPos + subStr.size() > tempNode->data.size()) {
//                    match = false;
//                    break;
//                }
//                if (tempNode->data.substr(tempPos, subStr.size()) != subStr) {
//                    match = false;
//                    break;
//                }
//                tempPos = 0;
//                subCurrent = subCurrent->next;
//                tempNode = tempNode->next;
//            }
//
//            if (match && subCurrent == nullptr) {
//                return globalPos + static_cast<int>(i);
//            }
//        }
//
//        globalPos += static_cast<int>(currentData.size());
//        current = current->next;
//    }
//
//    return -1;
//}

bool StringList::replaceSubList(const StringList& oldSub, const StringList& newSub) {
    if (oldSub.head == nullptr || oldSub.getLength() == 0) {
        return false; // 旧子串为空，直接返回失败
    }

    bool replaced = false;
    StringNode* current = head;
    StringNode* prev = nullptr;
    const int oldSubTotalLength = oldSub.getLength();

    while (current != nullptr && !replaced) {
        std::string currentData = current->data;

        size_t dataLength = currentData.size();

        // 遍历当前节点中所有可能的起始位置
        for (size_t i = 0; i <= dataLength; ++i) { // 注意循环条件改为 <= dataLength
            bool match = true;
            StringNode* subCurrent = oldSub.head;
            size_t tempPos = i;
            StringNode* tempNode = current;

            // 逐节点匹配旧子串
            while (subCurrent != nullptr && tempNode != nullptr) {
                const std::string& subStr = subCurrent->data;

                // 检查当前节点剩余长度是否足够匹配子串
                if (tempPos + subStr.size() > tempNode->data.size()) {
                    match = false;
                    break;
                }

                // 检查子串内容是否匹配
                if (tempNode->data.substr(tempPos, subStr.size()) != subStr) {
                    match = false;
                    break;
                }

                // 移动到下一个子串节点，重置偏移为0
                tempPos = 0;
                subCurrent = subCurrent->next;
                tempNode = tempNode->next;
            }

            // 完全匹配且旧子串已遍历完
            if (match && subCurrent == nullptr) {
                // 分割当前节点为前缀和后缀
                std::string prefix = currentData.substr(0, i);
                std::string suffix;
                if (i + oldSubTotalLength <= currentData.size()) {
                    suffix = currentData.substr(i + oldSubTotalLength);
                }

                // 更新当前节点为前缀
                current->data = prefix;

                // 深拷贝新子串的节点
                StringNode* newHead = nullptr;
                StringNode* newTail = nullptr;
                StringNode* subNew = newSub.head;
                while (subNew != nullptr) {
                    StringNode* newNode = new StringNode(subNew->data);
                    if (newHead == nullptr) {
                        newHead = newNode;
                        newTail = newNode;
                    }
                    else {
                        newTail->next = newNode;
                        newTail = newNode;
                    }
                    subNew = subNew->next;
                }

                // 处理后缀节点
                StringNode* suffixNode = nullptr;
                if (!suffix.empty()) {
                    suffixNode = new StringNode(suffix);
                }

                // 获取原后续节点
                StringNode* originalNext = current->next;

                // 链接新子串和后缀
                current->next = newHead; // 当前节点指向新子串头部
                if (newTail != nullptr) {
                    newTail->next = suffixNode; // 新子串尾部链接后缀
                }
                else {
                    current->next = suffixNode; // 新子串为空时直接链接后缀
                }

                if (suffixNode != nullptr) {
                    suffixNode->next = originalNext; // 后缀链接到原后续节点
                }

                replaced = true;
                break;
            }
        }

        if (!replaced) {
            prev = current;
            current = current->next;
        }
    }

    return replaced;
}
// 保存链表内容到文件
bool StringList::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        StringNode* current = head;
        while (current) {
            file << current->data;
            current = current->next;
        }
        file.close();
        return true;
    }
    return false;
}

// 从文件加载内容到链表
bool StringList::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line;
        }
        buildFromString(content);
        file.close();
        return true;
    }
    return false;
}

// 打印链表内容
void StringList::print() const {
    StringNode* current = head;
    while (current) {
        std::cout << current->data;
        current = current->next;
    }
    std::cout << std::endl;
}

// 辅助函数：获取链表长度
int StringList::getListLength() const {
    int count = 0;
    StringNode* current = head;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

// 辅助函数：查找子链表起始节点位置
StringNode* StringList::findSubListNode(const StringList& sub) const {
    StringNode* current = head;
    while (current) {
        StringNode* tempCurrent = current;
        StringNode* subCurrent = sub.head;
        while (subCurrent && tempCurrent) {
            if (tempCurrent->data != subCurrent->data) {
                break;
            }
            tempCurrent = tempCurrent->next;
            subCurrent = subCurrent->next;
        }
        if (!subCurrent) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}