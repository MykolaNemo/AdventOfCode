#include <iostream>
#include <string>
#include <iterator>
#include "../../utils.h"

int main()
{
    auto moves = split(readFile("input.txt")[0], ",");// one line

    int repeatNumber = 0;
    std::vector<std::string> allPositions;
    std::string programs = "abcdefghijklmnop";
    for(int i = 0; i < 1000000000; ++i)
    {
        for(const std::string& move : moves)
        {
            if(move[0] == 's')
            {
                int n = std::stoi(move.substr(1, move.size()-1));
                programs = programs.substr(programs.size() - n, n) + programs.substr(0, programs.size() - n);
            }
            else if(move[0] == 'x')
            {
                auto positions = split(move.substr(1, move.size()-1), "/");
                int pos1 = std::stoi(positions[0]);
                int pos2 = std::stoi(positions[1]);
                char t = programs[pos1];
                programs[pos1] = programs[pos2];
                programs[pos2] = t;
            }
            else if(move[0] == 'p')
            {
                char c1 = move[1];
                char c2 = move[3];
                auto pos1 = std::find(programs.begin(), programs.end(), c1) - programs.begin();
                auto pos2 = std::find(programs.begin(), programs.end(), c2) - programs.begin();
                programs[pos1] = c2;
                programs[pos2] = c1;
            }
        }
        if(!contains<std::string>(allPositions, programs))
        {
            allPositions.push_back(programs);
        }
        else
        {
            repeatNumber = i;
            break;
        }
        if(i == 0)
        {
            std::cout<<"[Part 1] Answer: "<<programs<<std::endl;
        }
    }
    std::cout<<"[Part 2] Answer: "<<allPositions[(1000000000 % repeatNumber)-1]<<std::endl;
    return 0;
}
