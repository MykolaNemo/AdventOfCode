#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "Computer.h"

using namespace std;

int main()
{
  std::ifstream infile("input.txt");
  std::vector<long long> program;

  std::string code;
  while(std::getline(infile, code, ','))
  {
    program.push_back(std::stol(code));
  }

  Computer arcade(0);
  arcade.setProgram(program);
  arcade.setWaitAfterOutput(true, 3);

  std::vector<long long> input;
  std::vector<long long> output;

  int score = -1;
  std::pair<int, int> ball;
  std::vector<std::pair<int, int>> blocks;
  std::vector<std::pair<int, int>> walls;
  std::vector<std::pair<int, int>> paddle;

  while(arcade.state() != Computer::Halted)
  {
    std::cout<<"start.."<<std::endl;
    arcade.start(input, output);

    if(output.size() == 3)
    {
      int x = output[0];
      int y = output[1];
      int id = output[2];
      output.clear();

      if(x == -1 && y == 0)
      {
        score = id;
      }
      else
      {
        switch(id)
        {
        case 0: // empty
        {
          std::pair<int, int> p = {x,y};
          auto it = std::find(blocks.begin(), blocks.end(), p);
          if(it != blocks.end())
          {
            blocks.erase(it);
          }

          it = std::find(paddle.begin(), paddle.end(), p);
          if(it != paddle.end())
          {
            paddle.erase(it);
          }
          if(ball.first == x && ball.second == y)
          {
            ball = {-1,-1};
          }
          break;
        }
        case 1: //wall
          walls.push_back({x,y});
          break;
        case 2: //block
          blocks.push_back({x,y});
          break;
        case 3: //paddle
          paddle.push_back({x,y});
          break;
        case 4: //ball
          ball.first = x;
          ball.second = y;
          break;
        }
      }
      std::cout<<"Ball: "<<ball.first<<","<<ball.second<<"\n";
      std::cout<<"Paddle: ";
      for(int i = 0; i < paddle.size(); ++i)
      {
        std::cout<<paddle[i].first<<","<<paddle[i].second<<"\n";
      }
      if(paddle.size() == 0)
      {
        std::cout<<"\n";
      }
      std::cout<<"score: "<<score<<"\n";

      if(paddle.size() > 0 && ball.first >= 0 && ball.second >= 0 && score >= 0)
      {
        int leftPaddleSide = paddle[0].first;
        int rightPaddleSide = paddle[0].first;
        for(int p = 0; p < paddle.size(); ++p)
        {
          if(paddle[p].first < leftPaddleSide)
          {
            leftPaddleSide = paddle[p].first;
          }
          if(paddle[p].first > rightPaddleSide)
          {
            rightPaddleSide = paddle[p].first;
          }
        }

        if(ball.first < leftPaddleSide)
        {
          std::cout<<"move left!\n";
          input = {-1};
        }
        if(ball.first > rightPaddleSide)
        {
          std::cout<<"move right!\n";
          input = {1};
        }
        if(ball.first >= leftPaddleSide && ball.first <= rightPaddleSide)
        {
          std::cout<<"stay!\n";
          input = {0};
        }
        continue;
      }
      input = {0};
    }
  }
  std::cout<<"\n========================\nFinal Score: "<<score<<std::endl;

//  int count = 0;
//  for(int i = 2; i < output.size(); i+=3)
//  {
////    std::cout<<output[i]<<std::endl;
//    if(output[i] == 2)
//    {
//      count++;
//    }
//  }
//  std::cout<<count<<std::endl;
  return 0;
}
