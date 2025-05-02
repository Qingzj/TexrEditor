#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "StringList.h"
#include <vector>
#include <string>

class TextEditor {
private:
    StringList currentString;
    // 改动 1：将 operationLog 声明为 mutable，允许在 const 成员函数中修改
    mutable std::vector<std::string> operationLog;

    std::string stripQuotes(const std::string& s);

public:
    bool assign(const std::string& s);
    // 改动 2：若之前 compare 为 const 函数，去掉 const 修饰符；若用 mutable 方案，可保留
    bool compare(const StringList& other);
    // 设置 currentString 的值
    void setCurrentString(const StringList& s) {
        currentString = s;
    }
    bool concatenate(const StringList& other);
    int getLength();
    StringList getSubstring(int start, int len);
    int findSubstring(const StringList& sub);
    bool replace(const StringList& oldSub, const StringList& newSub);
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    const StringList& getCurrentString() const;
    const std::vector<std::string>& getLog() const;
};

#endif    