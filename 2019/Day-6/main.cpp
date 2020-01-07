#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct StarSystem
{
  std::string sun;
  std::vector<StarSystem*> planets;
  int path = 0;
  bool visited = false;
};

namespace
{
std::vector<StarSystem*> universe;
StarSystem* gStartSystem = nullptr;
StarSystem* gEndSystem = nullptr;
int traverseResult = 0;

void traverseToTheCenter(std::string sun)
{
  if(sun == "COM")
  {
    return;
  }
  else
  {
    auto sunSystemIt = std::find_if(universe.begin(), universe.end(), [&sun](const StarSystem* sys)->bool{
      for(StarSystem* planet : sys->planets)
      {
        if(planet->sun == sun)
        {
          return true;
        }
      }
      return false;
    });
    if(sunSystemIt != universe.end())
    {
      traverseResult++;
      traverseToTheCenter((*sunSystemIt)->sun);
    }
  }
}

bool dijkstra(StarSystem* startSystem, StarSystem* endSystem, int& result)
{
  startSystem->visited = true;

  if(startSystem->sun == endSystem->sun)
  {
    result = startSystem->path;
    return true;
  }

  std::vector<StarSystem*> neighbours;
  for(StarSystem* planetSystem : startSystem->planets)
  {
    if((planetSystem->path == 0) && (planetSystem->visited == false))
    {
      planetSystem->path = startSystem->path+1;
      neighbours.push_back(planetSystem);
    }
  }

  auto sunSystemIt = std::find_if(universe.begin(), universe.end(), [startSystem](const StarSystem* sys)->bool{
    for(StarSystem* planet : sys->planets)
    {
      if(planet->sun == startSystem->sun)
      {
        return true;
      }
    }
    return false;
  });
  if(sunSystemIt != universe.end())
  {
    if(((*sunSystemIt)->path == 0) && ((*sunSystemIt)->visited == false))
    {
      (*sunSystemIt)->path = startSystem->path+1;
      neighbours.push_back(*sunSystemIt);
    }
  }

  for(StarSystem* neighbour : neighbours)
  {
      if(dijkstra(neighbour, endSystem, result))
      {
        return true;
      }
  }
  return false;
}
}

int main()
{
  std::ifstream infile("input.txt");

  std::string orbit;
  while(std::getline(infile, orbit))
  {
    std::string sun = orbit.substr(0,3);
    std::string planet = orbit.substr(4,3);

    auto sunSystemIt = std::find_if(universe.begin(), universe.end(), [&sun](const StarSystem* sys)->bool{
      return (sys->sun == sun);
    });
    auto planetIt = std::find_if(universe.begin(), universe.end(), [&planet](const StarSystem* sys)->bool{
      return (sys->sun == planet);
    });
    if(sunSystemIt == universe.end())
    {
      StarSystem* sunSystem = new StarSystem();
      sunSystem->sun = sun;
      if(planet == "YOU")
      {
        gStartSystem = sunSystem;
      }
      if(planet == "SAN")
      {
        gEndSystem = sunSystem;
      }

      if(planetIt == universe.end())
      {
        StarSystem* planetSystem = new StarSystem();
        planetSystem->sun = planet;
        sunSystem->planets.push_back(planetSystem);
        universe.push_back(planetSystem);
      }
      else
      {
        sunSystem->planets.push_back(*planetIt);
      }
      universe.push_back(sunSystem);
    }
    else
    {
      if(planet == "YOU")
      {
        gStartSystem = *sunSystemIt;
      }
      if(planet == "SAN")
      {
        gEndSystem = *sunSystemIt;
      }

      if(planetIt == universe.end())
      {
        StarSystem* planetSystem = new StarSystem();
        planetSystem->sun = planet;
        (*sunSystemIt)->planets.push_back(planetSystem);
        universe.push_back(planetSystem);
      }
      else
      {
        (*sunSystemIt)->planets.push_back(*planetIt);
      }
    }
  }

  std::cout<<"FROM "<<gStartSystem->sun<<" TO "<<gEndSystem->sun<<"\n";
//  for(const StarSystem& system : universe)
//  {
//    traverseToTheCenter(system.sun);
//  }

  int result = 0;
  if(dijkstra(gStartSystem, gEndSystem, result))
  {
    std::cout<<result<<std::endl;
  }
  else
  {
    std::cout<<"No Santa :("<<std::endl;
  }
  return 0;
}
