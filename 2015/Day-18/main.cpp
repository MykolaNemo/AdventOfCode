#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//#define PART1

using Table = std::vector<std::string>;

int width = 0;
int height = 0;

int numberOfNeightbours(int row, int col, Table lights)
{
  int number = 0;
  if((row > 0) && lights[row-1][col] == '#')
  {
    number++;
  }
  if((row > 0) && (col < width-1) && lights[row-1][col+1] == '#')
  {
    number++;
  }
  if((col < width-1) && lights[row][col+1] == '#')
  {
    number++;
  }
  if((row < height-1) && (col < width-1) && lights[row+1][col+1] == '#')
  {
    number++;
  }
  if((row < height-1) && lights[row+1][col] == '#')
  {
    number++;
  }
  if((row < height-1) && (col > 0) && lights[row+1][col-1] == '#')
  {
    number++;
  }
  if((col > 0) && lights[row][col-1] == '#')
  {
    number++;
  }
  if((row > 0) && (col > 0) && lights[row-1][col-1] == '#')
  {
    number++;
  }
  return number;
}

int main()
{
  std::fstream infile("input.txt");
  Table lights;
  std::string row;
  while(std::getline(infile, row))
  {
    lights.push_back(row);
  }

  height = lights.size();
  width = lights[0].size();

#ifndef PART1
  lights[0][0] = '#';
  lights[0][width-1] = '#';
  lights[height-1][0] = '#';
  lights[height-1][width-1] = '#';
#endif

  for(int i = 0; i < 100; ++i)
  {
    std::vector<std::pair<char*, char>> changes;
    for(int row = 0; row < height; ++row)
    {
      for(int col = 0; col < width; ++col)
      {
        if((row == 0 && col == 0) ||
           (row == 0 && col == width-1) ||
           (row == height-1 && col == 0) ||
           (row == height-1 && col == width-1))
        {
          continue;
        }
        int number = numberOfNeightbours(row, col, lights);
        if(lights[row][col] == '#' && number!=2 && number != 3)
        {
          changes.push_back({&lights[row][col], '.'});
        }
        if(lights[row][col] == '.' && number == 3)
        {
          changes.push_back({&lights[row][col], '#'});
        }
      }
    }
    for(auto& [light, state] : changes)
    {
      *light = state;
    }
    changes.clear();
  }

  int number = 0;
  for(auto& row : lights)
  {
    for(auto& c : row)
    {
      if(c == '#')
      {
        number++;
      }
    }
  }

#ifdef PART1
  std::cout<<"[Part 1] "<<number<<std::endl;
#else
  std::cout<<"[Part 2] "<<number<<std::endl;
#endif
  return 0;
}
