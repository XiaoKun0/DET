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
 
#include <iostream>
#include <string>

using namespace std;

#include "tools/TerminalTools.hpp"  // 终端
#include "tools/UrlTools.hpp"  // Url
#include "tools/StrTools.hpp"  // 字符
#include "tools/FileTools.hpp"  // 文件
#include "tools/OtherTools.hpp"  // 其它

// ANSI Color
namespace c = Color;  // text
namespace b = BackGround;  // background

// INIT FUNCTION
void 渲染主页();
void 处理单链接(const string& Input);
void 处理多链接(const string& Input);

// INIT GLOBAL VARIABLE
int 功能模块 = 1;
string Input;  // 输入


int main() {
    渲染主页();

    cin >> Input;
    渲染主页();
    switch (功能模块)
    {
        case 1:
            处理单链接(Input);
            break;
        case 2:
            处理多链接(Input);
            break;
        default:
            cout << c::红 << "ERROR MODELS " << 功能模块 << c::无 << endl;
            break;
    }
    return 0;
}

void 渲染主页()
{
    cout << "\033[2J\033[H";  // 清屏
    cout << c::黄;
    终端中间绘制文本("ERR DOWNLOAD TOOL");
    分割线();
    cout << c::无;
    
    cout << "\033[" << ( 终端宽() - 28 ) / 2 << "G";
    // Main Module (长度约 28)
    cout << " |";
    if (功能模块 == 1) cout << c::亮 << b::灰;
    cout << c::青 << " 单链接 " << c::无;
    cout << "|";
    if (功能模块 == 2) cout << c::亮 << b::灰;
    cout << c::青 << " 多链接 " << c::无;
    cout << "|";
    if (功能模块 == 3) cout << c::亮 << b::灰;
    cout << c::青 << " 设置 " << c::无;
    cout << "| " << endl;

    // 功能简介
    switch (功能模块)
    {
        case 1:
            cout << c::灰 << "粘贴包含链接的文本，当长度达到预设或点击回车时，自动进行解析" << endl;
            cout << c::黄 << "[+] 注: 按下回车立即结束输入" << c::灰 << endl;
            break;
        case 2:
            cout << c::红 << "暂不可用，我是懒逼不想完善了" << endl;
            cout << c::灰 << "输入一串包含多个链接的文本或文件路径，依次解析所有可用链接" << endl;
            break;
        case 3:
            cout << c::红 << "暂不可用，我是懒逼不想完善了" << endl;
            cout << c::灰 << "调整程序的设置，修改一下预设的变量" << endl;
            break;
    }
    分割线();
    cout << c::无 << flush;
}
void 处理单链接(const string& Input)
{
    vector<string> 链接集 = 解析链接(Input);
    // 快手链接普遍长度为29，此处使用substr提取前29个
    cout << c::黄 << "解析出链接: " << c::青 << 链接集[0].substr(0, 29) << c::无 << endl;

    for (const auto& 链接 : 链接集) {
        cout << c::黄 << "请求服务器: " << c::无 << endl;
        // 构建curl命令
        string 指令 = "curl -L -# " + 链接;
        try {  // 通过shell指令(curl)进行https请求
            string 指令返回 = 执行指令(指令);
            
            // 提取cdn部分
            std::string cdn字段 = 提取字段(指令返回, "cdn");
            std::vector<std::string> cdn列表 = 拆分(cdn字段);
            // 提取list部分
            std::string list字段 = 提取字段(指令返回, "list");
            std::vector<std::string> list列表 = 拆分(list字段);
            cout << cdn列表.size() << endl;

            cout << c::黄 << "链接指向类型为: " << c::亮 << flush;
            if (cdn列表.size() == 2)  // 图集
            {
                cout << "图片(集)" << c::无 << endl;
                string 保存路径 = "/sdcard/ErrorTools/Image/" + to_string(时间戳());
                if (!mkdir_p(保存路径, 0755))
                {
                    cerr << "创建目录" << 保存路径 << "失败\n错误: " << strerror(errno) << endl;
                    exit(1);
                }

                size_t list列表元素量 = list列表.size();
                for (size_t i = 0; i < cdn列表.size(); ++i) {
                    for (size_t ii = 0; ii < list列表元素量; ++ii) {
                        string 保存文件 = 保存路径 + "/" + to_string(ii+1) + ".jpg";
                        cout << c::蓝 << "[线路" << c::亮 << i+1 << c::无 << c::蓝 << "] "
                             << c::黄 << "下载中: [ " << c::亮 << ii+1 << c::无 << c::黄 << " / " <<  list列表元素量 << " ]" << c::无 << endl;
                        string 下载指令 = "curl -# -o " + 保存文件 + " " + cdn列表[i] + list列表[ii];
                        执行指令(下载指令);
                        cout << "\033[F\033[2K\033[F\033[2K";  // 向上清除两行
                    }
                    int 文件数 = 统计文件数(保存路径);
                    if (文件数 == list列表元素量)
                    {
                        cout << c::绿 << "共 " << c::亮 << 文件数 << c::无 << c::绿 << " 个图片" << "，全部下载成功\n"
                             << c::黄 << "已保存到路径: " << c::无 << 保存路径 << endl;
                        保存路径 += "/.nomedia";
                        创建文件(保存路径);
                        break;
                    } else {
                        cout << c::红 << "使用线路" << i+1 << "下载失败 (" << 文件数 << "/" << list列表元素量 << ")" << flush;
                    }
                }
            } else {
                cout << "视频" << c::无 << endl;
                string 保存路径 = "/sdcard/ErrorTools/Video";
                if (!mkdir_p(保存路径, 0755))
                {
                    cerr << "创建目录" << 保存路径 << "失败\n错误: " << strerror(errno) << endl;
                }
                string 保存文件 = 保存路径 + "/" + to_string(时间戳()) + ".mp4";
                string 视频Url = 提取视频Url(指令返回);
                if (视频Url.empty())
                    cout << "\033[31m解析失败\033[0m" << endl;
                
                string 下载指令 = "curl -# -o" + 保存文件 + " " + 视频Url;
                执行指令(下载指令);
                if (文件存在(保存文件))
                {
                    cout << c::绿 << "下载成功\n"
                         << c::黄 << "已保存为: " << c::无 << 保存文件 << endl;
                } else {
                    cout << c::红 << "下载失败" << c::无 << endl;
                    exit(1);
                }
            }
        }
        catch (const exception& e) {
            cerr << "ERROR: " << e.what() << endl;
        }
    }
}
void 处理多链接(const string& Input)
{
    // HAVE NOT NOW
}

