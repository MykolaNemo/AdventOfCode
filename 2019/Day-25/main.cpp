#include <iostream>
#include <fstream>
#include <string>
#include "../Computer.h"

int main()
{
    std::ifstream infile("input.txt");
    std::vector<intmax_t> program;

    std::string code;
    while(std::getline(infile, code, ','))
    {
      program.push_back(std::stoll(code));
    }

    std::string commandString;
    std::vector<intmax_t> input;
    std::vector<intmax_t> output;
    Computer comp(0);
    comp.setProgram(program);
    comp.setWaitForInput(true);
    comp.setASCIIOutput(true);
    while(comp.state() != Computer::Halted)
    {
        comp.start(input, output);

        std::getline(std::cin, commandString);
        for(auto& c : commandString)
        {
            input.push_back((intmax_t)c);
        }
        input.push_back((intmax_t)'\n');
    }
    return 0;
}
