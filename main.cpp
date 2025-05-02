#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <string>
#include "TextEditor.h"
#include "StringList.h"
#include "StringNode.h"
#include <iomanip>
#include <windows.h>
#include <locale>
using namespace std;

// 打印帮助信息
void printHelp() {
    cout << u8"可用命令列表:\n"
        << u8"  A'str'    : 赋值 (示例: A 'Hello')\n"
        << u8"  E's1''s2': 判等 (示例: E 'abc' 'abcd')\n"
        <<  u8"  C'str'    : 连接 (示例: C ' World')\n"
        << u8"  L          : 获取当前长度\n"
        << u8"  S p l      : 获取子串 (示例: S 0 3)\n"
        << u8"  F'sub'    : 查找子串 (示例: F 'lo')\n"
        << u8"  R 'o' 'n'  : 替换子串 (示例: R 'l' 'LL')\n"
        << u8"  W file     : 保存到文件 (示例: W data.txt)\n"
        << u8"  H          : 显示帮助信息\n"
        << u8"  Q          : 退出程序\n";
}

// 解析命令行输入，将带引号的字符串参数正确解析出来
//vector<string> parseCommand(const string& line) {
//    vector<string> args;
//    bool inQuotes = false;
//    char quoteChar = '\0';
//    string currentArg;
//
//    for (char c : line) {
//        if ((c == '\'' || c == '\"') && !inQuotes) {
//            inQuotes = true;
//            quoteChar = c;
//        }
//        else if (c == quoteChar && inQuotes) {
//            inQuotes = false;
//            args.push_back(currentArg);
//            currentArg.clear();
//        }
//        else if (inQuotes) {
//            currentArg += c;
//        }
//        else if (isspace(c)) {
//            if (!currentArg.empty()) {
//                args.push_back(currentArg);
//                currentArg.clear();
//            }
//        }
//        else {
//            currentArg += c;
//        }
//    }
//
//    if (!currentArg.empty()) {
//        args.push_back(currentArg);
//    }
//
//    return args;
//}
//vector<string> parseCommand(const string& line) {
//    vector<string> args;
//    string currentArg;
//    bool inQuotes = false;
//    char quoteType = '\0';
//
//    for (char c : line) {
//        if ((c == '\'' || c == '"') && !inQuotes) {
//            inQuotes = true;
//            quoteType = c;
//        }
//        else if (c == quoteType && inQuotes) {
//            inQuotes = false;
//            quoteType = '\0';
//        }
//        else if (isspace(c) && !inQuotes) {
//            if (!currentArg.empty()) {
//                args.push_back(currentArg);
//                currentArg.clear();
//            }
//        }
//        else {
//            currentArg += c;
//        }
//    }
//
//    if (!currentArg.empty()) {
//        args.push_back(currentArg);
//    }
//
//    return args;
//}
vector<string> parseCommand(const string& line) {
    vector<string> args;
    string currentArg;
    bool inQuotes = false;
    char quoteType = '\0';

    for (size_t i = 0; i < line.size();) {
        char c = line[i];
        if ((c == '\'' || c == '"') && !inQuotes) { // 检测引号
            inQuotes = true;
            quoteType = c;
            i++;
        }
        else if (c == quoteType && inQuotes) {    // 引号结束
            inQuotes = false;
            if (!currentArg.empty()) {
                args.push_back(currentArg);
                currentArg.clear();
            }
            i++;
        }
        else if (inQuotes) {                     // 引号内按 UTF-8 字符处理
            size_t charLen = 1;
            if ((c & 0x80) != 0) {                 // 处理多字节字符（如中文）
                if ((c & 0xE0) == 0xC0) charLen = 2;
                else if ((c & 0xF0) == 0xE0) charLen = 3;
                else if ((c & 0xF8) == 0xF0) charLen = 4;
            }
            currentArg += line.substr(i, charLen);  // 按字符长度截取
            i += charLen;
        }
        else if (isspace(c)) {                   // 空格分隔参数
            if (!currentArg.empty()) {
                args.push_back(currentArg);
                currentArg.clear();
            }
            i++;
        }
        else {                                  // 普通字符
            currentArg += c;
            i++;
        }
    }

    if (!currentArg.empty()) {
        args.push_back(currentArg);
    }

    return args;
}
bool isValidNumber(const std::string& str) {//加强参数合法性检查，S
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}
int main() {

   /* SetConsoleCP(936);
    SetConsoleOutputCP(936);*/
    // 设置控制台编码和本地化
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, ".UTF8");
    std::ios::sync_with_stdio(false);  // 禁用流同步，避免编码冲突
    
    system("chcp 65001"); // 切换控制台编码为 UTF-8
    TextEditor editor;
    
    cout << u8"=== 文本编辑器 (输入H查看帮助) ===\n";
    // 执行测试用例
    //runTestCases(editor);

    string line;
    while (true) {
        cout << "\n> ";  // 输入提示符
        getline(cin, line);
        if (line.empty()) continue;

        vector<string> args = parseCommand(line);
        if (args.empty()) continue;

        char cmd = toupper(args[0][0]);  // 命令统一转大写
        switch (cmd) {
        //case 'A': {  // 赋值
        //    if (args.size() < 2) {
        //        cout << "错误: 需要参数 (示例: A 'Hello')" << endl;
        //        break;
        //    }
        //    //不知道这一步能不能省
        //    editor.assign(args[1]);
        //    cout << u8"操作成功！当前内容: ";
        //    editor.getCurrentString().print();
        //    cout <<endl;
        //    break;
        //}
        case 'A': {
            if (args.size() < 2) {
                cout << "错误: 需要参数 (示例: A 'Hello')" << endl;
                break;
            }
            // 调试：输出解析后的参数
            cout << "DEBUG - args[1]: [" << args[1] << "]" << endl;
            editor.assign(args[1]);
            cout << "操作成功！当前内容: ";
            editor.getCurrentString().print();
            cout << endl;
            break;
        }
        case 'E': {  // 判等√
            /*if (args.size() < 3) {
                cout << "错误: 需要两个参数 (示例: E 'abc' 'abcd')" << endl;
                break;
            }*/
            if (args.size() < 3 || args.empty()) {  // ADD: 参数有效性检查
                cout << "错误: 参数不足或解析失败" << endl;
                break;
            }
            StringList s1, s2;
            s1.buildFromString(args[1]);
            s2.buildFromString(args[2]);
            /*editor.setCurrentString(s1);
            bool result = editor.compare(s2);*/

            //editor.setCurrentString(s1);  // 依赖 StringList 的深拷贝
            //bool result = editor.compare(s2);//E 命令不应修改当前字符串
            bool result = s1.equals(s2);  // 直接比较两个参数
            cout << "结果: " << (result ? "EQUAL" : "UNEQUAL") << endl;
            break;
        }
        case 'C': {  // 连接
            if (args.size() < 2) {
                cout << "错误: 需要参数 (示例: C ' World')" << endl;
                break;
            }
            StringList other;
            other.buildFromString(args[1]);
            editor.concatenate(other);
            cout << "连接后内容: ";
            editor.getCurrentString().print();
            cout << endl;
            break;
        }

        case 'L': {  // 获取长度
            cout << "当前长度: " << editor.getLength() << endl;
            break;
        }

        case 'S': {
            if (args.size() < 3) {
                cout << "错误: 需要起始位置和长度 (示例: S 0 3)" << endl;
                break;
            }
            // FIX: 添加参数有效性检查（确保 args[1] 和 args[2] 是合法数字）
            if (!isValidNumber(args[1]) || !isValidNumber(args[2])) {
                cout << "错误: 参数必须为整数" << endl;
                break;
            }
            try {
                int start = stoi(args[1]);
                int len = stoi(args[2]);
                // MODIFY: 检查 start 和 len 的合法性（非负）
                if (start < 0 || len <= 0) {
                    cout << "错误: 起始位置或长度不合法" << endl;
                    break;
                }
                StringList sub = editor.getSubstring(start, len);
                cout << "子串内容: ";
                sub.print();
                cout << endl;
            }
            // FIX: 精确捕获转换异常
            catch (const std::invalid_argument&) {
                cout << "错误: 非法数字格式" << endl;
            }
            catch (const std::out_of_range&) {
                cout << "错误: 数字超出范围" << endl;
            }
            break;
        }

        case 'F': {
            vector<string> args = parseCommand(line);
            if (args.size() < 2 || args[1].empty()) {  // 检查子串非空
                cout << "错误: 需要有效子串参数 (示例: F 'lo')" << endl;
                break;
            }
            if (args.size() < 2) {
                cout << "错误: 需要参数 (示例: F 'lo')" << endl;
                break;
            }
            StringList sub;
            // FIX: 确保参数解析支持引号（例如 `F 'lo'` 应解析为 "lo"）
            sub.buildFromString(args[1]);
            int pos = editor.findSubstring(sub);
            if (pos == -1) {
                cout << "未找到子串" << endl;
            }
            else {
                cout << "首次出现位置: " << pos << endl;
            }
            break;
        }
        case 'R': {  // 替换
            vector<string> args = parseCommand(line);
            if (args.size() < 3) {
                cout << "错误: 需要旧子串和新子串 (示例: R 'l' 'LL')" << endl;
                break;
            }
            StringList oldSub, newSub;
            oldSub.buildFromString(args[1]);
            newSub.buildFromString(args[2]);

            // 检查旧子串是否为空
            if (oldSub.getLength() == 0) {
                cout << "错误: 旧子串不能为空" << endl;
                break;
            }

            // 执行替换
            bool success = editor.replace(oldSub, newSub);
            if (success) {
                cout << "替换成功 当前内容: ";
                editor.getCurrentString().print();
                cout << endl;
            }
            else {
                cout << "替换失败: 未找到旧子串" << endl;
            }
            break;
        }
        case 'W': {  // 保存
            if (args.size() < 2) {
                cout << "错误: 需要文件名 (示例: W data.txt)" << endl;
                break;
            }
            bool success = editor.saveToFile(args[1]);
            cout << "保存" << (success ? "成功" : "失败") << endl;
            break;
        }
        case 'H':  // 帮助
            printHelp();
            break;
        case 'Q':  // 退出
            cout << "=== 操作日志 ===\n";
            for (const auto& log : editor.getLog()) cout << log << endl;
            return 0;
        default:
            cout << "未知命令! 输入H查看帮助" << endl;
        }
    }
    return 0;
}
