#include <iostream>
#include "../../utils.h"

//#define PART1

int main()
{
    std::vector<int> v = vectorSTOI(readFile("input.txt"));
    int pos = 0;
    int steps = 0;
    while(pos < static_cast<int>(v.size()))
    {
#ifdef PART1
        v[pos]++;
        pos += (v[pos]-1);
#else
        if(v[pos] >= 3)
        {
            v[pos]--;
            pos += (v[pos]+1);
        }
        else
        {
            v[pos]++;
            pos += (v[pos]-1);
        }
#endif
        steps++;
    }
    std::cout<<"[Part 1] Answer: "<<steps<<std::endl;
    return 0;
}
