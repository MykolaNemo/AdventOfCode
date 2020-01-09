#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "../Computer.h"

void part1(std::vector<intmax_t> program)
{
  program[1] = 12;
  program[2] = 2;
  Computer comp;
  comp.setProgram(program);
  comp.start();

  std::cout<<"[Part1] Answer: "<<comp.getProgram()[0]<<std::endl;
}

void part2(std::vector<intmax_t> program)
{
  std::vector<intmax_t> input, output;
  for(int verb = 0; verb <= 99; ++verb)
  {
    for(int noun = 0; noun <= 99; ++noun)
    {
      std::vector<intmax_t> programCopy = program;
      programCopy[1] = verb;
      programCopy[2] = noun;

      Computer comp;
      comp.setProgram(programCopy);
      comp.start();
      if(comp.getProgram()[0] == 19690720)
      {
        std::cout<<"[Part2] Answer: "<<100*verb+noun<<std::endl;
      }
    }
  }
}

int main()
{
  std::ifstream infile("input.txt");
  std::vector<intmax_t> program;
  std::string code;

  while(std::getline(infile, code, ',') )
  {
    program.push_back(std::stoll(code));
  }

  part1(program);
  part2(program);

  return 0;
}