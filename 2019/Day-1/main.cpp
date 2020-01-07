#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void part1(const std::vector<int>& moduleMasses)
{
  int totalFuel = 0;
  for(int mass : moduleMasses)
  {
    totalFuel += mass/3-2;
  }
  std::cout<<"[Part1] Fuel required:"<<totalFuel<<std::endl;
}

void part2(const std::vector<int>& moduleMasses)
{
  int totalFuel = 0;
  for(int mass : moduleMasses)
  {
    int fuel = 0;
    do
    {
      int f = mass/3-2;
      f = (f < 0) ? 0 : f;
      mass = f;
      fuel += f;
    }
    while(mass > 0);
    totalFuel += fuel;
  }
  std::cout<<"[Part2] Fuel required:"<<totalFuel<<std::endl;
}

int main()
{
  std::vector<int> moduleMasses;
  std::ifstream infile("input.txt");
  std::string massString;
  while (std::getline(infile, massString))
  {
    moduleMasses.push_back(std::stoi(massString));
  }

  part1(moduleMasses);
  part2(moduleMasses);
  return 0;
}