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
 
#ifndef __FILE_TOOLS_H
#define __FILE_TOOLS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <fstream>

bool mkdir_p(const std::string& path, mode_t mode) {
    if (path.empty()) {
        errno = EINVAL;
        return false;
    }

    // 如果路径已经存在且是一个目录，则无需创建
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0) {
        if (S_ISDIR(sb.st_mode)) {
            return true;
        } else {
            errno = ENOTDIR;
            return false;
        }
    }

    // 尝试创建父目录
    size_t pos = path.find_last_of('/');
    if (pos != std::string::npos) {
        std::string parent = path.substr(0, pos);
        if (!mkdir_p(parent, mode)) {
            return false;
        }
    }

    // 创建当前目录
    if (mkdir(path.c_str(), mode) == 0) {
        return true;
    } else {
        // 如果目录已经存在，则忽略错误
        if (errno == EEXIST) {
            struct stat sb;
            if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                return true;
            }
        }
        return false;
    }
}

#include <dirent.h>
#include <vector>
int 统计文件数(const std::string& path) {
    DIR* dir;
    struct dirent* entry;
    int count = 0;

    dir = opendir(path.c_str());
    if (dir == nullptr) {
        perror("无法打开目录");
        return -1;
    }

    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        std::string full_path = path + "/" + entry->d_name;

        struct stat sb;
        if (stat(full_path.c_str(), &sb) == 0) {
            if (S_ISREG(sb.st_mode)) {
                count++;
            }
        } else {
            perror("无法获取文件状态");
        }
    }

    closedir(dir);
    return count;
}

bool 创建文件(const std::string& path) {
    std::ofstream ofs(path);
    if (!ofs) {  // 失败
        return false;
    }
    ofs.close();
    return true;
}
bool 文件存在(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

#endif  // __FILE_TOOLS_H