#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>

//#define PART1

int globalId = 0;

struct Jar
{
  Jar(int l):liters(l), id(++globalId){}
  Jar(const Jar& other)
  {
    this->liters = other.liters;
    this->id = other.id;
  }
  int liters = 0;
  int id = 0;
};

bool operator==(const Jar& j1, const Jar& j2)
{
  return j1.id == j2.id;
}

bool operator<(const Jar& j1, const Jar& j2)
{
  return j1.liters < j2.liters;
}

const int maxLiters = 150; //AoC input
std::vector<Jar> allJars;
std::vector<std::set<const Jar*>> combinations;
//int combinations;

void fillJars()
{
  std::vector<std::set<const Jar*>> queue;
  for(const Jar& jar : allJars)
  {
    queue.push_back({&jar});
  }

  while(!queue.empty())
  {
    auto filledJars = queue.back();
    queue.pop_back();

    int sumLitters = 0;
    for(const Jar* j : filledJars)
    {
      sumLitters += j->liters;
    }
    if(sumLitters == maxLiters)
    {
      combinations.push_back(filledJars);
      continue;
    }

    for(const Jar& jar : allJars)
    {
      auto jarIt = std::find(filledJars.begin(), filledJars.end(), &jar);
      if(jarIt != filledJars.end())
      {
        continue;
      }
      if(sumLitters + jar.liters > maxLiters)
      {
        continue;
      }

      auto newFilledJars = filledJars;
      newFilledJars.insert(&jar);

      auto filledJarsIt = std::find(queue.begin(), queue.end(), newFilledJars);
      if(filledJarsIt == queue.end())
      {
        queue.insert(queue.begin(), newFilledJars);
      }
    }
  }
}

int main()
{
  std::fstream infile("input.txt");
  std::string jarString;
  while(std::getline(infile, jarString))
  {
    allJars.push_back(Jar(std::stoi(jarString)));
  }
  std::sort(allJars.begin(), allJars.end());

  fillJars();

#ifdef PART1
  std::cout<<"[Part 1] Combinations number: "<<combinations.size()<<std::endl;
#else
  std::map<int, int> jarAmounts;
  for(auto &c : combinations)
  {
    jarAmounts[c.size()]++;
  }
  int32_t minAmount = INT32_MAX;
  for(auto [amount, numberOfCombinations] : jarAmounts)
  {
    minAmount = std::min(minAmount, amount);
  }
  std::cout<<"[Part 2] Combinations number: "<<jarAmounts[minAmount]<<std::endl;
#endif

  return 0;
}
