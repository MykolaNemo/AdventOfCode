#include <iostream>
#include "../../utils.h"

int main()
{
    auto directions = split(readFile("input.txt")[0], ",");//one line
    Point pos(0, 0);
    int max = 0;
    for(const auto& direction : directions)
    {
        if(direction == "n")
        {
            pos.y -= 2;
        }
        else if(direction == "nw")
        {
            pos.x--;
            pos.y--;
        }
        else if(direction == "ne")
        {
            pos.x++;
            pos.y--;
        }
        else if(direction == "s")
        {
            pos.y += 2;
        }
        else if(direction == "sw")
        {
            pos.x--;
            pos.y++;
        }
        else if(direction == "se")
        {
            pos.x++;
            pos.y++;
        }
        max = std::max((std::abs(pos.x) + std::abs(pos.y))/2, max);
    }
    std::cout<<"[Part 1] Answer: "<<(std::abs(pos.x) + std::abs(pos.y))/2<<std::endl;
    std::cout<<"[Part 2] Answer: "<<max<<std::endl;
    return 0;
}
