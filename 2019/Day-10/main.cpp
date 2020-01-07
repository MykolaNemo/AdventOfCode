#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <math.h>

namespace{
struct Asteroid
{
  Asteroid(){}
  Asteroid(int _x, int _y):x(_x), y(_y){}
  int x = 0;
  int y = 0;
  double distance = 0.0;
  double angle = 0.0;
};

bool operator ==(const Asteroid& first, const Asteroid& second)
{
  return (first.x == second.x) && (first.y == second.y);
}

std::vector<std::string> field;
}

bool inFieldSpace(int x, int y)
{
  if(field.empty())
  {
    return false;
  }
  return (((x < field[0].length()) && (x >= 0)) && ((y < field.size()) && (y >= 0)));
}

std::pair<int, int> findStepSize(const Asteroid& from, const Asteroid& to)
{
  int xDistance = to.x - from.x;
  int yDistance = to.y - from.y;
  if(xDistance == 0 && yDistance == 0)
  {
    return {0,0};
  }

  std::pair<int, int> result;

  if(yDistance == 0)
  {
    result.first = (xDistance > 0) ? 1 : -1;
    result.second = 0;
  }
  else if(xDistance == 0)
  {
    result.first = 0;
    result.second = (yDistance > 0) ? 1 : -1;
  }

  {
    int minDistance = std::min(abs(xDistance), abs(yDistance));
    for(int divisor = minDistance; divisor >= 1; --divisor)
    {
      if(xDistance % divisor != 0) continue;
      if(yDistance % divisor != 0) continue;

      result.first = xDistance / divisor;
      result.second = yDistance / divisor;
      break;
    }
  }

  return result;
}

int main()
{
  std::ifstream infile("input.txt");

  std::string row;
  while(std::getline(infile, row))
  {
    field.push_back(row);
  }

  std::vector<Asteroid> asteroids;
  for(int row = 0; row < field.size(); ++row)
  {
    std::string rowString = field[row];
    std::cout<<rowString<<"\n";
    for(int col = 0; col < rowString.length(); ++col)
    {
      if(rowString[col] == '#')
      {
        asteroids.push_back(Asteroid(col, row));
      }
    }
  }
  std::cout.flush();

  int laserX = 0;
  int laserY = 0;
  {
    int maxCount = 0;
    for(const auto& lookAsteroid : asteroids)
    {
      int count = 0;
      std::vector<std::pair<int,int>> triedSteps;
      for(const auto& visibleAsteroid : asteroids)
      {
        if(lookAsteroid == visibleAsteroid) continue;

        auto stepSize = findStepSize(lookAsteroid, visibleAsteroid);
        if(stepSize.first == 0 && stepSize.second == 0) continue;

        auto stepsIt = std::find(triedSteps.begin(), triedSteps.end(), stepSize);
        if(stepsIt != triedSteps.end()) continue;
        triedSteps.push_back(stepSize);

        int x = lookAsteroid.x;
        int y = lookAsteroid.y;
        while(inFieldSpace(x, y))
        {
          if(x != lookAsteroid.x || y != lookAsteroid.y)
          {
            if(field[y][x] == '#')
            {
              count++;
              break;
            }
          }
          y+=stepSize.second;
          x+=stepSize.first;
        }
      }
      if(count > maxCount)
      {
        maxCount = count;
        laserX = lookAsteroid.x;
        laserY = lookAsteroid.y;
      }
    }
    std::cout<<"Best: "<<laserX<<", "<<laserY<<std::endl;
    std::cout<<"Number: "<<maxCount<<std::endl;
  }


  /**************************************************************************************************/

  asteroids.erase(std::find(asteroids.begin(), asteroids.end(),Asteroid(laserX,laserY)));

  for(auto &a : asteroids)
  {
    int x = a.x - laserX;
    int y = laserY - a.y;
    double katet = y;
    double hypotenusa = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    double cosinus = katet/hypotenusa;
    double angle = std::acos(cosinus);
    a.angle = (x < 0) ? (2*M_PI - angle) : angle;
    a.distance = hypotenusa;
  }

  std::sort(asteroids.begin(), asteroids.end(), [](Asteroid& a1, Asteroid& a2)->bool{
    if(std::abs(a1.angle - a2.angle) < 0.000001)
    {
      return a1.distance < a1.distance;
    }
    return a1.angle < a2.angle;
  });

  {
    int hitCount = 0;
    std::vector<std::pair<int,int>> triedSteps;
    auto targetAsteroidIt = asteroids.begin();
    while(asteroids.size() > 0)
    {
      if(targetAsteroidIt == asteroids.end())
      {
        targetAsteroidIt = asteroids.begin();
        triedSteps.clear();
      }

      auto steps = findStepSize(Asteroid(laserX, laserY), *targetAsteroidIt);
      if (steps.first == 0 && steps.second == 0) continue;

      auto stepsIt = std::find(triedSteps.begin(), triedSteps.end(), steps);
      if(stepsIt == triedSteps.end())
      {
        triedSteps.push_back(steps);
      }
      else
      {
        targetAsteroidIt++;
        continue;
      }

      {
        auto hitTheAsteroid = [&](int x, int y)->bool{
          if(x != laserX || y != laserY)
          {
            if(field[y][x] == '#')
            {
              hitCount++;
              field[y][x] = '.';
              targetAsteroidIt = asteroids.erase(targetAsteroidIt);
              return true;
            }
          }
          return false;
        };

        for(int x = laserX, y = laserY; inFieldSpace(x,y); x += steps.first, y += steps.second)
        {
          bool hit = hitTheAsteroid(x, y);
          if(hit && hitCount == 200)
          {
            std::cout<<"Asteroid 200: "<<x<<", "<<y<<std::endl;
            std::cout<<"Result:"<<x*100 + y<<std::endl;
            return 0;
          }
          if(hit) break;
        }
      }
    }
  }
  return 0;
}
