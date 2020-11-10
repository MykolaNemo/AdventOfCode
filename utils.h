#pragma once

#include <vector>
#include <set>
#include <string>
#include <algorithm>

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> list;
    size_t searchBegin = 0;
    size_t pos = str.find(delim);
    while(true)
    {
        list.push_back(str.substr(searchBegin, pos-searchBegin));

        size_t newPos = str.find(delim, pos+delim.size());
        if(newPos == std::string::npos)
        {
            list.push_back(str.substr(pos+delim.size(), str.size()-(pos+delim.size())));
            break;
        }
        searchBegin = pos+1;
        pos = newPos;
    }
    return list;
}

template<class T>
bool contains(const std::vector<T>& v, const T& item)
{
    return std::find(v.begin(), v.end(), item) != v.end();
}

template<class T>
bool contains(const std::set<T>& v, const T& item)
{
    return std::find(v.begin(), v.end(), item) != v.end();
}

template<class T>
auto erase(std::vector<T>& v, const T& item)
{
    return v.erase(std::remove(v.begin(), v.end(), item), v.end());
}