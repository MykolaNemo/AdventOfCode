#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string instructions;

    std::fstream infile("input.txt");
    std::getline(infile, instructions);

    int floor = 0;
    int firstBasementPos = -1;
    for(int i = 0; i < instructions.size(); ++i)
    {
        char in = instructions[i];
        switch(in)
        {
        case '(':
            floor++;
            break;
        case ')':
            floor--;
            if(floor == -1 && firstBasementPos == -1)
            {
                firstBasementPos = i + 1;
            }
            break;
        }
    }
    std::cout<<"[Part 1] Floor: "<<floor<<std::endl;
    std::cout<<"[Part 2] Answer: "<<firstBasementPos<<std::endl;
    return 0;
}
