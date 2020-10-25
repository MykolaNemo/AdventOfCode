#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//#define PART1

using button = std::pair<int, int>;

#ifdef PART1
std::vector<std::vector<char>> keypad = {{'1','2','3'},
                                        {'4','5','6'},
                                        {'7','8','9'}};
button currentButton = {1,1};

void goToTheNext(const char& in)
{
    if(in == 'U')
    {
        if(currentButton.first > 0)
        {
            currentButton.first--;
        }
    }
    else if(in == 'R')
    {
        if(currentButton.second < 2)
        {
            currentButton.second++;
        }
    }
    else if(in == 'D')
    {
        if(currentButton.first < 2)
        {
            currentButton.first++;
        }
    }
    else if(in == 'L')
    {
        if(currentButton.second > 0)
        {
            currentButton.second--;
        }
    }
}
#else
std::vector<std::vector<char>> keypad = {{'0','0','1','0','0'},
                                        {'0','2','3','4','0'},
                                        {'5','6','7','8','9'},
                                        {'0','A','B','C','0'},
                                        {'0','0','D','0','0'}};
button currentButton = {2,0};
void goToTheNext(const char& in)
{
    if(in == 'U')
    {
        if((currentButton.first > 0) && (keypad[currentButton.first-1][currentButton.second] != '0'))
        {
            currentButton.first--;
        }
    }
    else if(in == 'R')
    {
        if((currentButton.second < 4) && (keypad[currentButton.first][currentButton.second+1] != '0'))
        {
            currentButton.second++;
        }
    }
    else if(in == 'D')
    {
        if((currentButton.first < 4) && (keypad[currentButton.first+1][currentButton.second] != '0'))
        {
            currentButton.first++;
        }
    }
    else if(in == 'L')
    {
        if((currentButton.second > 0) && (keypad[currentButton.first][currentButton.second-1] != '0'))
        {
            currentButton.second--;
        }
    }
}
#endif

int main()
{
    std::vector<std::string> instructions;
    {
        std::string instructionLine;
        std::fstream infile("input.txt");
        while(std::getline(infile, instructionLine))
        {
            instructions.push_back(instructionLine);
        }
    }
    std::string code;
    for(const auto& instructionLine : instructions)
    {
        for(const auto& in : instructionLine)
        {
            goToTheNext(in);
        }
        code += keypad[currentButton.first][currentButton.second];
    }
    std::cout<<"Answer: "<<code<<std::endl;
    return 0;
}