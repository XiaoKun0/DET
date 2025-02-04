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
 
#ifndef __STR_TOOLS_H
#define __STR_TOOLS_H
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>

std::string 提取字段(const std::string& response, const std::string& field) {
    std::regex pattern(R"(")" + field + R"(":\[([^\]]*)\])");
    std::smatch match;
    
    if (std::regex_search(response, match, pattern)) {
        return match[1].str();
    }
    else {
        return "";
    }
}

std::vector<std::string> 拆分(const std::string& str, const int Version = 2) {
    std::vector<std::string> elements;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // 空白
        item.erase(item.begin(), std::find_if(item.begin(), item.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        item.erase(std::find_if(item.rbegin(), item.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), item.end());
        // 双引号
        item.erase(std::remove(item.begin(), item.end(), '\"'), item.end());
        
        elements.push_back(item);
    }
    return elements;
}

std::string 提取视频Url(const std::string& response) {
    std::regex pattern(R"(\"qualityLabel\".*?\"url\":\"([^\"]*)\")", std::regex::icase);

    std::smatch matches;
    std::string url;

    std::stringstream ss(response);
    std::string line;
    while (std::getline(ss, line)) {
        if (std::regex_search(line, matches, pattern)) {
            if (matches.size() > 1) {
                url = matches[1].str();
                break; // 找到第一个后退出
            }
        }
    }

    return url;
}

#endif // __STR_TOOLS_H