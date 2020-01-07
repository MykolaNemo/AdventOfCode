#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

struct Wire
{
  char side = '0';
  int distance = 0;
};

int main()
{
  std::ifstream infile("input.txt");

  std::vector<Wire> wires1;
  std::vector<Wire> wires2;

  std::string wire1String;
  std::string wire2String;
  std::getline(infile, wire1String);
  std::getline(infile, wire2String);

  std::string direction;
  std::stringstream stream(wire1String);
  while(std::getline(stream, direction, ',') )
  {
    char side = direction[0];
    int distance = std::stoi(direction.substr(1, direction.length()-1));
    Wire w;
    w.side = side;
    w.distance = distance;
    wires1.push_back(w);
  }

  std::stringstream stream2(wire2String);
  while(std::getline(stream2, direction, ',') )
  {
    char side = direction[0];
    int distance = std::stoi(direction.substr(1, direction.length()-1));
    Wire w;
    w.side = side;
    w.distance = distance;
    wires2.push_back(w);
  }

  std::vector<int> intersections;

  int x1w1 = 0;
  int y1w1 = 0;
  int x2w1 = 0;
  int y2w1 = 0;
  int wire1TotalDistance = 0;
  for(Wire w1 : wires1)
  {
    switch (w1.side)
    {
    case 'R':
      x2w1 += w1.distance;
      break;
    case 'L':
      x2w1 -= w1.distance;
      break;
    case 'U':
      y2w1 += w1.distance;
      break;
    case 'D':
      y2w1 -= w1.distance;
      break;
    }
    wire1TotalDistance += w1.distance;
//    std::cout<<x1w1<<","<<y1w1<<" "<<x2w1<<","<<y2w1<<"\n";

    int x1w2 = 0;
    int y1w2 = 0;
    int x2w2 = 0;
    int y2w2 = 0;
    int wire2TotalDistance = 0;
    for(Wire w2 : wires2)
    {
      switch (w2.side)
      {
      case 'R':
        x2w2 += w2.distance;
        break;
      case 'L':
        x2w2 -= w2.distance;
        break;
      case 'U':
        y2w2 += w2.distance;
        break;
      case 'D':
        y2w2 -= w2.distance;
        break;
      }
      wire2TotalDistance += w2.distance;

      switch (w1.side)
      {
      case 'R':
        if(w2.side == 'U')
        {
          if(((x1w1 < x1w2) && (x1w2 < x2w1)) &&
             ((y1w2 < y1w1) && (y1w1 < y2w2)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w1.distance-(x1w2-x1w1))
                                                                   - (w2.distance-(y1w1-y1w2));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w2) + std::abs(y1w1);
//            intersections.push_back(manhatten);
          }
        }
        else if(w2.side == 'D')
        {
          if(((x1w1 < x1w2) && (x1w2 < x2w1)) &&
             ((y1w2 > y1w1) && (y1w1 > y2w2)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w1.distance-(x1w2-x1w1))
                                                                   - (w2.distance-(y1w2 - y1w1));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w2) + std::abs(y1w1);
//            intersections.push_back(manhatten);
          }
        }
        break;
      case 'L':
        if(w2.side == 'U')
        {
          if(((x1w1 > x1w2) && (x1w2 > x2w1)) &&
             ((y1w2 < y1w1) && (y1w1 < y2w2)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w1.distance-(x1w1-x1w2))
                                                                   - (w2.distance-(y1w1 - y1w2));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w2) + std::abs(y1w1);
//            intersections.push_back(manhatten);
          }
        }
        else if(w2.side == 'D')
        {
          if(((x1w1 > x1w2) && (x1w2 > x2w1)) &&
             ((y1w2 > y1w1) && (y1w1 > y2w2)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w1.distance-(x1w1-x1w2))
                                                                   - (w2.distance-(y1w2 - y1w1));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w2) + std::abs(y1w1);
//            intersections.push_back(manhatten);
          }
        }
        break;
      case 'U':
        if(w2.side == 'R')
        {
          if(((x1w2 < x1w1) && (x1w1 < x2w2)) &&
             ((y1w1 < y1w2) && (y1w2 < y2w1)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w2.distance-(x1w1-x1w2))
                                                                   - (w1.distance-(y1w2 - y1w1));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w1) + std::abs(y1w2);
//            intersections.push_back(manhatten);
          }
        }
        else if(w2.side == 'L')
        {
          if(((x1w2 > x1w1) && (x1w1 > x2w2)) &&
             ((y1w1 < y1w2) && (y1w2 < y2w1)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w2.distance-(x1w2-x1w1))
                                                                   - (w1.distance-(y1w2 - y1w1));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w1) + std::abs(y1w2);
//            intersections.push_back(manhatten);
          }
        }
        break;
      case 'D':
        if(w2.side == 'R')
        {
          if(((x1w2 < x1w1) && (x1w1 < x2w2)) &&
             ((y1w1 > y1w2) && (y1w2 > y2w1)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w2.distance-(x1w1-x1w2))
                                                                   - (w1.distance-(y1w1 - y1w2));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w1) + std::abs(y1w2);
//            intersections.push_back(manhatten);
          }
        }
        else if(w2.side == 'L')
        {
          if(((x1w2 > x1w1) && (x1w1 > x2w2)) &&
             ((y1w1 > y1w2) && (y1w2 > y2w1)))
          {
            int distance = wire1TotalDistance + wire2TotalDistance - (w2.distance-(x1w2-x1w1))
                                                                   - (w1.distance-(y1w1 - y1w2));
            intersections.push_back(distance);
//            int manhatten = std::abs(x1w1) + std::abs(y1w2);
//            intersections.push_back(manhatten);
          }
        }
        break;
      }

      x1w2 = x2w2;
      y1w2 = y2w2;
    }

    x1w1 = x2w1;
    y1w1 = y2w1;
  }

  if(!intersections.empty())
  {
  int min = intersections[0];
  for(int i :intersections)
  {
    if (i < min)
    {
      min = i;
    }
  }
  std::cout<<min<<std::endl;
  }

  return 0;
}
