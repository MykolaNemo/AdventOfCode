#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>

// Because your neighbors keep defeating you in the holiday house decorating
// contest year after year, you've decided to deploy one million lights in a 1000x1000 grid.

// Furthermore, because you've been especially nice this year, Santa has mailed
// you instructions on how to display the ideal lighting configuration.

// Lights in your grid are numbered from 0 to 999 in each direction;
// the lights at each corner are at 0,0, 0,999, 999,999, and 999,0.
// The instructions include whether to turn on, turn off, or toggle various
// inclusive ranges given as coordinate pairs. Each coordinate pair represents
// opposite corners of a rectangle, inclusive; a coordinate pair like 0,0 through
// 2,2 therefore refers to 9 lights in a 3x3 square. The lights all start turned off.

// To defeat your neighbors this year, all you have to do is set up your lights by doing
// the instructions Santa sent you in order.

//For example:

//    turn on 0,0 through 999,999 would turn on (or leave on) every light.
//    toggle 0,0 through 999,0 would toggle the first line of 1000 lights, turning off the
//        ones that were on, and turning on the ones that were off.
//    turn off 499,499 through 500,500 would turn off (or leave off) the middle four lights.

//After following the instructions, how many lights are lit?

//#define PART1

struct Point
{
  Point(){}
  Point(int _x, int _y): x(_x), y(_y){}
  int x = 0;
  int y = 0;
};

struct Instruction
{
  enum Type
  {
    TurnOn = 0,
    TurnOff,
    Toggle
  };
  Instruction(Type t, Point& topL, Point& bottomR): type(t),topLeft(topL), bottomRight(bottomR){}
  Type type = TurnOn;
  Point topLeft;
  Point bottomRight;
};

bool operator<(const Point& p1, const Point& p2)
{
  if(p1.y < p2.y)
  {
    return true;
  }
  if(p1.y > p2.y)
  {
    return false;
  }
  if(p1.x < p2.x)
  {
    return true;
  }
  return false;
}

std::ostream& operator<<(std::ostream& stream, const Point& p)
{
  std::cout<<'('<<p.x<<','<<p.y<<')';
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const Instruction& in)
{
  std::cout<<in.type<<", "<<in.topLeft<<' '<<in.bottomRight;
  return stream;
}

//  turn off 464,858 through 833,915
//  turn on 599,989 through 806,993
//  toggle 50,472 through 452,788

Instruction parseInstruction(const std::string& instructionString)
{
  Instruction::Type type;
  Point topLeft;
  Point bottomRight;

  size_t spacePos1 = instructionString.find(' ');
  std::string firstWord = instructionString.substr(0, spacePos1);
  if(firstWord == "turn")
  {
    size_t spacePos2 = instructionString.substr(spacePos1+1, instructionString.size()-(spacePos1+1)).find(' ') + spacePos1;
    std::string secondWord = instructionString.substr(spacePos1+1, spacePos2-spacePos1);
    if(secondWord == "on")
    {
      type = Instruction::TurnOn;
    }
    else if(secondWord == "off")
    {
      type = Instruction::TurnOff;
    }
  }
  else if(firstWord == "toggle")
  {
    type = Instruction::Toggle;
  }

  size_t firstCoordComma = instructionString.find_first_of(',');
  size_t firstCoordBegin = instructionString.substr(0, firstCoordComma).find_last_of(' ')+1;
  size_t firstCoordEnd = instructionString.substr(firstCoordComma, instructionString.size()-firstCoordComma).find_first_of(' ') - 1 + firstCoordComma;
//  std::cout<<firstCoordComma<<std::endl;
//  std::cout<<firstCoordBegin<<std::endl;
//  std::cout<<firstCoordEnd<<std::endl;
//  std::cout<<instructionString.substr(firstCoordBegin,firstCoordComma-firstCoordBegin)<<std::endl;
//  std::cout<<instructionString.substr(firstCoordComma+1,firstCoordEnd-firstCoordComma)<<std::endl;
  topLeft.x = std::stoi(instructionString.substr(firstCoordBegin,firstCoordComma-firstCoordBegin));
  topLeft.y = std::stoi(instructionString.substr(firstCoordComma+1,firstCoordEnd-firstCoordComma));

  size_t secondCoordComma = instructionString.find_last_of(',');
  size_t secondCoordBegin = instructionString.substr(0, secondCoordComma).find_last_of(' ')+1;
  size_t secondCoordEnd = instructionString.size()-1;
//  std::cout<<secondCoordComma<<std::endl;
//  std::cout<<secondCoordBegin<<std::endl;
//  std::cout<<secondCoordEnd<<std::endl;
//  std::cout<<instructionString.substr(secondCoordBegin,secondCoordComma-secondCoordBegin)<<std::endl;
//  std::cout<<instructionString.substr(secondCoordComma+1,secondCoordEnd-secondCoordComma)<<std::endl;
  bottomRight.x = std::stoi(instructionString.substr(secondCoordBegin,secondCoordComma-secondCoordBegin));
  bottomRight.y = std::stoi(instructionString.substr(secondCoordComma+1,secondCoordEnd-secondCoordComma));

  return Instruction(type, topLeft, bottomRight);
}

int main()
{
  std::vector<std::string> instructions;

  std::string instruction;
  std::fstream infile("input.txt");
  while(std::getline(infile, instruction))
  {
    instructions.push_back(instruction);
  }

#ifdef PART1
  std::map<Point, bool> lights;
#else
  std::map<Point, int> lights;
  for(int x = 0; x <= 999; ++x)
  {
    for(int y = 0; y <= 999; ++y)
    {
      Point point{x,y};
      lights[point] = 0;
    }
  }
#endif

  for(auto& instruction : instructions)
  {
    Instruction in = parseInstruction(instruction);
#ifdef PART1
    switch (in.type)
    {
    case Instruction::TurnOn:
      for(int x = in.topLeft.x; x <= in.bottomRight.x; ++x)
      {
        for(int y = in.topLeft.y; y <= in.bottomRight.y; ++y)
        {
          Point point{x,y};
          lights[point] = true;
        }
      }
      break;
    case Instruction::TurnOff:
      for(int x = in.topLeft.x; x <= in.bottomRight.x; ++x)
      {
        for(int y = in.topLeft.y; y <= in.bottomRight.y; ++y)
        {
          Point point{x,y};
          lights[point] = false;
        }
      }
      break;
    case Instruction::Toggle:
      for(int x = in.topLeft.x; x <= in.bottomRight.x; ++x)
      {
        for(int y = in.topLeft.y; y <= in.bottomRight.y; ++y)
        {
          Point point{x,y};
          lights[point] = !lights[point];
        }
      }
      break;
    }
#else
    switch (in.type)
    {
    case Instruction::TurnOn:
      for(int x = in.topLeft.x; x <= in.bottomRight.x; ++x)
      {
        for(int y = in.topLeft.y; y <= in.bottomRight.y; ++y)
        {
          Point point{x,y};
          lights[point]++;
        }
      }
      break;
    case Instruction::TurnOff:
      for(int x = in.topLeft.x; x <= in.bottomRight.x; ++x)
      {
        for(int y = in.topLeft.y; y <= in.bottomRight.y; ++y)
        {
          Point point{x,y};
          lights.at(point)--;
          if(lights[point] < 0)
          {
            lights[point] = 0;
          }
        }
      }
      break;
    case Instruction::Toggle:
      for(int x = in.topLeft.x; x <= in.bottomRight.x; ++x)
      {
        for(int y = in.topLeft.y; y <= in.bottomRight.y; ++y)
        {
          Point point{x,y};
          lights[point] += 2;
        }
      }
      break;
    }

#endif
//    std::cout<<instruction<<std::endl;
//    std::cout<<in<<std::endl<<"===============================\n";
  }
#ifdef PART1
  int count = 0;
  for(auto& [point, lightOn] : lights)
  {
    if(lightOn)
    {
      count++;
    }
  }
  std::cout<<"[Part 1] Number of lights: "<<count<<std::endl;
#else
  int brightness = 0;
  for(auto& [point, lightBrightness] : lights)
  {
    brightness+=lightBrightness;
  }
  std::cout<<"[Part 2] Brightness: "<<brightness<<std::endl;
#endif

  std::ofstream outfile;
  outfile.open("output.txt");
  for(int x = 0; x <= 999; ++x)
  {
    for(int y = 0; y <= 999; ++y)
    {
      Point point{x,y};
//      if(lights[point])
//      {
        outfile<<std::setfill('0')<<std::setw(3)<<lights[point];
//      }
//      else
//      {
//        outfile<<' ';
//      }
    }
    outfile<<'\n';
  }
  outfile.close();
  return 0;
}
