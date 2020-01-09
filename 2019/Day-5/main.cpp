#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Computer.h"

int main()
{
  std::ifstream infile("input.txt");
  std::vector<intmax_t> program;

  std::string code;
  while(std::getline(infile, code, ',') )
  {
    program.push_back(std::stoll(code));
  }

  {
      Computer comp;
      CompInput input = {1};
      CompOutput output;
      comp.setProgram(program);
      comp.start(input, output);
      std::cout<<"[Part 1] Diagnostic code: "<<output.back()<<std::endl;
  }

  {
      Computer comp;
      CompInput input = {5};
      CompOutput output;
      comp.setProgram(program);
      comp.start(input, output);
      std::cout<<"[Part 2] Diagnostic code: "<<output.front()<<std::endl;
  }

  return 0;
}
