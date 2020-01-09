#include <iostream>
#include <fstream>
#include <string>
#include "Computer.h"
#include <stdlib.h>

using namespace std;

enum class Status
{
    Wall = 0,
    Ok,
    OkAndOxygen
};

enum class MapTile
{
    Unknown = -1,
    Wall = 0,
    Empty
};

enum class DirectionCommand
{
    North = 1,
    South = 2,
    West = 3,
    East = 4
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


    auto getNextPosition = [](const std::pair<int, int>& currentPos, const DirectionCommand& direction)->std::pair<int, int>{
        std::pair<int, int> result = currentPos;
        switch (direction) {
        case DirectionCommand::North:
        {
            result.second--;
            break;
        }
        case DirectionCommand::South:
        {
            result.second++;
            break;
        }
        case DirectionCommand::East:
        {
            result.first++;
            break;
        }
        case DirectionCommand::West:
        {
            result.first--;
            break;
        }
        }
        return result;
    };
    auto drawMap = [](const std::vector<std::vector<MapTile>>& map, const std::pair<int,int>& currentPos){
        system("CLS");
        int y = 0;
        std::cout<<"-------------------------------------------------------------------------\n";
        for(const auto& row : map)
        {
            int x = 0;
            std::cout<<"|";
            for(const MapTile& tile : row)
            {
                if((x == currentPos.first) && (y == currentPos.second))
                {
                    std::cout<<"X";
                }
                else
                {
                    switch (tile) {
                    case MapTile::Unknown:
                        std::cout<<" ";
                        break;
                    case MapTile::Wall:
                        std::cout<<"#";
                        break;
                    case MapTile::Empty:
                        std::cout<<".";
                        break;
                    }
                }
                x++;
            }
            y++;
            std::cout<<"|\n";
//            std::cout<<(x == currentPos.first) << (y == currentPos.second)<<"\n";
        }
        std::cout<<"-------------------------------------------------------------------------\n";
        std::cout.flush();
    };

    std::pair<int, int> currentPos = {0,0};
    std::pair<int, int> nextPos;
    std::pair<int, int> oxygenPos = {-1,-1};
    std::vector<std::vector<MapTile>> map;
    const int mapWidth = 71;
    const int mapHeight = 61;
    const int coordX = (mapWidth-1)*0.5;
    const int coordY = (mapHeight-1)*0.5;
//    const int coordX = 1;
//    const int coordY = 1;
    for(int row = 0; row < mapHeight; ++row)
    {
        std::vector<MapTile> v;
        for(int col = 0; col < mapWidth; ++col)
        {
            v.push_back(MapTile::Unknown);
        }
        map.push_back(v);
    }

    map[currentPos.second + coordY][currentPos.first + coordX] = MapTile::Empty;


    Computer arcade(0);
    arcade.setProgram(program);
    arcade.setWaitAfterOutput(true, 1);
    std::vector<long long> input = {(long long)DirectionCommand::North};
    std::vector<long long> output;
    nextPos = getNextPosition(currentPos, (DirectionCommand)input[0]);

    while(arcade.state() != Computer::Halted)
    {
      arcade.start(input, output);
      Status statusCode = (Status)output[0];
      output.clear();

      switch (statusCode) {
      case Status::Wall:
          map[nextPos.second + coordY][nextPos.first + coordX] = MapTile::Wall;
          break;
      case Status::Ok:
          map[nextPos.second + coordY][nextPos.first + coordX] = MapTile::Empty;
          currentPos = nextPos;
          break;
      case Status::OkAndOxygen:
          map[nextPos.second + coordY][nextPos.first + coordX] = MapTile::Empty;
          currentPos = nextPos;
          oxygenPos = nextPos;
          break;
      default:
          break;
      }

      if(oxygenPos.first == -1 && oxygenPos.second == -1)
      {
          drawMap(map, std::pair<int,int >(currentPos.first+coordX, currentPos.second+coordY));

          std::cout<<"\n        North - 8\n\nWest - 4         East - 6\n\n        South - 2\n";
          int direction = 0;
          while(!(direction == 8 || direction == 6 || direction == 4 || direction == 2))
          {
              std::cin>>direction;
          }
          if(direction == 8)
          {
            input.push_back((int)DirectionCommand::North);
          }
          else if(direction == 6)
          {
            input.push_back((int)DirectionCommand::East);
          }
          else if(direction == 4)
          {
            input.push_back((int)DirectionCommand::West);
          }
          else if(direction == 2)
          {
            input.push_back((int)DirectionCommand::South);
          }
          nextPos = getNextPosition(currentPos, (DirectionCommand)input[0]);
      }
      else
      {
          drawMap(map, std::pair<int,int >(currentPos.first+coordX, currentPos.second+coordY));
          std::cout<<"You win!"<<std::endl;
      }
    }
    return 0;
}
