#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

//#define EXAMPLE
//#define PART2

enum class Type
{
    Wall,
    Space
};

struct Point
{
    Point(int _x, int _y):x(_x),y(_y){}
    int x = 0;
    int y = 0;
    int steps = 0;
};

Point operator+(const Point& rhs, const Point& lhs)
{
    return Point(rhs.x + lhs.x, rhs.y + lhs.y);
}
bool operator==(const Point& rhs, const Point& lhs)
{
    return (rhs.x == lhs.x) && (rhs.y == lhs.y);
}
bool operator!=(const Point& rhs, const Point& lhs)
{
    return !(rhs==lhs);
}
bool operator<(const Point& rhs, const Point& lhs)
{
    if(rhs.x != lhs.x)
    {
        return rhs.x < lhs.x;
    }
    if(rhs.y != lhs.y)
    {
        return rhs.y < lhs.y;
    }
    return false;
}

#ifdef EXAMPLE
int input = 10;
Point startPoint(1, 1);
Point finalPoint(7, 4);
#else
int input = 1350;
Point startPoint(1, 1);
Point finalPoint(31, 39);
#endif

bool isWall(int x, int y)
{
    int num = x*x + 3*x + 2*x*y + y + y*y + input;
    int bits = 0;
    while(num != 0)
    {
        if(num % 2 == 1) bits++;
        num /= 2;
    }
    return (bits % 2 != 0);
}

std::vector<Point> spaceAround(const Point& p)
{
    static const std::vector<Point> shifts = {        {0,-1},
                                              {-1, 0},       {1, 0},
                                                      {0, 1}      };
    std::vector<Point> points;
    for(const auto& shift : shifts)
    {
        if(p.x + shift.x < 0) continue;
        if(p.y + shift.y < 0) continue;
        if(!isWall(p.x + shift.x, p.y + shift.y))
        {
            Point point(p.x + shift.x, p.y + shift.y);
            point.steps = p.steps+1;
            points.push_back(point);
        }
    }
    return points;
}

template<class T>
bool contains(const std::vector<T>& v, const T& item)
{
    return std::find(v.begin(), v.end(), item) != v.end();
}

template<class T>
bool contains(const std::set<T>& v, const T& item)
{
    return std::find(v.begin(), v.end(), item) != v.end();
}

int main()
{
#ifndef PART2
    std::set<Point> visitedPoints;
    std::vector<Point> pointsToVisit = {startPoint};
    while(!pointsToVisit.empty())
    {
        Point nextPoint = pointsToVisit.front();
        pointsToVisit.erase(std::remove(pointsToVisit.begin(), pointsToVisit.end(), nextPoint), pointsToVisit.end());

        if(contains<Point>(visitedPoints, nextPoint)) continue;

        if(nextPoint != finalPoint)
        {
            visitedPoints.insert(nextPoint);
            for(const auto& p : spaceAround(nextPoint))
            {
                if(!contains<Point>(pointsToVisit, p))
                {
                    pointsToVisit.push_back(p);
                }
            }
        }
        else
        {
            std::cout<<"Answer: "<<nextPoint.steps<<std::endl;
            break;
        }
    }
#else
    std::set<Point> visitedPoints;
    std::vector<Point> pointsToVisit = {startPoint};
    while(!pointsToVisit.empty())
    {
        Point nextPoint = pointsToVisit.front();
        pointsToVisit.erase(std::remove(pointsToVisit.begin(), pointsToVisit.end(), nextPoint), pointsToVisit.end());

        if(contains<Point>(visitedPoints, nextPoint)) continue;

        visitedPoints.insert(nextPoint);
        for(const auto& p : spaceAround(nextPoint))
        {
            if(p.steps <= 50 && !contains<Point>(pointsToVisit, p))
            {
                pointsToVisit.push_back(p);
            }
        }
    }
    std::cout<<"Answer: "<<visitedPoints.size()<<std::endl;
#endif
    return 0;
}
