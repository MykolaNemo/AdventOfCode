#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

std::string input = "^.^^^.^..^....^^....^^^^.^^.^...^^.^.^^.^^.^^..^.^...^.^..^.^^.^..^.....^^^.^.^^^..^^...^^^...^...^.";
#ifdef PART1
int rowsNumber = 40;
#else
int rowsNumber = 400000;
#endif

bool isTrap(char c)
{
    return (c == '^');
}

bool isNextTrap(char l, char c, char r)
{
    return ((l == c && c != r) || (l != c && c == r));
}

int numberOfSafeTiles(const std::string& row)
{
    int n = 0;
    for(char c : row)
    {
        if(!isTrap(c)) n++;
    }
    return n;
}

int main()
{
    std::vector<std::string> floor;
    floor.push_back(input);

    int currentRow = 1;
    while(currentRow < rowsNumber)
    {
        std::string row;
        for(int i = 0; i < static_cast<int>(input.size()); ++i)
        {
            bool trap = false;
            if(i == 0)
            {
                trap = isNextTrap('.', floor[currentRow-1][i], floor[currentRow-1][i+1]);
            }
            else if(i == static_cast<int>(input.size())-1)
            {
                trap = isNextTrap(floor[currentRow-1][i-1], floor[currentRow-1][i], '.');
            }
            else
            {
                trap = isNextTrap(floor[currentRow-1][i-1], floor[currentRow-1][i], floor[currentRow-1][i+1]);
            }
            row.push_back(trap ? '^' : '.');
        }
        floor.push_back(row);
        currentRow++;
    }
    int safeTiles = 0;
    for(const auto& row : floor)
    {
        safeTiles += numberOfSafeTiles(row);
    }
    std::cout<<"Answer: "<<safeTiles<<std::endl;
    return 0;
}
