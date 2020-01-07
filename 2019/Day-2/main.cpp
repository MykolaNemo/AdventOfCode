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

int main()
{
  std::ifstream infile("input.txt");
  std::vector<int> opcodes;
  std::string code;

  while(std::getline(infile, code, ',') )
  {
    opcodes.push_back(std::stoi(code));
  }

  for(int verb = 0; verb <= 99; ++verb)
  {
    for(int noun = 0; noun <= 99; ++noun)
    {
      std::vector<int> copy = opcodes;
      copy[1] = verb;
      copy[2] = noun;
      runCode(copy);
      if(copy[0] == 19690720)
      {
        std::cout<<100*verb+noun<<std::endl;
      }
    }
  }

  return 0;
}
