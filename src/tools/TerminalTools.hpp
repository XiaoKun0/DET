/*
 * Copyright (C) 2025  F_Error11
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
 
#ifndef __TERMINAL_TOOLS_H
#define __TERMINAL_TOOLS_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>

int 终端宽()
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return 48; // 返回48 (一般手机mt终端宽)
    return w.ws_col;
}

void 分割线(char delimiter = '-')
{
    int width = 终端宽();
    if (width < 1) {return;}  // 宽度异常

    char* divider = new (std::nothrow) char[width + 1];
    if (!divider) {return;}  // 内存分配失败

    memset(divider, delimiter, width);
    divider[width] = '\0';

    std::cout << divider << std::endl;

    delete[] divider;
}
void 终端中间绘制文本(const std::string str)
{
    int 光标位置 = ( 终端宽() - str.length() ) / 2;
    std::cout << "\033[" << 光标位置 << "G" << str << std::endl;
}



namespace Color {
    const string 无 = "\033[0m";
    const string 亮 = "\033[1m";
    const string 红 = "\033[31m";
    const string 绿 = "\033[32m";
    const string 黄 = "\033[33m";
    const string 蓝 = "\033[34m";
    const string 紫 = "\033[35m";
    const string 青 = "\033[36m";
    const string 橙 = "\033[38;5;208m";
    const string 灰 = "\033[90m";
}
namespace BackGround {
    const string 无 = "\033[0m";
    const string 亮 = "\033[1m";
    const string 红 = "\033[41m";
    const string 绿 = "\033[42m";
    const string 黄 = "\033[43m";
    const string 蓝 = "\033[44m";
    const string 紫 = "\033[45m";
    const string 青 = "\033[46m";
    const string 灰 = "\033[100m";
}

#include <termios.h>

std::string 限制字符长度输入(int maxChars) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // 获取当前终端设置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 关闭缓冲和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 应用新的终端设置

    std::string input;
    while (input.length() < maxChars) {
        char ch;
        ssize_t n = read(STDIN_FILENO, &ch, 1); // 非阻塞读取一个字符
        if (n == -1) {
            // 读取错误
            break;
        } else if (n == 0) {
            // 没有输入，继续等待
            continue;
        } else {
            if (ch == '\n' || ch == '\r') {
                break; // 按下回车键，结束输入
            }
            input += ch;
            std::cout << ch << std::flush;  // 使用flush实时刷新
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 恢复终端原始设置

    return input;
}

#endif  // __TERMINAL_TOOLS_H