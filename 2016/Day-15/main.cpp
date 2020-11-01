#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#define PART2

//Disc #1 has 17 positions; at time=0, it is at position 15.
//Disc #2 has 3 positions; at time=0, it is at position 2.
//Disc #3 has 19 positions; at time=0, it is at position 4.
//Disc #4 has 13 positions; at time=0, it is at position 2.
//Disc #5 has 7 positions; at time=0, it is at position 2.
//Disc #6 has 5 positions; at time=0, it is at position 0.
//Disc #7 has 11 positions; at time=0, it is at position 0.

using Disc = std::pair<int,int>;
std::vector<Disc> discs = {{17,15},{3,2},{19,4},{13,2},{7,2},{5,0}};

int main()
{
#ifdef PART2
    discs.push_back({11, 0});
#endif
    for(int i = 0; i < static_cast<int>(discs.size()); ++i)
    {
        discs[i].second += i+1;
    }
    int time = 1;
    while(true)
    {
        bool good = true;
        for(Disc& disc : discs)
        {
            if((disc.second + time) % disc.first != 0)
            {
                good = false;
                break;
            }
        }
        if(good)
        {
            std::cout<<"Answer: "<<time<<std::endl;
            return 0;
        }
        time++;
    }
    return 0;
}
