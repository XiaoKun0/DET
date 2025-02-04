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
 
#ifndef __URL_TOOLS_H
#define __URL_TOOLS_H
#include <iostream>
#include <string>
#include <vector>
#include <regex>

std::vector<std::string> 解析链接(const std::string& text)
{
    std::vector<std::string> urls;
    std::regex url_regex(R"(https?://\S+)");
    
    auto words_begin = \
        std::sregex_iterator(text.begin(), text.end(), url_regex);
    auto words_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i; 
        urls.push_back(match.str());
    }
    return urls;
}

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string 执行指令(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() 失败!");
    }
    // 读取命令输出
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

#endif  //__URL_TOOLS_H