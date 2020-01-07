#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <set>
#include <list>

using namespace std;

struct MapObject;
//struct PartialMap;

//using DoorsMap = std::map<char, std::pair<int, int>>;
//using KeysVector = std::vector<std::pair<MapObject, int>>;
using MapOfSymbols = std::vector<std::string>;
using MapOfSteps = std::vector<std::vector<int>>;

std::map<MapObject, std::map<MapObject, int>> stepsBetweenKeys;
std::vector<MapObject> doors;

int globalId = 1;

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
  MapObject(char _name, std::pair<int, int> _coords, Type _type): symbol(_name), x(_coords.first), y(_coords.second), type(_type){}
  MapObject(char _name, int _x, int _y, Type _type): symbol(_name), x(_x), y(_y), type(_type){}

  char symbol = ' ';
  int x = -1;
  int y = -1;
  Type type = Type::Unknown;
};

struct Tree
{
  Tree(){}
  Tree(MapObject object): value(std::move(object)){}

  Tree* parent = nullptr;
  std::vector<Tree*> children;
  MapObject value;
};

//struct Tile
//{
//  char symbol;
//  int steps;
//};

struct PartialMap
{
  PartialMap()
  {
    id = globalId;
    globalId++;
  }
  int id;
//  MapOfSymbols map;
  std::vector<MapObject> keys;
  std::vector<MapObject> doors;
  std::map<MapObject, std::pair<int,int>> pointsBeforeDoors;
};

bool operator <(const MapObject& first, const MapObject& second)
{
  return (first.symbol < second.symbol);
}

bool operator ==(const MapObject& first, const MapObject& second)
{
  return first.symbol == second.symbol;
}


//template<typename T>
//std::vector<std::vector<T>> permutations(const std::vector<T>& input)
//{
//    const auto swap = [](T& a, T& b){
//        T t = a;
//        a = b;
//        b = t;
//    };

//    std::vector<std::vector<T>> result;
//    const std::function<void(size_t, std::vector<T>&)> generate =
//    [&generate, &swap, &result](size_t k, std::vector<T>& input){
//        if (k == 1)
//        {
//            result.push_back(input);
//        }
//        else
//        {
//            for (int i = 0; i < k; ++i)
//            {
//                generate(k - 1, input);
//                if (k % 2 == 0)
//                {
//                    swap(input[i], input[k-1]);
//                }
//                else
//                {
//                    swap(input[0], input[k-1]);
//                }
//            }
//        }
//        return std::vector<T>();
//    };

//    std::vector<T> temp = input;
//    generate(input.size(), temp);

//    return result;
//}

template<typename T>
void printMap(std::vector<T>& v)
{
  for(T& e : v)
  {
    std::cout<<e<<std::endl;
  }
  std::cout<<std::endl;
}

char doorForKey(char key)
{
  return (key - 0x20);
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

//PartialMap* openDoor(const MapObject& key, std::list<PartialMap*>& partialMaps, PartialMap* currentMap)
//{
//  char doorName = doorForKey(key.symbol);
//  auto doorIt = std::find_if(doors.begin(), doors.end(), [&doorName](const MapObject& door){
//    return door.symbol == doorName;
//  });
//  if(doorIt == doors.end())
//  {
//    return currentMap;
//  }

//  MapObject openedDoor = *doorIt;

//  bool returnCurrentMap = false;
//  auto it = std::find(currentMap->doors.begin(), currentMap->doors.end(), openedDoor);
//  if(it == currentMap->doors.end())
//  {
//    returnCurrentMap = true;
//  }

//  PartialMap* first = nullptr;
//  PartialMap* second = nullptr;
//  {
//    auto partialMapWithDoor = [&openedDoor](PartialMap* map)->bool{
//      return (std::find(map->doors.begin(), map->doors.end(), openedDoor) != map->doors.end());
//    };

//    auto it1 = std::find_if(partialMaps.begin(), partialMaps.end(), partialMapWithDoor);
//    if(it1 == partialMaps.end())
//    {
//      return nullptr;
//    }
//    else
//    {
//      first = *it1;
//      it1 = partialMaps.erase(it1);
//      auto removeIt = std::remove(first->doors.begin(),first->doors.end(), openedDoor);
//      first->doors.erase(removeIt, first->doors.end());
//      first->pointsBeforeDoors.erase(openedDoor);
//    }

//    auto it2 = std::find_if(it1, partialMaps.end(), partialMapWithDoor);
//    if(it2 == partialMaps.end())
//    {
//      return nullptr;
//    }
//    else
//    {
//      second = *it2;
//      partialMaps.erase(it2);
//      auto removeIt = std::remove(second->doors.begin(),second->doors.end(), openedDoor);
//      second->doors.erase(removeIt, second->doors.end());
//      second->pointsBeforeDoors.erase(openedDoor);
//    }
//  }

//  PartialMap* merged = new PartialMap();
//  for(MapObject& door : first->doors)
//  {
//    merged->doors.push_back(door);
//  }
//  for(MapObject& door : second->doors)
//  {
//    merged->doors.push_back(door);
//  }
//  for(MapObject& key : first->keys)
//  {
//    merged->keys.push_back(key);
//  }
//  for(MapObject& key : second->keys)
//  {
//    merged->keys.push_back(key);
//  }
//  for(auto& [door, point] : first->pointsBeforeDoors)
//  {
//    merged->pointsBeforeDoors[door] = point;
//  }
//  for(auto& [door, point] : second->pointsBeforeDoors)
//  {
//    merged->pointsBeforeDoors[door] = point;
//  }
//  delete first;
//  delete second;
//  partialMaps.push_back(merged);

//  if(returnCurrentMap)
//  {
//    return currentMap;
//  }
//  else
//  {
//    return merged;
//  }
//}

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
        stepsBetweenKeys[startObject][key] = stepsMap[visitY][visitX];
        stepsBetweenKeys[key][startObject] = stepsMap[visitY][visitX];
      }
    }

    visited.push_back({visitX, visitY});
    if(!toVisit.empty())
    {
      toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), toVisit.front()), toVisit.end());
    }
  }
}

//bool isMapBehindTheDoorExists(const MapObject& door, const std::pair<int,int>& pointBeforeDoors, std::list<PartialMap*> partialMaps)
//{
//  if(door.type == MapObject::Type::Door)
//  {
//    for(PartialMap* map : partialMaps)
//    {
//      if(map->pointsBeforeDoors[door] == pointBeforeDoors)
//      {
//        continue;
//      }

//      if(std::find(map->doors.begin(), map->doors.end(), door) != map->doors.end())
//      {
//        return true;
//      }
//    }
//  }
//  return false;
//}

//PartialMap* CreatePartialMap(int startX, int startY, const MapOfSymbols& map)
//{
//  PartialMap* partialMap = new PartialMap();

//  int visitX = -1;
//  int visitY = -1;
//  std::vector<std::pair<int, int>> visited;
//  std::vector<std::pair<int, int>> toVisit;
//  toVisit.push_back({startX, startY});
//  {
//    MapObject obj = findObject(startX, startY, map);
//    if(obj.type == MapObject::Type::Key)
//    {
//      partialMap->keys.push_back(obj);
//    }
//    else if(obj.type == MapObject::Type::Door)
//    {
//      partialMap->doors.push_back(obj);
//      for(auto& way : findWays(obj.x, obj.y, map))
//      {
//        MapObject door = findObject(way.first, way.second, map);
//        if(door.type == MapObject::Type::Door)
//        {
//          partialMap->doors.push_back(door);
//          partialMap->pointsBeforeDoors[obj] = {door.x,door.y};
//          partialMap->pointsBeforeDoors[door] = {obj.x,obj.y};
//          return partialMap;
//        }
//      }
//    }
//  }

//  while(!toVisit.empty())
//  {
//    visitX = toVisit.front().first;
//    visitY = toVisit.front().second;
//    for(auto& way : findWays(visitX, visitY, map))
//    {
//      auto itVisited = std::find(visited.begin(), visited.end(), way);
//      auto itToVisit = std::find(toVisit.begin(), toVisit.end(), way);
//      if(itVisited == visited.end() && itToVisit == toVisit.end())
//      {
//        int neighbourX = way.first;
//        int neighbourY = way.second;

//        MapObject obj = findObject(neighbourX, neighbourY, map);
//        if(obj.type == MapObject::Type::Door)
//        {
//          doors.push_back(obj);
//          partialMap->doors.push_back(obj);
//          partialMap->pointsBeforeDoors[obj] = {visitX,visitY};
//        }
//        else if(obj.type == MapObject::Type::Key)
//        {
//          partialMap->keys.push_back(obj);
//        }

//        if(obj.type != MapObject::Type::Door)
//        {
//          toVisit.push_back(way);
//        }
//      }
//    }

//    visited.push_back({visitX, visitY});
//    if(!toVisit.empty())
//    {
//      toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), toVisit.front()), toVisit.end());
//    }
//  }
//  return partialMap;
//}

//void CreatePartialMaps(int startX, int startY, const MapOfSymbols& map, std::list<PartialMap*>& partialMaps)
//{
//  PartialMap* partialMap = CreatePartialMap(startX, startY, map);
//  partialMaps.push_back(partialMap);

//  std::set<std::pair<MapObject, std::pair<int,int>> > doorsToCheck;
//  for(auto door : partialMap->doors)
//  {
//    doorsToCheck.insert({door, partialMap->pointsBeforeDoors[door]});
//  }
//  while(!doorsToCheck.empty())
//  {
//    auto pair = *(doorsToCheck.begin());
//    doorsToCheck.erase(doorsToCheck.begin());

//    const auto& door = pair.first;
//    const auto& pointBeforeDoor = pair.second;
//    if(!isMapBehindTheDoorExists(door, pointBeforeDoor, partialMaps))
//    {
//      std::pair<int, int> pointBehindDoor = {door.x,door.y};
//      if(pointBeforeDoor.first != door.x)
//      {
//        (pointBeforeDoor.first < door.x) ? pointBehindDoor.first++ : pointBehindDoor.first--;
//      }
//      if(pointBeforeDoor.second != door.y)
//      {
//        (pointBeforeDoor.second < door.y) ? pointBehindDoor.second++ : pointBehindDoor.second--;
//      }

//      auto partialMap = CreatePartialMap(pointBehindDoor.first, pointBehindDoor.second, map);
//      partialMaps.push_back(partialMap);
//      for(auto door : partialMap->doors)
//      {
//        doorsToCheck.insert({door, partialMap->pointsBeforeDoors[door]});
//      }
//    }
//  }
//}

//PartialMap* mapCopy(PartialMap* partialMap)
//{
//  PartialMap* newMap = new PartialMap();
//  newMap->id = partialMap->id;
//  newMap->doors = partialMap->doors;
//  newMap->keys = partialMap->keys;
//  newMap->pointsBeforeDoors = partialMap->pointsBeforeDoors;
//  return newMap;
//}

//std::list<PartialMap*> mapCopy(std::list<PartialMap*> partialMaps)
//{
//  std::list<PartialMap*> mapList;
//  for(PartialMap* map : partialMaps)
//  {
//    mapList.push_back(mapCopy(map));
//  }
//  return mapList;
//}

//int a = 0;
//long long index = 0;
//int runForKeys(PartialMap* currentMap, const MapObject& startObject, std::list<PartialMap*>& partialMaps, std::string& seq)
//{
//  a++;
//  if(a > 30)
//  exit(0);
//  if(startObject.type == MapObject::Type::Unknown) return 0;

//  if(!currentMap) return 0;
//  if(currentMap->keys.empty()) return 0;

//  int minSteps = -1;
//  for(MapObject key: currentMap->keys)
//  {
//    std::string s = seq;
//    s.push_back(key.symbol);
//    std::list<PartialMap*> partialMapsTemp = mapCopy(partialMaps);
//    PartialMap* mapCopy = nullptr;
//    auto it = std::find_if(partialMapsTemp.begin(), partialMapsTemp.end(), [&currentMap](PartialMap* map)->bool{
//        return currentMap->id == map->id;
//    });
//    if(it != partialMapsTemp.end())
//    {
//      mapCopy = *it;
//    }

//    //pick up
//    int steps = stepsBetweenKeys[startObject][key];
//    mapCopy->keys.erase(std::remove(mapCopy->keys.begin(),mapCopy->keys.end(), key), mapCopy->keys.end());

//    //open the door and merge maps
//    PartialMap* newPartialMap = openDoor(key, partialMapsTemp, mapCopy);
//    newPartialMap = newPartialMap ? newPartialMap : mapCopy;

//    steps += runForKeys(newPartialMap, key, partialMapsTemp, s);
//    a--;
//    index++;
//    //    if(s.size() == 9)
//    {
//      std::cout<<s<<": "<<index<<" "<<a<<std::endl;
//    }
//    if(minSteps == -1 || steps < minSteps)
//    {
//      minSteps = steps;
//    }
//    for(auto map : partialMapsTemp)
//    {
//      delete map;
//    }
//  }
//  return minSteps;
//}



//void dijkstra(int startX, int startY, const MapOfSymbols& map)
//{
//  int visitX = -1;
//  int visitY = -1;
//  std::vector<std::pair<int, int>> visited;
//  std::vector<std::pair<int, int>> toVisit = {{startX, startY}};
//  while(!toVisit.empty())
//  {
//    visitX = toVisit.front().first;
//    visitY = toVisit.front().second;

//    //do something with current cell

//    for(const auto& neighbourWay : findWays(visitX, visitY, map))
//    {
//      const auto& itVisited = std::find(visited.begin(), visited.end(), neighbourWay);
//      const auto& itToVisit = std::find(toVisit.begin(), toVisit.end(), neighbourWay);
//      if(itVisited == visited.end() && itToVisit == toVisit.end())
//      {
//        toVisit.push_back(neighbourWay);
//        //do something with neighbour cell
//      }
//    }
//    visited.push_back({visitX, visitY});
//    if(!toVisit.empty())
//    {
//      toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), toVisit.front()), toVisit.end());
//    }
//  }
//}

//@(

//jJxX(R(D(F(A(Z(T(Hv(L(q(c(mI(Y(w(b(G)))))))))k))))u))

//r(d(U(V)))

//f(se(p(QC(M(N(y(W(B(g)))))i))o))

//a(t(K(h)))

//zS(EP(l)O)
//)

std::vector<MapObject> findClosestObjects(int startX, int startY, const MapOfSymbols& map, MapOfSteps& stepsMap)
{
  std::vector<MapObject> objects;

  stepsMap[startY][startX] = findMinSteps(startX, startY, stepsMap) + 1;

  std::vector<std::pair<int, int>> visited;
  std::vector<std::pair<int, int>> toVisit = {{startX, startY}};
  while(!toVisit.empty())
  {
    int visitX = toVisit.front().first;
    int visitY = toVisit.front().second;
    for(const auto& neighbourWay : findWays(visitX, visitY, map))
    {
      const auto& itVisited = std::find(visited.begin(), visited.end(), neighbourWay);
      const auto& itToVisit = std::find(toVisit.begin(), toVisit.end(), neighbourWay);
      if(itVisited == visited.end() && itToVisit == toVisit.end())
      {
        const int wayX = neighbourWay.first;
        const int wayY = neighbourWay.second;

        MapObject object = findObject(wayX, wayY, map);
        if(object.type == MapObject::Type::Key || object.type == MapObject::Type::Door)
        {
          objects.push_back(object);
        }
        else
        {
          toVisit.push_back(neighbourWay);
        }

        if(stepsMap[wayY][wayX] == -1)
        {
          stepsMap[wayY][wayX] = findMinSteps(wayX, wayY, stepsMap) + 1;
        }
      }
    }

    visited.push_back({visitX, visitY});
    if(!toVisit.empty())
    {
      toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), toVisit.front()), toVisit.end());
    }
  }
  return objects;
}

std::pair<int, int> closestUnexplored(int x, int y, MapOfSymbols& map, MapOfSteps& stepsMap)
{
  if(map[y+1][x] != '#' && stepsMap[y+1][x] == -1)
  {
    return {x, y+1};
  }
  if(map[y-1][x] != '#' && stepsMap[y-1][x] == -1)
  {
    return {x, y-1};
  }
  if(map[y][x+1] != '#' && stepsMap[y][x+1] == -1)
  {
    return {x+1, y};
  }
  if(map[y][x-1] != '#' && stepsMap[y][x-1] == -1)
  {
    return {x-1, y};
  }
  return {-1,-1};
}

int main()
{
  MapOfSymbols TheMap;
  std::vector<MapObject> keys;
  std::vector<MapObject> doors;

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
            keys.push_back(obj);
            break;
          }
          case MapObject::Type::Door:
          {
            doors.push_back(obj);
            break;
          }
          case MapObject::Type::Entrance:
          {
            entrance = obj;
            break;
          }
          }
        }
        x++;
      }
      y++;
    }
  }
  std::cout<<"Entrance: "<<entrance.x<<", "<<entrance.y<<"\n";

  for(const auto& key : keys)
  {
    calculateDistancesToKeys(key, TheMap);
  }
  calculateDistancesToKeys(entrance, TheMap);

//  std::list<PartialMap*> partialMaps;
//  CreatePartialMaps(entrance.x, entrance.y, TheMap,partialMaps);

  {
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
          std::cout<<std::setw(5)<<it2->second;
          ++it2;
        }
        std::cout<<std::endl;
        ++it;
      }
    }
//    std::cout<<"\nPartial maps: \n";
//    {
//      for(const PartialMap* map : partialMaps)
//      {
//        for(const MapObject& door : map->doors)
//        {
//          std::cout<<door.symbol<<" ";
//        }
//        std::cout<<"\n";
//        for(const MapObject& key : map->keys)
//        {
//          std::cout<<key.symbol<<" ";
//        }
//        std::cout<<"\n----------------------"<<std::endl;
//      }
//    }
  }

  Tree aMaze;
  aMaze.value = entrance;
  {
    std::function<void(Tree*, MapOfSymbols&, MapOfSteps&)> buildMazeTree;
    buildMazeTree = [&buildMazeTree](Tree* tree, MapOfSymbols& map, MapOfSteps& stepsMap){
      auto closestUnexploredPoint = closestUnexplored(tree->value.x, tree->value.y, map, stepsMap);
      if(closestUnexploredPoint.first != -1 && closestUnexploredPoint.second != -1)
      {
        auto objects = findClosestObjects(closestUnexploredPoint.first, closestUnexploredPoint.second, map, stepsMap);
        for(MapObject& object : objects)
        {
          Tree* child = new Tree(object);
          tree->children.push_back(child);
          child->parent = tree;
          map[child->value.y][child->value.x] = '#';
          buildMazeTree(child, map, stepsMap);
        }
      }
    };

    MapOfSteps stepsMap(TheMap.size());
    for(auto& row : stepsMap)
    {
      for(int i = 0; i < TheMap[0].size(); ++i)
      {
        row.push_back(-1);
      }
    }

    MapOfSymbols TheMapCopy = TheMap;
    auto objects = findClosestObjects(aMaze.value.x, aMaze.value.y, TheMapCopy, stepsMap);
    for(MapObject& object : objects)
    {
      Tree* child = new Tree(object);
      child->parent = &aMaze;
      aMaze.children.push_back(child);
      TheMapCopy[child->value.y][child->value.x] = '#';
      buildMazeTree(child, TheMapCopy, stepsMap);
    }

    std::function<bool(Tree*)> removal;
    removal = [&removal](Tree* tree)->bool{
      for(Tree* child : tree->children)
      {
        if (removal(child))
        {
//          delete child;
        }
      }

      if(tree->value.type == MapObject::Type::Door && tree->children.empty())
      {
        auto& parentChildren = tree->parent->children;
        parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), tree), parentChildren.end());
        return true;
      }
      return false;
    };
    removal(&aMaze);

    std::function<void(Tree*)> printTree;
    printTree = [&printTree](Tree* tree){
      std::cout<<tree->value.symbol;
      if(!tree->children.empty())
      {
        std::cout<<"(";
        for(auto childTree : tree->children)
        {
          printTree(childTree);
        }
        std::cout<<")"<<std::endl;
      }
    };
    printTree(&aMaze);
  }


//  std::string s;
//  std::cout<<"\nResult: "<<runForKeys(partialMaps.front(), entrance, partialMaps,s)<<std::endl;
  return 0;
}
