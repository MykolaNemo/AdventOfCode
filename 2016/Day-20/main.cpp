#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <climits>
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

using IP =  long long;
IP lastIp = ULONG_MAX;
//IP lastIp = 9;
std::vector<std::pair<IP, IP>> ips;

double findIndex(const IP& value)
{
    double index = -1;
    if(value < ips[0].first)
    {
        index = -0.5;
    }

    if(ips[ips.size()-1].first <= value)
    {
        if(value <= ips[ips.size()-1].second)
        {
            index = static_cast<double>(ips.size()-1);
        }
        else
        {
            index = static_cast<double>(ips.size()) - 0.5;
        }
    }

    if(index == -1)
    {
        for(int i = 0; i < static_cast<int>(ips.size()); ++i)
        {
            if((ips[i].first <= value) && (value < ips[i+1].first))
            {
                if(value <= ips[i].second)
                {
                    index = i;
                    break;
                }
                else
                {
                    index = (double)i + 0.5;
                    break;
                }
            }
        }
    }
    return index;
}

int main()
{
    std::fstream infile("input.txt");
    {
        std::string instruction;
        while(std::getline(infile, instruction))
        {
            auto words = split(instruction, "-");
            std::pair<IP, IP> range = {std::stoull(words[0]), std::stoull(words[1])};
            if(ips.empty())
            {
                ips.push_back(range);
                continue;
            }

            double firstRangeIndex = findIndex(range.first);
            double lastRangeIndex =  findIndex(range.second);
            if(firstRangeIndex == lastRangeIndex)
            {
                if(ceil(firstRangeIndex) != firstRangeIndex)// in between
                {
                    ips.push_back(range);
                    std::sort(ips.begin(), ips.end());
                }
            }
            else
            {
                int firstIndex = ceil(firstRangeIndex);
                ips[firstIndex].first = std::min(ips[firstIndex].first, range.first);
                ips[firstIndex].second = std::max(ips[firstIndex].second, range.second);

                int lastIndex = floor(lastRangeIndex);
                if(lastIndex != firstIndex)
                {
                    for(int i = firstIndex+1; i <= lastIndex; ++i)
                    {
                        auto it = ips.begin();
                        std::advance(it, firstIndex+1);
                        ips[firstIndex].second = std::max(it->second, ips[firstIndex].second);
                        ips.erase(it);
                    }
                }
            }
        }
    }
    {
        IP ip = 0;
        for(auto& pair : ips)
        {
            if(ip < pair.first)
            {
                std::cout<<"[Part 1] Answer: "<<ip<<std::endl;
                break;
            }
            else if(ip <= pair.second)
            {
                ip = pair.second+1;
            }
        }
    }
    {
        IP n = ips[0].first;
        for(IP i = 1; i < static_cast<IP>(ips.size()); ++i)
        {
            if(ips[i].first - ips[i-1].second > 1)
            {
                n += (ips[i].first - ips[i-1].second - 1);
            }
        }
        if(lastIp - ips[ips.size()-1].second > 0)
        {
            n += (lastIp - ips[ips.size()-1].second);
        }
        std::cout<<"[Part 2] Answer: "<<n<<std::endl;
    }
    return 0;
}
