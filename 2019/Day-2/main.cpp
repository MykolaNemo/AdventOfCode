#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

void runCode(std::vector<int>& opcodes)
{
  int pos = 0;
  while(pos < opcodes.size())
  {
    int code = opcodes[pos];
    switch (code)
    {
    case 1:
      opcodes[opcodes[pos+3]] = opcodes[opcodes[pos+1]] + opcodes[opcodes[pos+2]];
      pos += 4;
      break;
    case 2:
      opcodes[opcodes[pos+3]] = opcodes[opcodes[pos+1]] * opcodes[opcodes[pos+2]];
      pos += 4;
      break;
    case 99:
      return;
    default:
      std::cout<<"Error!"<<std::endl;
      return;
    }
  }
}

void part1(std::vector<int> program)
{
  program[1] = 12;
  program[2] = 2;
  runCode(program);
  std::cout<<"[Part1] Answer: "<<program[0]<<std::endl;
}

void part2(std::vector<int> program)
{
  for(int verb = 0; verb <= 99; ++verb)
  {
    for(int noun = 0; noun <= 99; ++noun)
    {
      std::vector<int> programCopy = program;
      programCopy[1] = verb;
      programCopy[2] = noun;
      runCode(programCopy);
      if(programCopy[0] == 19690720)
      {
        std::cout<<"[Part2] Answer: "<<100*verb+noun<<std::endl;
      }
    }
  }
}

int main()
{
  std::ifstream infile("input.txt");
  std::vector<int> program;
  std::string code;

  while(std::getline(infile, code, ',') )
  {
    program.push_back(std::stoi(code));
  }

  part1(program);
  part2(program);

  return 0;
}