#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "../Computer.h"

using namespace std;

#define PART1

int main()
{
  std::ifstream infile("input.txt");
  std::vector<long long> program;

  std::string code;
  while(std::getline(infile, code, ','))
  {
    program.push_back(std::stol(code));
  }
#ifdef PART1
  std::vector<long long> input = {1};
#else
  std::vector<long long> input = {2};
#endif
  std::vector<long long> output;
  Computer m(0);
  m.setProgram(program);
  m.start(input, output);

  for(auto c : output)
  {
    std::cout<<c<<",";
  }
  std::cout<<std::endl;
  return 0;
}
