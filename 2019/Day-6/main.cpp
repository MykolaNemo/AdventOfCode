#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

struct PlanetSystem
{
  std::string name;
  std::vector<PlanetSystem*> moons;
  int path = 0;
  bool visited = false;
};

std::vector<PlanetSystem*> universe;

PlanetSystem* systemByStarName(const std::string& starName)
{
  auto systemIt = std::find_if(universe.begin(), universe.end(), [&starName](const PlanetSystem* system)->bool{
    return (system->name == starName);
  });

  if(systemIt != universe.end())
  {
    return *systemIt;
  }

  PlanetSystem* system = new PlanetSystem();
  system->name = starName;
  universe.push_back(system);
  return system;
}

PlanetSystem* systemByMoonName(const std::string& moonName)
{
  auto starSystemIt = std::find_if(universe.begin(), universe.end(), [&moonName](const PlanetSystem* system)->bool{
    for(auto moon : system->moons)
    {
      if(moon->name == moonName)
      {
        return true;
      }
    }
    return false;
  });

  if(starSystemIt != universe.end())
  {
    return *starSystemIt;
  }

  return nullptr;
}

void traverseToTheCenter(const std::string& systemName, int& orbitsCount)
{
  if(systemName == "COM")
  {
    return;
  }
  else
  {
    PlanetSystem* parentSystem = systemByMoonName(systemName);
    if(parentSystem)
    {
      traverseToTheCenter(parentSystem->name, ++orbitsCount);
    }
  }
}

bool traverseTheUniverse(PlanetSystem* startSystem, PlanetSystem* endSystem, int& steps)
{
  startSystem->visited = true;

  if(startSystem->name == endSystem->name)
  {
    steps = startSystem->path;
    return true;
  }

  std::vector<PlanetSystem*> systemsToVisit;
  for(PlanetSystem* moon : startSystem->moons)
  {
    if((moon->path == 0) && (moon->visited == false))
    {
      moon->path = startSystem->path + 1;
      systemsToVisit.push_back(moon);
    }
  }

  PlanetSystem* parentSystem = systemByMoonName(startSystem->name);
  if(parentSystem)
  {
    if((parentSystem->path == 0) && (parentSystem->visited == false))
    {
      parentSystem->path = startSystem->path + 1;
      systemsToVisit.push_back(parentSystem);
    }
  }

  for(PlanetSystem* toVisit : systemsToVisit)
  {
    if(traverseTheUniverse(toVisit, endSystem, steps))
    {
      return true;
    }
  }
  return false;
}

void part1()
{
  int orbitsCount = 0;
  for(auto system : universe)
  {
    traverseToTheCenter(system->name, orbitsCount);
  }
  std::cout<<"[Part 1] Number of orbits total: "<<orbitsCount<<std::endl;
}

void part2(PlanetSystem* startSystem, PlanetSystem* endSystem)
{
  int result = 0;
  if(traverseTheUniverse(startSystem, endSystem, result))
  {
    std::cout<<"[Part 2] Number of orbit switches: "<<result<<std::endl;
  }
  else
  {
    std::cout<<"[Part 2] No Santa :("<<std::endl;
  }
}

int main()
{
  PlanetSystem* StartSystem = nullptr;
  PlanetSystem* EndSystem = nullptr;

  std::ifstream infile("input.txt");

  std::string orbitInfo;
  while(std::getline(infile, orbitInfo))
  {
    std::string starName = orbitInfo.substr(0, 3);
    std::string moonName = orbitInfo.substr(4, 3);

    PlanetSystem* moonSystem = systemByStarName(moonName);
    PlanetSystem* starSystem = systemByStarName(starName);
    starSystem->moons.push_back(moonSystem);

    if(moonName == "YOU")
    {
      StartSystem = starSystem;
    }
    else if(moonName == "SAN")
    {
      EndSystem = starSystem;
    }
  }
  std::cout<<"FROM "<<StartSystem->name<<" TO "<<EndSystem->name<<"\n";

  part1();
  part2(StartSystem, EndSystem);
  return 0;
}