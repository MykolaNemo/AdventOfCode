#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <limits.h>

using namespace std;

unsigned long long timeStep = 0;
std::vector<bool> patternFound = {false,false,false};
std::vector<unsigned long long> pattern = {0ull,0ull,0ull};
struct Moon
{
  std::vector<int> velocity = {0,0,0};
  std::vector<int> position = {0,0,0};
  std::map<std::vector<int>, std::map<long long, int>> previousPositions;//positions, indexes
  std::map<std::vector<int>, std::map<long long, int>> previousVelocities;//velocities, indexes
};

void updateVelocity(std::vector<Moon>& moons)
{
  for(int i = 0; i < moons.size(); ++i)
  {
    Moon& moon1 = moons[i];
    for(int j = i+1; j < moons.size(); ++j)
    {
      Moon& moon2 = moons[j];
      for(int index = 0; index <= 2; ++index)
      {
        if(moon1.position[index] > moon2.position[index])
        {
          moon1.velocity[index]--;
          moon2.velocity[index]++;
        }
        else if(moon1.position[index] < moon2.position[index])
        {
          moon1.velocity[index]++;
          moon2.velocity[index]--;
        }
      }
    }
  }
}

void updatePosition(std::vector<Moon>& moons)
{
  for(int i = 0; i < moons.size(); ++i)
  {
    for(int index = 0; index <= 2; ++index)
    {
      moons[i].position[index] += moons[i].velocity[index];
    }
  }
}

int calculateEnergy(std::vector<Moon>& moons)
{
  int energy = 0;
  for(int i = 0; i < moons.size(); ++i)
  {
    int pot = std::abs(moons[i].position[0]) + std::abs(moons[i].position[1]) + std::abs(moons[i].position[2]);
    int kin = std::abs(moons[i].velocity[0]) + std::abs(moons[i].velocity[1]) + std::abs(moons[i].velocity[2]);
    energy+= pot*kin;
    std::cout<<"------------------"<<std::endl;
    std::cout<<"pot: "<<pot<<","<<"kin: "<<kin<<std::endl;
    std::cout<<"------------------"<<std::endl;
  }
  return energy;
}

int main()
{

  std::vector<Moon> moons;
  std::vector<std::vector<int>> initPos = {{14,2,8},{7,4,10},{1,17,16},{-4,-1,1}};
//  std::vector<std::vector<int>> initPos = {{-1,0,2},{2,-10,-7},{4,-8,8},{3,5,-1}};
//  std::vector<std::vector<int>> initPos = {{-8,-10,0},{5,5,10},{2,-7,3},{9,-8,-3}};

  Moon moon1;
  moon1.position = initPos[0];

  Moon moon2;
  moon2.position = initPos[1];

  Moon moon3;
  moon3.position = initPos[2];

  Moon moon4;
  moon4.position = initPos[3];

  moons.push_back(moon1);
  moons.push_back(moon2);
  moons.push_back(moon3);
  moons.push_back(moon4);

  while(true)
  {
    updateVelocity(moons);
    updatePosition(moons);
//    for(int i = 0; i < moons.size(); ++i)
//    {
//      std::cout<<moons[i].position[0]<<", "<<moons[i].position[1]<<", "<<moons[i].position[2]<<std::endl;
//    }
//    for(int i = 0; i < moons.size(); ++i)
//    {
//      std::cout<<moons[i].velocity[0]<<", "<<moons[i].velocity[1]<<", "<<moons[i].velocity[2]<<std::endl;
//    }
//    std::cout<<"============================="<<std::endl;
    timeStep++;
//        std::cout<<timeStep<<std::endl;

    std::vector<bool> found = {true, true, true};
    for(int i = 0; i < moons.size(); ++i)
    {
      Moon& moon = moons[i];
      if(!patternFound[0])
      {
        if(!(moon.position[0] == initPos[i][0] && moon.velocity[0] == 0))
        {
          found[0] = false;
        }
      }
      if(!patternFound[1])
      {
        if(!(moon.position[1] == initPos[i][1] && moon.velocity[1] == 0))
        {
          found[1] = false;
        }
      }
      if(!patternFound[2])
      {
        if(!(moon.position[2] == initPos[i][2] && moon.velocity[2] == 0))
        {
          found[2] = false;
        }
      }
    }
    for(int index = 0; index <= 2; ++index)
    {
      if(found[index] && !patternFound[index])
      {
        patternFound[index] = true;
        pattern[index] = timeStep;
        std::cout<<index<<" "<<timeStep<<"\n"<<std::endl;
      }
    }

    if(patternFound[0] && patternFound[1] && patternFound[2])
    {
      break;
    }
  }

  std::cout<<"calculating.."<<std::endl;
//  unsigned long long  x = pattern[0];
//  unsigned long long  y = pattern[1];
//  unsigned long long  z = pattern[2];
//  for(unsigned long long i = x; i < ULONG_LONG_MAX; i+=x)
//  {
//    if(i % x != 0ull) continue;
//    if(i % y != 0ull) continue;
//    if(i % z != 0ull) continue;
//    std::cout<<i<<std::endl;
//    return 0;
//  }

  //x 268296
  //y 231614
  //z 108344

  for(unsigned long long i = 1; i < ULONG_LONG_MAX; i++)
  {
    if((i*268296ull) % 268296ull != 0) continue;
    if((i*268296ull) % 231614ull != 0) continue;
    if((i*268296ull) % 108344ull != 0) continue;
    std::cout<<i*268296ull<<std::endl;
    return 0;
  }
}
