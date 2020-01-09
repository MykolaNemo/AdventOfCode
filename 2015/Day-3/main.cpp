#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

struct Point
{
    Point(){}
    Point(int _x, int _y): x(_x), y(_y){}
    int x = 0;
    int y = 0;
};

bool operator<(const Point& a, const Point& b)
{
    return (a.x+a.y < b.x+b.y);
}

bool operator==(const Point& a, const Point& b)
{
    return (a.x==b.x) && (a.y==b.y);
}

int main()
{
    std::string directions;

    std::fstream infile("input.txt");
    std::getline(infile, directions);

    std::vector<Point> houses;

    Point currentPoint = Point(0,0);
    houses.push_back(currentPoint);
    for(char& dir : directions)
    {
        switch (dir) {
        case '>':
            currentPoint.x++;
            break;
        case '<':
            currentPoint.x--;
            break;
        case '^':
            currentPoint.y++;
            break;
        case 'v':
            currentPoint.y--;
            break;
        }
        if(std::find(houses.begin(), houses.end(), currentPoint) == houses.end())
        {
            houses.push_back(currentPoint);
        }
    }
    std::cout<<"[Part 1] Houses with presents: "<<houses.size()<<std::endl;
    return 0;
}
