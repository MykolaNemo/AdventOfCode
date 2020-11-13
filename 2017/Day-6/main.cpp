#include <iostream>
#include "../../utils.h"

int main()
{
    std::vector<int> memory = vectorSTOI(split(readFile("input.txt")[0], "\t"));
    std::vector<std::vector<int>> states = {memory};
    int steps = 0;
    while(true)
    {
        steps++;
        int index = -1;
        int value = -1;
        {
            for(int i = 0; i < static_cast<int>(memory.size()); ++i)
            {
                bool less = (value < memory[i]);
                value = less ? memory[i] : value;
                index = less ? i : index;
            }
        }
        memory[index] = 0;
        int pos = index+1;
        while(value > 0)
        {
            if(pos >= static_cast<int>(memory.size()))
            {
                pos = 0;
            }
            memory[pos]++;
            value--;
            pos++;
        }

        auto it = std::find(states.begin(), states.end(), memory);
        if(it == states.end())
        {
            states.push_back(memory);
        }
        else
        {
            std::cout<<"[Part 1] Answer: "<<steps<<std::endl;
            std::cout<<"[Part 2] Answer: "<<steps - (it - states.begin())<<std::endl;
            return 0;
        }
    }
    return 0;
}
