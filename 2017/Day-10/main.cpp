#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "../../utils.h"

struct KnotHash
{
    int currentPos = 0;
    int skipSize = 0;
    std::vector<int> data;

    KnotHash()
    {
        for(int i = 0; i < 256; ++i)
        {
            data.push_back(static_cast<int>(i));
        }
    }

    void calculate(const std::vector<int>& lengthVector)
    {
        for(const auto length : lengthVector)
        {
            for(int i = 0; i < length/2; ++i)
            {
                auto start = (currentPos+i) % data.size();
                auto end = (currentPos+(length-1)-i) % data.size();
                auto t = data[start];
                data[start] = data[end];
                data[end] = t;
            }
            currentPos = (currentPos + length + skipSize) % data.size();
            skipSize++;
        }
    }
};

int main()
{
    auto inputString = readFile("input.txt")[0];//one line

    {
        // Part 1
        KnotHash hash;
        hash.calculate(vectorSTOI(split(inputString, ",")));
        std::cout<<"[Part 1] Answer: "<<hash.data[0]*hash.data[1]<<std::endl;
    }

    {
        // Part 2
        auto lengths = [&inputString]()->std::vector<int>{
            std::vector<int> v;
            for(const auto c : inputString)
            {
                v.push_back(c);
            }
            std::vector<int> additionalLengths = {17, 31, 73, 47, 23};
            v.insert(v.end(),
                     std::make_move_iterator(additionalLengths.begin()),
                     std::make_move_iterator(additionalLengths.end()));
            return v;
        }();

        KnotHash hash;
        for(int i = 0; i < 64; ++i)
        {
            hash.calculate(lengths);
        }

        std::vector<unsigned char> denseHash;
        for(int i = 0; i < 16; ++i)
        {
            unsigned char c = hash.data[16*i];
            for(int j = 16*i + 1; j < (16*i + 16); ++j)
            {
                c ^= (unsigned char)(hash.data[j]);
            }
            denseHash.push_back(c);
        }

        std::stringstream ss;
        for(const auto c : denseHash)
        {
            ss<<std::setfill('0')<<std::setw(2)<<std::hex<<(int)c;
        }
        std::cout<<"[Part 2] Answer: "<<ss.str()<<std::endl;
    }
    return 0;
}
