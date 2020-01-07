#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

enum Mode
{
  Position = 0,
  Immediate = 1
};

inline int paramValuePos(const std::vector<int>& opcodes, int pos, Mode mode)
{
  switch(mode)
  {
  case Position:
  {
    return opcodes[pos];
  }
  case Immediate:
  {
    return pos;
  }
  default:
    throw std::exception();
  }
}

void runCode(std::vector<int>& opcodes)
{
  int pointer = 0;
  while("not terminated")
  {
    int opcode = opcodes[pointer];
    Mode mode3 = (Mode)(opcode / 10000);
    Mode mode2 = (Mode)((opcode % 10000) / 1000);
    Mode mode1 = (Mode)((opcode % 1000) / 100);

    int pos1, pos2, pos3 = 0;
    int instruction = opcode % 100;
    switch (instruction)
    {
    case 1:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      pos2 = paramValuePos(opcodes, pointer+2, mode2);
      pos3 = paramValuePos(opcodes, pointer+3, mode3);
      opcodes[pos3] = opcodes[pos1] + opcodes[pos2];
      pointer += 4;
      break;
    }
    case 2:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      pos2 = paramValuePos(opcodes, pointer+2, mode2);
      pos3 = paramValuePos(opcodes, pointer+3, mode3);
      opcodes[pos3] = opcodes[pos1] * opcodes[pos2];
      pointer += 4;
      break;
    }
    case 3:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      std::cout<<"Waiting for input: ";
      std::cin>>opcodes[pos1];
      pointer += 2;
      break;
    }
    case 4:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      std::cout<<opcodes[pos1]<<std::endl;
      pointer += 2;
      break;
    }
    case 5:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      pos2 = paramValuePos(opcodes, pointer+2, mode2);
      if(opcodes[pos1] != 0)
      {
        pointer = opcodes[pos2];
      }
      else
      {
        pointer += 3;
      }
      break;
    }
    case 6:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      pos2 = paramValuePos(opcodes, pointer+2, mode2);
      if(opcodes[pos1] == 0)
      {
        pointer = opcodes[pos2];
      }
      else
      {
        pointer += 3;
      }
      break;
    }
    case 7:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      pos2 = paramValuePos(opcodes, pointer+2, mode2);
      pos3 = paramValuePos(opcodes, pointer+3, mode3);
      if(opcodes[pos1] < opcodes[pos2])
      {
        opcodes[pos3] = 1;
      }
      else
      {
        opcodes[pos3] = 0;
      }
      pointer += 4;
      break;
    }
    case 8:
    {
      pos1 = paramValuePos(opcodes, pointer+1, mode1);
      pos2 = paramValuePos(opcodes, pointer+2, mode2);
      pos3 = paramValuePos(opcodes, pointer+3, mode3);
      if(opcodes[pos1] == opcodes[pos2])
      {
        opcodes[pos3] = 1;
      }
      else
      {
        opcodes[pos3] = 0;
      }
      pointer += 4;
      break;
    }
    case 99:
      return;
    default:
      std::cout<<"Error!"<<std::endl;
      return;
    }
  }
}

int main()
{
  std::ifstream infile("input.txt");
  std::vector<int> opcodes;
  std::string code;

  while(std::getline(infile, code, ',') )
  {
    opcodes.push_back(std::stoi(code));
  }
  runCode(opcodes);

  return 0;
}
