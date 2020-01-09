#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <set>
#include <list>
#include <string>

using namespace std;

using MapOfSymbols = std::vector<std::string>;
using MapOfSteps = std::vector<std::vector<int>>;

struct MapObject
{
  enum class Type
  {
    Unknown = 0,
    Door,
    Key,
    Entrance
  };

  MapObject(){}
  MapObject(char _name, int _x, int _y, Type _type): symbol(_name), x(_x), y(_y), type(_type){}

  char symbol = ' ';
  int x = -1;
  int y = -1;
  Type type = Type::Unknown;
};

struct Path
{
    Path(){}
    Path(int _steps): steps(_steps){}
    int steps = 0;
    std::vector<MapObject> objectsOnWay;
};

struct State
{
    std::set<MapObject> gatheredKeys;
    MapObject currentKey;
    int steps;
};

std::map<MapObject, std::map<MapObject, Path>> stepsBetweenKeys;
std::vector<MapObject> allDoors;
std::vector<MapObject> allKeys;

bool operator <(const MapObject& first, const MapObject& second)
{
  return (first.symbol < second.symbol);
}

bool operator ==(const MapObject& first, const MapObject& second)
{
  return first.symbol == second.symbol;
}

char keyForDoor(char door)
{
  return (door + 0x20);
}

MapObject findObject(int x, int y, const MapOfSymbols& map)
{
  if(map[y][x] >= 'a' && map[y][x] <= 'z')
  {
    return MapObject(map[y][x], x, y, MapObject::Type::Key);
  }
  else if(map[y][x] >= 'A' && map[y][x] <= 'Z')
  {
    return MapObject(map[y][x], x, y, MapObject::Type::Door);
  }
  else if(map[y][x] == '@')
  {
    return MapObject(map[y][x], x, y, MapObject::Type::Entrance);
  }
  return MapObject();
}

std::vector<std::pair<int, int>> findWays(int x, int y, const MapOfSymbols& map)
{
  std::vector<std::pair<int, int>> v;

  if(map[y-1][x] != '#')
  {
    v.push_back({x, y-1});
  }

  if(map[y][x+1] != '#')
  {
    v.push_back({x+1, y});
  }

  if(map[y+1][x] != '#')
  {
    v.push_back({x, y+1});
  }

  if(map[y][x-1] != '#')
  {
    v.push_back({x-1, y});
  }

  return v;
}

int findMinSteps(int x, int y, const MapOfSteps& stepsMap)
{
  int minSteps = -1;

  if(stepsMap[y-1][x] != -1 && (stepsMap[y-1][x] < minSteps || minSteps == -1))
  {
      minSteps = stepsMap[y-1][x];
  }

  if(stepsMap[y+1][x] != -1 && (stepsMap[y+1][x] < minSteps || minSteps == -1))
  {
    minSteps = stepsMap[y+1][x];
  }

  if(stepsMap[y][x+1] != -1 && (stepsMap[y][x+1] < minSteps || minSteps == -1))
  {
    minSteps = stepsMap[y][x+1];
  }

  if(stepsMap[y][x-1] != -1 && (stepsMap[y][x-1] < minSteps || minSteps == -1))
  {
    minSteps = stepsMap[y][x-1];
  }

  return minSteps;
}

std::vector<MapObject> gatherObjectsOnTheWay(const MapObject& from, const MapObject& to,
                                   const MapOfSteps& stepsMap, const MapOfSymbols& map)
{
    std::vector<MapObject> objects;
    int x = from.x;
    int y = from.y;
    while((x != to.x) || (y != to.y))
    {
        if(stepsMap[y-1][x]+1 == stepsMap[y][x] && (stepsMap[y-1][x] != -1))
        {
            y--;
        }
        else if(stepsMap[y][x+1]+1 == stepsMap[y][x] && (stepsMap[y][x+1] != -1))
        {
            x++;
        }
        else if(stepsMap[y+1][x]+1 == stepsMap[y][x] && (stepsMap[y+1][x] != -1))
        {
            y++;
        }
        else if(stepsMap[y][x-1]+1 == stepsMap[y][x] && (stepsMap[y][x-1] != -1))
        {
            x--;
        }
        MapObject object = findObject(x, y, map);
        if(object.type == MapObject::Type::Door)
        {
            auto it = std::find(allDoors.begin(), allDoors.end(), object);
            if(it != allDoors.end())
            {
                objects.push_back(object);
            }
        }
        else if(object.type == MapObject::Type::Key)
        {
            objects.push_back(object);
        }
    }
    return objects;
}

void calculateDistancesToKeys(const MapObject& startObject, const MapOfSymbols& map)
{
  const int startX = startObject.x;
  const int startY = startObject.y;

  MapOfSteps stepsMap;
  stepsMap.resize(map.size());
  for(auto& row : stepsMap)
  {
    for(int i = 0; i < map[0].size(); ++i)
    {
      row.push_back(-1);
    }
  }
  stepsMap[startY][startX] = findMinSteps(startX, startY, stepsMap) + 1;

  int visitX = -1;
  int visitY = -1;
  std::vector<std::pair<int, int>> visited;
  std::vector<std::pair<int, int>> toVisit = {{startX, startY}};
  while(!toVisit.empty())
  {
    visitX = toVisit.front().first;
    visitY = toVisit.front().second;
    for(const auto& neighbourWay : findWays(visitX, visitY, map))
    {
      const auto& itVisited = std::find(visited.begin(), visited.end(), neighbourWay);
      const auto& itToVisit = std::find(toVisit.begin(), toVisit.end(), neighbourWay);
      if(itVisited == visited.end() && itToVisit == toVisit.end())
      {
        toVisit.push_back(neighbourWay);

        const int neighbourX = neighbourWay.first;
        const int neighbourY = neighbourWay.second;
        if(stepsMap[neighbourY][neighbourX] == -1)
        {
          stepsMap[neighbourY][neighbourX] = findMinSteps(neighbourX, neighbourY, stepsMap) + 1;
        }
      }
    }
    {
      MapObject key = findObject(visitX, visitY, map);
      if(key.type == MapObject::Type::Key)
      {
        Path path(stepsMap[visitY][visitX]);
        path.objectsOnWay = gatherObjectsOnTheWay(key, startObject, stepsMap, map);
        stepsBetweenKeys[startObject][key] = path;
      }
    }

    visited.push_back({visitX, visitY});
    if(!toVisit.empty())
    {
      toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), toVisit.front()), toVisit.end());
    }
  }
}

int BFS(MapObject entrance, const std::vector<MapObject>& allKeys)
{
    std::vector<State> queue;
    queue.insert(queue.begin(), State{{}, entrance, 0});

    int result = -1;
    while(!queue.empty())
    {
        State state = queue.back();
        queue.pop_back();
        if(state.gatheredKeys.size() == allKeys.size())
        {
            if(result == -1 || result > state.steps)
            {
                result = state.steps;
                continue;
            }
        }
        for(const MapObject& key : allKeys)
        {
            if(key == state.currentKey)
            {
                continue;
            }
            auto gatheredIt = std::find(state.gatheredKeys.begin(), state.gatheredKeys.end(), key);
            if(gatheredIt != state.gatheredKeys.end())
            {
                continue;
            }

            auto objects = stepsBetweenKeys[state.currentKey][key].objectsOnWay;
            bool nextKeyReachable = true;
            for(const MapObject& object : objects)
            {
                if(object.type == MapObject::Type::Key)
                {
                    auto gatheredIt = std::find(state.gatheredKeys.begin(), state.gatheredKeys.end(), object);
                    if(gatheredIt == state.gatheredKeys.end())
                    {
                        nextKeyReachable = false;
                    }
                    break;
                }
                else if(object.type == MapObject::Type::Door)
                {
                    char neededKeySymbol = keyForDoor(object.symbol);
                    auto stateIt = std::find_if(state.gatheredKeys.begin(), state.gatheredKeys.end(),
                                                [&neededKeySymbol](const MapObject& checkKey)->bool{
                        return checkKey.symbol == neededKeySymbol;
                    });
                    if(stateIt == state.gatheredKeys.end())
                    {
                        nextKeyReachable = false;
                        break;
                    }
                }
            }
            if(!nextKeyReachable) continue;

            State newState = state;
            newState.gatheredKeys.insert(key);
            newState.steps += stepsBetweenKeys[newState.currentKey][key].steps;
            newState.currentKey = key;

            auto stateIt = std::find_if(queue.begin(), queue.end(), [&newState](const State& checkState)->bool{
                return (checkState.gatheredKeys == newState.gatheredKeys) &&
                       (checkState.currentKey == newState.currentKey);
            });

            if(stateIt != queue.end())
            {
                if(stateIt->steps > newState.steps)
                {
                    stateIt->steps = newState.steps;
                }
            }
            else
            {
                queue.insert(queue.begin(), newState);
            }
        }
    }
    return result;
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

  MapObject entrance;
  {
    int y = 0;
    for(auto& s : TheMap)
    {
      std::cout<<s<<std::endl;
      int x = 0;
      for(auto& c : s)
      {
        MapObject obj = findObject(x, y, TheMap);
        if(obj.symbol != ' ')
        {
          switch(obj.type)
          {
          case MapObject::Type::Key:
          {
            allKeys.push_back(obj);
            break;
          }
          case MapObject::Type::Door:
          {
            allDoors.push_back(obj);
            break;
          }
          case MapObject::Type::Entrance:
          {
            entrance = obj;
            break;
          }
          default:
              break;
          }
        }
        x++;
      }
      y++;
    }
  }

  //part 2
  auto doorIt = allDoors.begin();
  while(doorIt != allDoors.end())
  {
      char neededKeySymbol = keyForDoor(doorIt->symbol);
      auto stateIt = std::find_if(allKeys.begin(), allKeys.end(), [&neededKeySymbol](const MapObject& checkKey)->bool{
          return checkKey.symbol == neededKeySymbol;
      });
      if(stateIt == allKeys.end())
      {
          doorIt = allDoors.erase(doorIt);
      }
      else
      {
          doorIt++;
      }
  }
  //part 2 end
  std::cout<<"Entrance: "<<entrance.x<<", "<<entrance.y<<"\n";

  for(const auto& key : allKeys)
  {
    calculateDistancesToKeys(key, TheMap);
  }
  calculateDistancesToKeys(entrance, TheMap);

  std::cout<<"\nKeys: \n";
  {
      auto it = stepsBetweenKeys.begin();
      while(it != stepsBetweenKeys.end())
      {
          std::cout<<it->first.symbol<<": ";

          auto it2 = it->second.begin();
          if(it->first.symbol != '@')
          {
              while((it2->first.symbol != it->first.symbol) && (it2 != it->second.end()))
              {
                  it2++;
                  std::cout<<std::setw(5)<<"";
              }
          }
          else
          {
              std::cout<<std::setw(5)<<"";
          }
          while(it2 != it->second.end())
          {
              std::cout<<std::setw(5)<<it2->second.steps;
              for(auto& object : it2->second.objectsOnWay)
              {
                  std::cout<<object.symbol;
              }
              ++it2;
          }
          std::cout<<std::endl;
          ++it;
      }
  }

  int result = BFS(entrance, allKeys);
  std::cout<<"Result: "<<result<<std::endl;

  return 0;
}
