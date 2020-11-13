#include <iostream>
#include "../../utils.h"

int main()
{
    std::string sequence = readFile("input.txt")[0];//one line
    int sum1 = 0;
    int sum2 = 0;
    int next1 = 1;
    int next2 = static_cast<int>(sequence.size())/2;
    for(int i = 0; i < static_cast<int>(sequence.size()); ++i)
    {
        int nextIndex = (i + next1) % sequence.size();
        if(sequence[i] == sequence[nextIndex])
        {
            sum1 += sequence[i] - '0';
        }
        nextIndex = (i + next2) % sequence.size();
        if(sequence[i] == sequence[nextIndex])
        {
            sum2 += sequence[i] - '0';
        }
    }
    std::cout<<"[Part 1] Answer: "<<sum1<<std::endl;
    std::cout<<"[Part 2] Answer: "<<sum2<<std::endl;
    return 0;
}
