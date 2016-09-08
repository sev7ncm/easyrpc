#ifndef _FILEUTIL_H
#define _FILEUTIL_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>

#ifndef PATH_MAX
#define PATH_MAX        1024        // 默认最大路径长度
#endif

namespace easyrpc
{

class FileUtil
{
public:
    static std::string currentWorkPath()
    {
        char buf[PATH_MAX] = {'\0'};
        if (getcwd(buf, sizeof (buf) - 1) != nullptr)
        {
            return buf;
        }
        return "";
    }

    static bool setCurrentWorkPath(const std::string& path)
    {
        int ret = chdir(path.c_str());
        return ret == -1 ? false : true;
    }

    static std::string currentExePath()
    {
        char buf[PATH_MAX] = {'\0'};

        int ret = readlink("/proc/self/exe", buf, PATH_MAX);
        if (ret < 0 || ret >= PATH_MAX)
        {
            return "";
        }

        std::string path(buf);
        int pos = path.find_last_of("/");
        if (pos == -1)
        {
            return "";
        }

        return path.substr(0, pos);
    }

    static std::string currentExeName()
    {
        char buf[PATH_MAX] = {'\0'};

        int ret = readlink("/proc/self/exe", buf, PATH_MAX);
        if (ret < 0 || ret >= PATH_MAX)
        {
            return "";
        }

        std::string path(buf);
        int pos = path.find_last_of("/");
        if (pos == -1)
        {
            return "";
        }

        return path.substr(pos + 1, path.size() - 1);
    }

    static bool isExists(const std::string& path)
    {
        // F_OK 用于判断文件是否存在
        int ret = access(path.c_str(), F_OK);
        return ret == -1 ? false : true;
    }

    static bool mkdir(const std::string& path)
    {
        if (path.empty())
        {
            return false;
        }

        if (isExists(path))
        {
            return true;
        }

        std::string dirPath = path;
        if (dirPath[dirPath.size() - 1] != '/')
        {
            dirPath += '/';
        }

        for (std::size_t i = 0; i < dirPath.size(); ++i)
        {
            if (dirPath[i] == '/') 
            {
                if (i == 0)
                {
                    continue;
                }

                std::string tempPath = dirPath.substr(0, i);
                if (!isExists(tempPath))
                {
                    if (::mkdir(tempPath.c_str(), 0755) == -1)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    static bool remove(const std::string& path)
    {
        if (path.empty())
        {
            return false;
        }

        if (!isExists(path))
        {
            return false;
        }

        int ret = ::remove(path.c_str());
        return ret == -1 ? false : true;
    }
};

}
#endif
