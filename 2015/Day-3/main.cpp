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

bool operator==(const Point& a, const Point& b)
{
    return (a.x==b.x) && (a.y==b.y);
}

Point nextLocation(const Point& location, char dir)
{
  Point nextLocation = location;
  switch (dir) {
  case '>':
      nextLocation.x++;
      break;
  case '<':
      nextLocation.x--;
      break;
  case '^':
      nextLocation.y++;
      break;
  case 'v':
      nextLocation.y--;
      break;
  }
  return nextLocation;
}

void part1(const std::string& directions)
{
  std::vector<Point> houses;
  Point santaLocation = Point(0,0);
  houses.push_back(santaLocation);
  for(int i = 0; i < directions.size(); ++i)
  {
    char dir = directions[i];
    santaLocation = nextLocation(santaLocation, dir);
    if(std::find(houses.begin(), houses.end(), santaLocation) == houses.end())
    {
      houses.push_back(santaLocation);
    }
  }
  std::cout<<"[Part 1] Houses with presents: "<<houses.size()<<std::endl;
}

void part2(const std::string& directions)
{
  std::vector<Point> houses;

  Point robotLocation = Point(0,0);
  Point santaLocation = Point(0,0);
  houses.push_back(santaLocation);
  for(int i = 0; i < directions.size(); ++i)
  {
    char dir = directions[i];
    if(i % 2 == 0)
    {
      santaLocation = nextLocation(santaLocation, dir);
      if(std::find(houses.begin(), houses.end(), santaLocation) == houses.end())
      {
        houses.push_back(santaLocation);
      }
    }
    else
    {
      robotLocation = nextLocation(robotLocation, dir);
      if(std::find(houses.begin(), houses.end(), robotLocation) == houses.end())
      {
        houses.push_back(robotLocation);
      }
    }
  }
  std::cout<<"[Part 2] Houses with presents: "<<houses.size()<<std::endl;
}

int main()
{
    std::string directions;

    std::fstream infile("input.txt");
    std::getline(infile, directions);

    part1(directions);
    part2(directions);

    return 0;
}
