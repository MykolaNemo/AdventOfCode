#include <iostream>
#include <vector>
#include "../../utils.h"

int input = 325489;

//#define PART1

#ifndef PART1
int sumAround(const std::vector<std::vector<int>>& spiral, const Point& coord)
{
    int sum = 0;
    sum += spiral[coord.y][coord.x+1];
    sum += spiral[coord.y-1][coord.x+1];
    sum += spiral[coord.y-1][coord.x];
    sum += spiral[coord.y-1][coord.x-1];
    sum += spiral[coord.y][coord.x-1];
    sum += spiral[coord.y+1][coord.x-1];
    sum += spiral[coord.y+1][coord.x];
    sum += spiral[coord.y+1][coord.x+1];
    return sum;
}

void checkResult(int result)
{
    if(result > input)
    {
        std::cout<<"[Part 2] Answer: "<<result<<std::endl;
        exit(0);
    }
}
#endif

int main()
{
#ifdef PART1
    /*
    37  36  35  34  33  32  31
    38  17  16  15  14  13  30
    39  18   5   4   3  12  29
    40  19   6   1   2  11  28
    41  20   7   8   9  10  27
    42  21  22  23  24  25  26
    43  44  45  46  47  48  49
    */
    int circleNumber = 1;
    {
        int num = 1;
        while(num < input)
        {
            circleNumber++;
            int sideSize = circleNumber*2-1;
            num += (sideSize*sideSize) - (sideSize-2)*(sideSize-2);
        }
    }

    int side = circleNumber*2-1;
    int sideSqr = side*side;

    //right side
    if(input == sideSqr || (input <= sideSqr - 3*(side-1)))
    {
        std::cout<<"Right"<<std::endl;
    }
    //top side
    if(input >= sideSqr - 3*(side-1) && input <= sideSqr - 2*(side-1))
    {
        std::cout<<"Top"<<std::endl;
    }
    //left side
    if(input >= sideSqr - 2*(side-1) && input <= sideSqr - 1*(side-1))
    {
        std::cout<<"Left"<<std::endl;
    }
    //bottom side
    if(input >= sideSqr - 1*(side-1) && input <= sideSqr)
    {
        std::cout<<"Bottom"<<std::endl;
    }

    //Input is at the bottom
    std::cout<<"[Part 1] Answer: "<<std::abs((sideSqr-input)-side/2) + circleNumber-1<<std::endl;
#else
/*
147  142  133  122   59
304    5    4    2   57
330   10    1    1   54
351   11   23   25   26
362  747  806--->   ...
*/
    int spiralSide = 11;// 11x11 is enough
    std::vector<std::vector<int>> spiral;
    spiral.resize(spiralSide);
    for(int i = 0; i < spiralSide; ++i)
    {
        spiral[i].resize(spiralSide, 0);
    }

    auto lambda = [&spiral](const Point& p){
        int value = sumAround(spiral, p);
        checkResult(value);
        spiral[p.y][p.x] = value;
    };

    Point coord(spiralSide/2, spiralSide/2);
    spiral[coord.y][coord.x] = 1;
    int sideSize = 1;
    while(true)
    {
        sideSize += 2;

        coord.x++;
        lambda(coord);

        for(int i = 0; i < sideSize-2; ++i)
        {
            coord.y--;
            lambda(coord);
        }
        for(int i = 0; i < sideSize-1; ++i)
        {
            coord.x--;
            lambda(coord);
        }
        for(int i = 0; i < sideSize-1; ++i)
        {
            coord.y++;
            lambda(coord);
        }
        for(int i = 0; i < sideSize-1; ++i)
        {
            coord.x++;
            lambda(coord);
        }
    }
#endif
    return 0;
}
