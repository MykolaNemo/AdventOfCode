#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "md5.h"

#define PART2

std::string salt = "jlmsuwbz";

unsigned long long charactersInRow(const std::string& str, int repeatTimes)
{
    if(repeatTimes < 1 || repeatTimes >= static_cast<int>(str.size())) return std::string::npos;
    if(str.empty()) return std::string::npos;
    if(repeatTimes == 1) return 0;

    for(int i = 0; i <= static_cast<int>(str.size())-repeatTimes; ++i)
    {
        bool good = true;
        for(int j = 0; j < repeatTimes-1; ++j)
        {
            if(str[i+j] != str[i+j+1])
            {
                good = false;
            }
        }
        if(good)
        {
            return i;
        }
    }
    return std::string::npos;
}

unsigned long long charactersInRow(const std::string& str, int repeatTimes, char c)
{
    if(repeatTimes < 1 || repeatTimes >= static_cast<int>(str.size())) return std::string::npos;
    if(str.empty()) return std::string::npos;
    if(repeatTimes == 1) return 0;

    for(int i = 0; i <= static_cast<int>(str.size())-repeatTimes; ++i)
    {
        bool good = true;
        for(int j = 0; j < repeatTimes-1; ++j)
        {
            if((str[i+j] != str[i+j+1]) || (str[i+j] != c) || (str[i+j+1] != c))
            {
                good = false;
            }
        }
        if(good)
        {
            return i;
        }
    }
    return std::string::npos;
}

template<class T>
bool contains(const std::vector<T>& v, const T& item)
{
    return std::find(v.begin(), v.end(), item) != v.end();
}

int main()
{
    std::map<int, char> keysForVerification;
    int n = 0;
    std::vector<int> keys;
    while(true)
    {
        MD5 md5(salt + std::to_string(n));
        md5.finalize();
        std::string md5String = md5.hexdigest();
#ifdef PART2
        for(int i = 0; i < 2016; ++i)
        {
            MD5 md5(md5String);
            md5.finalize();
            md5String = md5.hexdigest();
        }
        std::cout<<n<<std::endl;
#endif
        auto pos = charactersInRow(md5String, 3);
        if(pos != std::string::npos)
        {
            keysForVerification[n] = md5String[pos];
        }
        for(const auto& p : keysForVerification)
        {
            int i = p.first;
            char c = p.second;
            if((n - i <= 1000) && (n != i) && !contains<int>(keys, i))
            {
                auto pos = charactersInRow(md5String, 5, c);
                if(pos != std::string::npos)
                {
                    keys.push_back(i);
                    if(keys.size() == 64)
                    {
                        std::cout<<"Answer: "<<i<<std::endl;
                        return 0;
                    }
                }
            }
        }
        n++;
    }
    return 0;
}
