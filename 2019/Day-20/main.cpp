#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
//#include <functional>
#include <iomanip>
//#include <set>
//#include <list>
#include <string>

using namespace std;
struct Point;

using MapOfSymbols = std::vector<std::string>;
//using MapOfSteps = std::vector<std::vector<int>>;
using MapOfSteps = std::map<Point, int>;

//*
bool PART2 = true;
/*/
bool PART2 = false;//*/

struct Point
{
    Point(int _x, int _y, int _z): x(_x),y(_y),z(_z){}
//    Point(int _x, int _y): x(_x),y(_y){}

    int x = 0;
    int y = 0;
    int z = 0;
};

bool operator==(const Point& a, const Point& b)
{
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

bool operator<(const Point& a, const Point& b)
{
    if(a.z != b.z)
    {
        return (a.z < b.z);
    }
    if(a.y != b.y)
    {
        return (a.y < b.y);
    }
    return (a.x < b.x);
}

struct Portal
{
    bool goInside = false;
    Point outsidePoint = {-1,-1,0};
    Point insidePoint = {-1,-1,0};
    std::string name;
};

std::vector<Portal*> portals;

bool isLetter(char c)
{
    return (c >= 'A' && c <= 'Z');
}

int stepsAt(const Point& point, MapOfSteps& mapOfSteps)
{
    try
    {
        return mapOfSteps.at(point);
    }
    catch(...)
    {
        mapOfSteps[point] = -1;
        return -1;
    }
}

void findPortalsTop(const MapOfSymbols& map)
{
    int rowY = 1;
    for(int colX = 3; colX < map[rowY].size()-3; ++colX)
    {
        char symbol = map[rowY][colX];
        if(isLetter(symbol))
        {
            std::string name = {symbol, map[rowY+1][colX]};
            auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                return name == p->name;
            });
            if(portalIt == portals.end())
            {
                Portal* portal = new Portal();
                portal->name = name;
                portal->outsidePoint = {colX, rowY+2, 0};
                portals.push_back(portal);
            }
            else
            {
                (*portalIt)->outsidePoint = {colX, rowY+2, 0};
            }
        }
    }
}

void findPortalsBottom(const MapOfSymbols& map)
{
    int rowY = map.size()-3;
    for(int colX = 3; colX < map[rowY].size()-3; ++colX)
    {
        char symbol = map[rowY][colX];
        if(isLetter(symbol))
        {
            std::string name = {symbol, map[rowY+1][colX]};
            auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                return name == p->name;
            });
            if(portalIt == portals.end())
            {
                Portal* portal = new Portal();
                portal->name = name;
                portal->outsidePoint = {colX, rowY-1, 0};
                portals.push_back(portal);
            }
            else
            {
                (*portalIt)->outsidePoint = {colX, rowY-1, 0};
            }
        }
    }
}

void findPortalsLeft(const MapOfSymbols& map)
{
    int colX = 1;
    for(int rowY = 3; rowY < map.size()-3; ++rowY)
    {
        char symbol = map[rowY][colX];
        if(isLetter(symbol))
        {
            std::string name = {symbol, map[rowY][colX+1]};
            auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                return name == p->name;
            });
            if(portalIt == portals.end())
            {
                Portal* portal = new Portal();
                portal->name = name;
                portal->outsidePoint = {colX+2, rowY, 0};
                portals.push_back(portal);
            }
            else
            {
                (*portalIt)->outsidePoint = {colX+2, rowY, 0};
            }
        }
    }
}

void findPortalsRight(const MapOfSymbols& map)
{
    int colX = map[0].size()-3;
    for(int rowY = 3; rowY < map.size()-3; ++rowY)
    {
        char symbol = map[rowY][colX];
        if(isLetter(symbol))
        {
            std::string name = {symbol, map[rowY][colX+1]};
            auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                return name == p->name;
            });
            if(portalIt == portals.end())
            {
                Portal* portal = new Portal();
                portal->name = name;
                portal->outsidePoint = {colX-1, rowY, 0};
                portals.push_back(portal);
            }
            else
            {
                (*portalIt)->outsidePoint = {colX-1, rowY, 0};
            }
        }
    }
}

void findPortalsInside(const MapOfSymbols& map)
{
    for(int rowY = 3; rowY < map.size()-3; ++rowY)
    {
        for(int colX = 3; colX < map[rowY].size()-3; ++colX)
        {
            char symbol = map[rowY][colX];
            if(!isLetter(symbol)) continue;

            //top inside
            if(isLetter(map[rowY+1][colX]) && map[rowY-1][colX] == '.')
            {
                std::string name = {symbol, map[rowY+1][colX]};
                auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                        return name == p->name;
                });
                if(portalIt == portals.end())
                {
                    Portal* portal = new Portal();
                    portal->name = name;
                    portal->insidePoint = {colX, rowY-1, 0};
                    portals.push_back(portal);
                }
                else
                {
                    (*portalIt)->insidePoint = {colX, rowY-1, 0};
                }
            }
            //bottom inside
            else if(isLetter(map[rowY+1][colX]) && map[rowY+2][colX] == '.')
            {
                std::string name = {symbol, map[rowY+1][colX]};
                auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                        return name == p->name;
                });
                if(portalIt == portals.end())
                {
                    Portal* portal = new Portal();
                    portal->name = name;
                    portal->insidePoint = {colX, rowY+2, 0};
                    portals.push_back(portal);
                }
                else
                {
                    (*portalIt)->insidePoint = {colX, rowY+2, 0};
                }
            }
            //left inside
            else if(isLetter(map[rowY][colX+1]) && map[rowY][colX-1] == '.')
            {
                std::string name = {symbol, map[rowY][colX+1]};
                auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                        return name == p->name;
                });
                if(portalIt == portals.end())
                {
                    Portal* portal = new Portal();
                    portal->name = name;
                    portal->insidePoint = {colX-1, rowY, 0};
                    portals.push_back(portal);
                }
                else
                {
                    (*portalIt)->insidePoint = {colX-1, rowY, 0};
                }
            }
            //right inside
            else if(isLetter(map[rowY][colX+1]) && map[rowY][colX+2] == '.')
            {
                std::string name = {symbol, map[rowY][colX+1]};
                auto portalIt = std::find_if(portals.begin(), portals.end(), [&name](Portal* p)->bool{
                        return name == p->name;
                });
                if(portalIt == portals.end())
                {
                    Portal* portal = new Portal();
                    portal->name = name;
                    portal->insidePoint = {colX+2, rowY, 0};
                    portals.push_back(portal);
                }
                else
                {
                    (*portalIt)->insidePoint = {colX+2, rowY, 0};
                }
            }
        }
    }
}

void searchPortals(const MapOfSymbols& map)
{
    findPortalsTop(map);
    findPortalsBottom(map);
    findPortalsLeft(map);
    findPortalsRight(map);
    findPortalsInside(map);
}

Portal* findPortal(const Point& point)
{
    auto portalIt = std::find_if(portals.begin(), portals.end(), [&point](Portal* p)->bool{
        return (p->outsidePoint.x == point.x && p->outsidePoint.y == point.y) ||
               (p->insidePoint.x == point.x && p->insidePoint.y == point.y);
    });
    if(portalIt != portals.end())
    {
        return *portalIt;
    }
    return nullptr;
}

int findMinSteps(const Point& currentPoint, MapOfSteps& stepsMap)
{
    int x = currentPoint.x;
    int y = currentPoint.y;
    int z = currentPoint.z;
  int minSteps = -1;

  int steps = stepsAt({x, y-1, z}, stepsMap);
  if(steps >= 0 && (steps < minSteps || minSteps == -1))
  {
      minSteps = steps;
  }

  steps = stepsAt({x, y+1, z},stepsMap);
  if(steps >= 0 && (steps < minSteps || minSteps == -1))
  {
      minSteps = steps;
  }

  steps = stepsAt({x+1, y, z}, stepsMap);
  if(steps >= 0 && (steps < minSteps || minSteps == -1))
  {
      minSteps = steps;
  }

  steps = stepsAt({x-1, y, z}, stepsMap);
  if(steps >= 0 && (steps < minSteps || minSteps == -1))
  {
      minSteps = steps;
  }

  Portal* portal = findPortal(currentPoint);
  if(portal && portal->name != "AA" && portal->name != "ZZ")
  {
      if(portal->outsidePoint.x == currentPoint.x && portal->outsidePoint.y == currentPoint.y)
      {
          if(!PART2 || currentPoint.z > 0)
          {
              int zz = 0;
              if(PART2)
              {
                  zz = currentPoint.z - 1;
              }
              Point destinationPoint = {portal->insidePoint.x,
                                        portal->insidePoint.y,
                                        zz};
              int s = stepsAt(destinationPoint, stepsMap);
              if((s > 0) && (s < minSteps || minSteps == -1))
              {
                  minSteps = s;
              }
          }
      }
      else if(portal->insidePoint.x == currentPoint.x && portal->insidePoint.y == currentPoint.y)
      {
          int zz = 0;
          if(PART2)
          {
              zz = currentPoint.z + 1;
          }
          Point destinationPoint = {portal->outsidePoint.x,
                                    portal->outsidePoint.y,
                                    zz};
          int s = stepsAt(destinationPoint, stepsMap);
          if((s > 0) && (s < minSteps || minSteps == -1))
          {
              minSteps = s;
          }
      }
  }

  return minSteps;
}

std::vector<Point> findWays(Point currentPoint, const MapOfSymbols& map)
{
    int x = currentPoint.x;
    int y = currentPoint.y;
    int z = currentPoint.z;
  std::vector<Point> v;

  if(map[y-1][x] == '.')
  {
    v.push_back({x, y-1, z});
  }

  if(map[y][x+1] == '.')
  {
    v.push_back({x+1, y, z});
  }

  if(map[y+1][x] == '.')
  {
    v.push_back({x, y+1, z});
  }

  if(map[y][x-1] == '.')
  {
    v.push_back({x-1, y, z});
  }

  if(x == 43 && y == 26)
  {
      int a;
      a++;
  }
  Portal* portal = findPortal(currentPoint);
  if(portal && portal->name != "AA" && portal->name != "ZZ")
  {
      if(portal->outsidePoint.x == currentPoint.x && portal->outsidePoint.y == currentPoint.y)
      {
          if(!PART2 || currentPoint.z > 0)
          {
              int zz = 0;
              if(PART2)
              {
                  zz = currentPoint.z - 1;
              }
              Point destinationPoint = {portal->insidePoint.x,
                                        portal->insidePoint.y,
                                        zz};
              v.push_back(destinationPoint);
          }
      }
      else if (!portal->goInside && (portal->insidePoint.x == currentPoint.x) && (portal->insidePoint.y == currentPoint.y))
      {
          int zz = 0;
          if(PART2)
          {
              zz = currentPoint.z + 1;
          }
          Point destinationPoint = {portal->outsidePoint.x,
                                    portal->outsidePoint.y,
                                    zz};
          v.push_back(destinationPoint);
      }
      else if(portal->goInside)
      {
          std::cout<<"Not again! "<<portal->name<<std::endl;
      }
  }

  return v;
}

int findPath(const Point& startPoint, const Point& endPoint, const MapOfSymbols& map)
{
    MapOfSteps stepsMap;
    for(int y = 0; y < map.size(); ++y)
    {
        for(int x = 0; x < map[0].size(); ++x)
        {
            if(map[y][x] == '.' || map[y][x] == '#')
            {
                stepsMap[Point(x, y, 0)] = -1;
            }
            else
            {
                stepsMap[Point(x, y, 0)] = -2;
            }
        }
    }

    stepsMap[startPoint] = findMinSteps(startPoint, stepsMap) + 1;

    std::vector<Point> visited;
    std::vector<Point> toVisit = {{startPoint.x, startPoint.y, 0}};
    while(!toVisit.empty())
    {
        Point visitPoint = toVisit.front();
        for(const auto& neighbourWay : findWays(visitPoint, map))
        {
            const auto& itVisited = std::find(visited.begin(), visited.end(), neighbourWay);
            const auto& itToVisit = std::find(toVisit.begin(), toVisit.end(), neighbourWay);
            if(itVisited == visited.end() && itToVisit == toVisit.end())
            {
                toVisit.push_back(neighbourWay);

                if(stepsAt(neighbourWay, stepsMap) == -1)
                {
                    stepsMap[neighbourWay] = findMinSteps(neighbourWay, stepsMap) + 1;
                    if(neighbourWay == endPoint)
                    {
                        return stepsMap[endPoint];
                    }
                }
            }
        }

        visited.push_back(visitPoint);
        if(!toVisit.empty())
        {
            toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), toVisit.front()), toVisit.end());
        }
    }

    return stepsMap[endPoint];
}

int main()
{
    MapOfSymbols TheMap;
    {
      std::ifstream infile("input.txt");
      {
        std::string row;
        while(std::getline(infile, row))
        {
          TheMap.push_back(row);
        }
      }
    }
    searchPortals(TheMap);

    for(std::string& row : TheMap)
    {
        std::cout<<row<<std::endl;
    }

    for(auto& portal : portals)
    {
        std::cout<<portal->name<<" "<<portal->outsidePoint.x<<","<<portal->outsidePoint.y
                               <<" "<<portal->insidePoint.x<<","<<portal->insidePoint.y<<std::endl;
    }

    auto entranceIt = std::find_if(portals.begin(), portals.end(), [](Portal* p)->bool{
        return p->name == "AA";
    });
    auto exitIt = std::find_if(portals.begin(), portals.end(), [](Portal* p)->bool{
        return p->name == "ZZ";
    });

    int result = findPath((*entranceIt)->outsidePoint, (*exitIt)->outsidePoint, TheMap);
    std::cout<<"\nResult: "<<result<<std::endl;

    return 0;
}
