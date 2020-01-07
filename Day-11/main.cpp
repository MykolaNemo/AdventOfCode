#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "../Computer.h"

using namespace std;

enum Direction
{
  Up = 0,
  Right,
  Down,
  Left
};

struct Paint
{
  int first = 0;
  int second = 0;
  int color = 0;
};

int main()
{
  std::ifstream infile("input.txt");
  std::vector<long long> program;

  std::string code;
  while(std::getline(infile, code, ','))
  {
    program.push_back(std::stol(code));
  }

  std::pair<int, int> currentCoordinates = {0,0};
  std::vector<Paint> paintedCoordinates;
  Direction dir = Up;
  auto turnLambda = [](Direction& dir, int turn){
    switch(dir)
    {
    case Up:
      if(turn == 0)
      {
        dir = Left;
      }
      else if(turn == 1)
      {
        dir = Right;
      }
      break;
    case Right:
      if(turn == 0)
      {
        dir = Up;
      }
      else if(turn == 1)
      {
        dir = Down;
      }
      break;
    case Down:
      if(turn == 0)
      {
        dir = Right;
      }
      else if(turn == 1)
      {
        dir = Left;
      }
      break;
    case Left:
      if(turn == 0)
      {
        dir = Down;
      }
      else if(turn == 1)
      {
        dir = Up;
      }
      break;
    }
  };
  auto moveLambda = [](Direction& dir, std::pair<int, int>& coord){
    switch(dir)
    {
    case Up:
      coord.second--;
      break;
    case Right:
      coord.first++;
      break;
    case Down:
      coord.second++;
      break;
    case Left:
      coord.first--;
      break;
    }
  };
  auto findPaint = [&paintedCoordinates](std::pair<int, int>& coord){
    return std::find_if(paintedCoordinates.begin(), paintedCoordinates.end(), [&coord](Paint& paint)->bool{
      return ((paint.first == coord.first) && (paint.second == coord.second));
    });
  };

  std::vector<long long> input = {1};
  std::vector<long long> output;
  Computer robot(0);
  robot.setWaitForInput(true);
  robot.setProgram(program);
  while(robot.state() != Computer::Halted)
  {
    if(input.empty())
    {
      auto paintIt = findPaint(currentCoordinates);
      if(paintIt == paintedCoordinates.end())
      {
        input.push_back(0);
      }
      else
      {
        input.push_back(paintIt->color);
      }
    }

    robot.start(input, output);

    if(output.size() == 2)
    {
      int color = output[0];
      int turn = output[1];
      output.clear();

      auto paintIt = findPaint(currentCoordinates);
      if(paintIt == paintedCoordinates.end())
      {
        Paint p;
        p.first = currentCoordinates.first;
        p.second = currentCoordinates.second;
        p.color = color;
        paintedCoordinates.push_back(p);
      }
      else
      {
        paintIt->color = color;
      }

      turnLambda(dir, turn);
      moveLambda(dir, currentCoordinates);
    }
    else
    {
      std::cout<<"Bad output!"<<std::endl;
    }
  }

  std::cout<<"Number: "<<paintedCoordinates.size()<<std::endl;

  int minX = paintedCoordinates.begin()->first;
  int minY = paintedCoordinates.begin()->second;
  int maxX = paintedCoordinates.begin()->first;
  int maxY = paintedCoordinates.begin()->second;
  for(Paint& p : paintedCoordinates)
  {
    if(p.first < minX)
    {
      minX = p.first;
    }
    if(p.first > maxX)
    {
      maxX = p.first;
    }
    if(p.second < minY)
    {
      minY = p.second;
    }
    if(p.second > maxY)
    {
      maxY = p.second;
    }
  }

  int hullWidth = std::abs(maxX - minX);
  int hullHeight = std::abs(maxY - minY);
  std::vector<std::string> hull;
  for(int y = 0; y <= hullHeight; ++y)
  {
    std::string s;
    for(int x = 0; x <= hullWidth; ++x)
    {
      s += ' ';
    }
    hull.push_back(s);
  }
  hull[0 - minY][0 - minX] = '#';

  for(Paint& p : paintedCoordinates)
  {
    if(p.color == 0)
    {
      hull[p.second - minY][p.first - minX] = ' ';
    }
    else
    {
      hull[p.second - minY][p.first - minX] = '#';
    }
  }

  for(int y = 0; y <= hullHeight; ++y)
  {
    for(int x = 0; x <= hullWidth; ++x)
    {
      std::cout<<hull[y][x];
    }
    std::cout<<std::endl;
  }

  return 0;
}
