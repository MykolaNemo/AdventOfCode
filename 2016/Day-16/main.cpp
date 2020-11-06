#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

//#define PART1

#ifdef PART1
int dataLength = 272;
#else
int dataLength = 35651584;
#endif
std::string input = "10111011111001111";

int main()
{
    auto generateData = [](std::string& data){
        std::string addData;
        for(int i = static_cast<int>(data.size())-1; i >= 0; --i)
        {
            if(data[i] == '1')
            {
                addData.push_back('0');
            }
            else if(data[i] == '0')
            {
                addData.push_back('1');
            }
        }
        data += "0" + addData;
    };
    while(static_cast<int>(input.size()) < dataLength)
    {
        generateData(input);
    }

    std::string checksum = input.substr(0,dataLength);
    while(checksum.size() % 2 == 0)
    {
        std::string newChecksum;
        for(int i = 0; i < static_cast<int>(checksum.size()); i+=2)
        {
            if(checksum[i] == checksum[i+1])
            {
                newChecksum.push_back('1');
            }
            else
            {
                newChecksum.push_back('0');
            }
        }
        checksum = newChecksum;
    }
    std::cout<<"Answer: "<<checksum<<std::endl;
    return 0;
}
