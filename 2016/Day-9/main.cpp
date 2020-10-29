#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

int main()
{
    std::fstream infile("input.txt");
    std::string str;
    while(std::getline(infile, str)){}

    {
        std::string file = str;
        auto pos = file.find_first_of('(');
        int length = pos;
        while(!file.empty())
        {
            auto posStart = file.find_first_of('(');
            auto posEnd = file.find_first_of(')');
            if(posStart != std::string::npos && posEnd != std::string::npos)
            {
                std::string marker = file.substr(posStart + 1, posEnd - posStart - 1);
                auto posSeparator = marker.find_first_of('x');
                int numberOfSymbols = 0;
                if(posSeparator != std::string::npos)
                {
                    numberOfSymbols = std::stoi(marker.substr(0, posSeparator));
                    int numberOfRepeat = std::stoi(marker.substr(posSeparator+1, marker.size() - posSeparator - 1));
                    length += numberOfSymbols*numberOfRepeat;
                }
                file = file.substr(posEnd+numberOfSymbols+1, file.size() - posEnd  - numberOfSymbols - 1);
            }
            else
            {
                length += file.size();
                file.clear();
            }
        }
        std::cout<<"Answer 1: "<<length<<std::endl;
    }

    {
        std::function<long long(const std::string&)> lambda;
        lambda = [&lambda](const std::string& file)->long long{
            long long length = 0;
            auto markerStart = file.find_first_of('(');
            auto markerEnd = file.find_first_of(')');
            if(markerStart == std::string::npos || markerEnd == std::string::npos)
            {
                return file.size();
            }
            else
            {
                length = markerStart;
                std::string marker = file.substr(markerStart + 1, markerEnd - markerStart - 1);
                auto xPos = marker.find_first_of('x');
                long long numberOfSymbols = std::stoll(marker.substr(0, xPos));
                long long numberOfRepeat = std::stoll(marker.substr(xPos + 1, marker.size() - xPos - 1));
                std::string repeatedPart = file.substr(markerEnd + 1, numberOfSymbols);
                auto markerStart2 = repeatedPart.find_first_of('(');
                auto markerEnd2 = repeatedPart.find_first_of(')');
                if(markerStart2 != std::string::npos && markerEnd2 != std::string::npos)
                {
                    length += numberOfRepeat * lambda(repeatedPart);
                }
                else
                {
                    length += numberOfRepeat * numberOfSymbols;
                }
                length += lambda(file.substr(markerEnd+numberOfSymbols+1, file.size() - markerStart  - numberOfSymbols - 1));
            }
            return length;
        };

        std::string file = str;
        long long length = lambda(file);
        std::cout<<"Answer 2: "<<length<<std::endl;
    }
    return 0;
}
