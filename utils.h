#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>

using StringVector = std::vector<std::string>;

template<class T>
void vectorPrint(const std::vector<T>& v)
{
    for(const auto& i : v)
    {
        std::cout<<i<<" ";
    }
}

std::vector<int> vectorSTOI(const StringVector& stringVector)
{
    std::vector<int> result;
    for(const auto& string : stringVector)
    {
        result.push_back(std::stoi(string));
    }
    return result;
}

StringVector readFile(const char* filename)
{
    StringVector content;
    std::fstream inputFile(filename);
    {
        std::string row;
        while(std::getline(inputFile, row))
        {
            content.push_back(row);
        }
    }
    return content;
}

struct Point
{
    Point(){}
    Point(int _x, int _y): x(std::move(_x)), y(std::move(_y)){}
    friend bool operator==(const Point& p1, const Point& p2)
    {
        return (p1.x == p2.x) && (p1.y == p2.y);
    }
    int x = 0;
    int y = 0;
};

StringVector split(const std::string& str, const std::string& delim)
{
    StringVector list;
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
bool contains(const std::vector<T>& container, const T& item, typename std::vector<T>::iterator& it)
{
    it = std::find(container.begin(), container.end(), item);
    return it != container.end();
}

template<class T>
bool contains(const std::set<T>& container, const T& item)
{
    return std::find(container.begin(), container.end(), item) != container.end();
}

template<class T>
bool contains(const std::set<T>& container, const T& item, typename std::set<T>::iterator& it)
{
    it = std::find(container.begin(), container.end(), item);
    return it != container.end();
}

template<class T>
auto erase(std::vector<T>& v, const T& item)
{
    return v.erase(std::remove(v.begin(), v.end(), item), v.end());
}