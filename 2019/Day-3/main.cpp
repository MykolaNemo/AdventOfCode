#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

struct Point
{
  Point(){}
  Point(int _x, int _y): x(_x), y(_y){}
  int x = 0;
  int y = 0;
};

struct Intersection
{
  Intersection(int mDist, int aDist): manhattenDistance(mDist), absoluteDistance(aDist){}
  int manhattenDistance = 0;
  int absoluteDistance = 0;
};

std::ostream& operator<< (std::ostream& stream, const Point& point)
{
  stream<<'('<<point.x<<", "<<point.y<<')';
  return stream;
}

struct Wire
{
  Wire(char dir, int length): direction(dir), length(length){}
  char direction = '0';
  int length = 0;
};

Point endOfTheWire(const Point& startPoint, const Wire& wire)
{
  Point endPoint = startPoint;
  switch (wire.direction)
  {
  case 'R':
    endPoint.x += wire.length;
    break;
  case 'L':
    endPoint.x -= wire.length;
    break;
  case 'U':
    endPoint.y += wire.length;
    break;
  case 'D':
    endPoint.y -= wire.length;
    break;
  }
  return endPoint;
}

bool inBetweenX(const Point& end1, const Point& between, const Point& end2)
{
  return (((end1.x < between.x) && (between.x < end2.x)) || ((end2.x < between.x) && (between.x < end1.x)));
}

bool inBetweenY(const Point& end1, const Point& between, const Point& end2)
{
  return (((end1.y < between.y) && (between.y < end2.y)) || ((end2.y < between.y) && (between.y < end1.y)));
}

bool areCrossed(const Point& start1, const Point& end1, const Point& start2, const Point& end2)
{
  bool crossed = inBetweenX(start1, start2, end1) && inBetweenY(start2, start1, end2);
  crossed |= (inBetweenX(start2, start1, end2) && inBetweenY(start1, start2, end1));
  return crossed;
}

void solution(const std::vector<Wire>& wires1, const std::vector<Wire>& wires2)
{
  std::vector<Intersection> intersections;

  Point w1PointStart, w1PointEnd;
  int wire1TotalDistance = 0;
  for(const Wire &wire1 : wires1)
  {
    wire1TotalDistance += wire1.length;
    w1PointEnd = endOfTheWire(w1PointStart, wire1);

    Point w2PointStart, w2PointEnd;
    int wire2TotalDistance = 0;
    for(const Wire& wire2 : wires2)
    {
      wire2TotalDistance += wire2.length;
      w2PointEnd = endOfTheWire(w2PointStart, wire2);

      if(!areCrossed(w1PointStart, w1PointEnd, w2PointStart, w2PointEnd))
      {
        w2PointStart = w2PointEnd;
        continue;
      }

      int absoluteDistance = wire1TotalDistance + wire2TotalDistance;
      int manhattenDistance = 0;
      switch (wire1.direction)
      {
      case 'R':
      case 'L':
      {
        int wire1Partial = wire1.length - std::abs(w2PointStart.x - w1PointStart.x);
        int wire2Partial = wire2.length - std::abs(w2PointStart.y - w1PointStart.y);
        absoluteDistance -= (wire1Partial + wire2Partial);
        manhattenDistance = std::abs(w2PointStart.x) + std::abs(w1PointStart.y);
        break;
      }
      case 'U':
      case 'D':
      {
        int wire1Partial = wire1.length - std::abs(w2PointStart.y - w1PointStart.y);
        int wire2Partial = wire2.length - std::abs(w2PointStart.x - w1PointStart.x);
        absoluteDistance -= (wire1Partial + wire2Partial);
        manhattenDistance = std::abs(w1PointStart.x) + std::abs(w2PointStart.y);
        break;
      }
      }
      intersections.push_back(Intersection(manhattenDistance, absoluteDistance));
      w2PointStart = w2PointEnd;
    }
    w1PointStart = w1PointEnd;
  }

  if(!intersections.empty())
  {
    int minManhatten = intersections[0].manhattenDistance;
    int minAbsDistance = intersections[0].absoluteDistance;
    for(const Intersection& i : intersections)
    {
      if (i.manhattenDistance < minManhatten)
      {
        minManhatten = i.manhattenDistance;
      }
      if (i.absoluteDistance < minAbsDistance)
      {
        minAbsDistance = i.absoluteDistance;
      }
    }
    std::cout<<"[Part1] Answer: "<<minManhatten<<std::endl;
    std::cout<<"[Part2] Answer: "<<minAbsDistance<<std::endl;
  }
}

void parseWire(const std::string& parseString, std::vector<Wire>& wires)
{
  std::string directionString;
  std::stringstream stream(parseString);
  while(std::getline(stream, directionString, ',') )
  {
    char direction = directionString[0];
    int length = std::stoi(directionString.substr(1, directionString.length()-1));

    wires.push_back(Wire(direction, length));
  }
}

int main()
{
  std::vector<Wire> wires1, wires2;
  {
    std::ifstream infile("input.txt");

    std::string wire1String, wire2String;
    std::getline(infile, wire1String);
    std::getline(infile, wire2String);

    parseWire(wire1String, wires1);
    parseWire(wire2String, wires2);
  }
  solution(wires1, wires2);
  return 0;
}
