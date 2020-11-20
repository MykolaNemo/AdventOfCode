#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitset>
#include "../../utils.h"

std::string input = "ffayrhll";
//std::string input = "flqrgnkx";//test

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

std::string calculateHash(const std::string& inputString)
{
    const auto lengths = [&inputString]()->std::vector<int>{
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
    return ss.str();
}

int main()
{
    std::vector<std::string> hashes;
    for(int i = 0; i < 128; ++i)
    {
        hashes.push_back(calculateHash(input + "-" + std::to_string(i)));
    }

    std::vector<std::string> binaryField;
    {
        int used = 0;
        for(const auto& hash : hashes)
        {
            std::string row;
            for(auto c : hash)
            {
                std::stringstream ss;
                unsigned int n;

                ss << std::hex << c;
                ss >> n;
                std::bitset<4> binary(n);
                row += binary.to_string();
                for(auto c : binary.to_string())
                {
                    used += c-'0';
                }
            }
            binaryField.push_back(row);
        }
        std::cout<<"[Part 1] Answer: "<<used<<std::endl;
    }

    {
        std::map<int, std::map<int, bool>> visited;
        int groupsCount = 0;
        while(true)
        {
            std::vector<Point> toVisit;
            for(int y = 0; y < 128; ++y)
            {
                for(int x = 0; x < 128; ++x)
                {
                    if(!visited[x][y] && (binaryField[y][x] == '1'))
                    {
                        toVisit.push_back(Point(x,y));
                        break;
                    }
                }
                if(!toVisit.empty()) break;
            }
            if(toVisit.empty()) break;

            while(!toVisit.empty())
            {
                Point currentPoint = toVisit.front();
                erase<Point>(toVisit, currentPoint);
                visited[currentPoint.x][currentPoint.y] = true;

                const std::vector<Point> shifts = {Point(1,0),Point(-1,0),Point(0,-1),Point(0,1)};
                for(const auto& shift : shifts)
                {
                    const Point neighbour = currentPoint + shift;
                    if(neighbour.x >= 0 && neighbour.x < 128 && neighbour.y >= 0 && neighbour.y < 128)
                    {
                        if(!visited[neighbour.x][neighbour.y] && (binaryField[neighbour.y][neighbour.x] == '1'))
                        {
                            toVisit.push_back(neighbour);
                        }
                    }
                }
            }
            groupsCount++;
        }
        std::cout<<"[Part 2] Answer: "<<groupsCount<<std::endl;
    }
    return 0;
}
