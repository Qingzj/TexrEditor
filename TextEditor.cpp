#include "TextEditor.h"
#include <sstream>
#include <string>
#include <iostream>

// 去除参数中的单引号
std::string TextEditor::stripQuotes(const std::string& s) {
    if (s.size() >= 2 && s.front() == '\'' && s.back() == '\'')
        return s.substr(1, s.size() - 2);
    return s;
}

// 赋值操作，设置当前字符串
bool TextEditor::assign(const std::string& s) {
    std::string cleaned = stripQuotes(s);
    currentString.buildFromString(cleaned);
    operationLog.push_back("Assign: '" + cleaned + "' [Success]");
    return true;
}

// 比较两个字符串是否相等
bool TextEditor::compare(const StringList& other) {
    std::cout << "currentString 内容: ";
    // 假设 StringList 类有打印自身内容方法，若没有可添加类似遍历打印逻辑
    currentString.print();
    std::cout << "other 内容: ";
    other.print();
    bool result = currentString.equals(other);
    // 改动 3：修正字符串拼接问题，避免 C2110 错误
    operationLog.push_back(std::string("Compare: ") + (result ? "EQUAL" : "UNEQUAL"));
    return result;
}

// 连接字符串到当前字符串
bool TextEditor::concatenate(const StringList& other) {
    currentString.concatenate(other);
    operationLog.push_back("Concatenate [Success]");
    return true;
}

// 获取当前字符串长度
int TextEditor::getLength() {
    // 改动 4：若涉及 size_t 转 int，使用 static_cast 确保安全转换，避免 C4267 警告
    int len = static_cast<int>(currentString.getLength());
    operationLog.push_back("GetLength: " + std::to_string(len));
    return len;
}

// 获取子串
StringList TextEditor::getSubstring(int start, int len) {
    StringList sub = currentString.getSubList(start, len);
    operationLog.push_back("Substring from " + std::to_string(start) + " length " + std::to_string(len));
    return sub;
}

// 查找子串在当前字符串中的位置
int TextEditor::findSubstring(const StringList& sub) {
    int pos = currentString.findSubList(sub);
    operationLog.push_back("FindSubstring: Position " + std::to_string(pos));
    return pos;
}

// 替换子串
bool TextEditor::replace(const StringList& oldSub, const StringList& newSub) {
    bool success = currentString.replaceSubList(oldSub, newSub);

    operationLog.push_back(std::string("Replace ") + (success ? "[Success]" : "[Failed]"));
    return success;
}

// 保存当前字符串到文件
bool TextEditor::saveToFile(const std::string& filename) const {
    bool success = currentString.saveToFile(filename);
    operationLog.push_back("Save to '" + filename + "' " + (success ? "[Success]" : "[Failed]"));
    return success;
}

// 从文件加载内容到当前字符串
bool TextEditor::loadFromFile(const std::string& filename) {
    bool success = currentString.loadFromFile(filename);
    operationLog.push_back("Load from '" + filename + "' " + (success ? "[Success]" : "[Failed]"));
    return success;
}

// 获取当前字符串
const StringList& TextEditor::getCurrentString() const {
    return currentString;
}

// 获取操作日志
const std::vector<std::string>& TextEditor::getLog() const {
    return operationLog;
}
//#include "TextEditor.h"
//#include <sstream>
//#include <string>
//#include <iostream>
//
//// 去除参数中的单引号
//std::string TextEditor::stripQuotes(const std::string& s) {
//    if (s.size() >= 2 && s.front() == '\'' && s.back() == '\'')
//        return s.substr(1, s.size() - 2);
//    return s;
//}
//
//// 赋值操作，设置当前字符串
//bool TextEditor::assign(const std::string& s) {
//    std::string cleaned = stripQuotes(s);
//    currentString.buildFromString(cleaned);
//    operationLog.push_back("Assign: '" + cleaned + "' [Success]");
//    return true;
//}
//
//// 比较两个字符串是否相等
//bool TextEditor::compare(const StringList& other) {
//    std::cout << "currentString 内容: ";
//    // 假设 StringList 类有打印自身内容方法，若没有可添加类似遍历打印逻辑
//    currentString.print();
//    std::cout << "other 内容: ";
//    other.print();
//    bool result = currentString.equals(other);
//    // 改动 3：修正字符串拼接问题，避免 C2110 错误
//    operationLog.push_back(std::string("Compare: ") + (result ? "EQUAL" : "UNEQUAL"));
//    return result;
//}
//
//// 连接字符串到当前字符串
//bool TextEditor::concatenate(const StringList& other) {
//    currentString.concatenate(other);
//    operationLog.push_back("Concatenate [Success]");
//    return true;
//}
//
//// 获取当前字符串长度
//int TextEditor::getLength() {
//    // 改动 4：若涉及 size_t 转 int，使用 static_cast 确保安全转换，避免 C4267 警告
//    int len = static_cast<int>(currentString.getLength());
//    operationLog.push_back("GetLength: " + std::to_string(len));
//    return len;
//}
//
//// 获取子串
//StringList TextEditor::getSubstring(int start, int len) {
//    StringList sub = currentString.getSubList(start, len);
//    operationLog.push_back("Substring from " + std::to_string(start) + " length " + std::to_string(len));
//    return sub;
//}
//
//// 查找子串在当前字符串中的位置
//int TextEditor::findSubstring(const StringList& sub) {
//    int pos = currentString.findSubList(sub);
//    operationLog.push_back("FindSubstring: Position " + std::to_string(pos));
//    return pos;
//}
//
//// 替换子串
//bool TextEditor::replace(const StringList& oldSub, const StringList& newSub) {
//    bool success = currentString.replaceSubList(oldSub, newSub);
//
//    operationLog.push_back(std::string("Replace ") + (success ? "[Success]" : "[Failed]"));
//    return success;
//}
//
//// 保存当前字符串到文件
//bool TextEditor::saveToFile(const std::string& filename) const {
//    bool success = currentString.saveToFile(filename);
//    operationLog.push_back("Save to '" + filename + "' " + (success ? "[Success]" : "[Failed]"));
//    return success;
//}
//
//// 从文件加载内容到当前字符串
//bool TextEditor::loadFromFile(const std::string& filename) {
//    bool success = currentString.loadFromFile(filename);
//    operationLog.push_back("Load from '" + filename + "' " + (success ? "[Success]" : "[Failed]"));
//    return success;
//}
//
//// 获取当前字符串
//const StringList& TextEditor::getCurrentString() const {
//    return currentString;
//}
//
//// 获取操作日志
//const std::vector<std::string>& TextEditor::getLog() const {
//    return operationLog;
//}