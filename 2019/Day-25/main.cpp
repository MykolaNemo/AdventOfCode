#include <iostream>
#include <fstream>
#include <string>
#include "Computer.h"

using namespace std;

//std::string north = "north";
//std::string south = "south";
//std::string east = "east";
//std::string west = "west";
//std::string take = "take ";
//std::string drop = "drop ";
//std::string inventory = "inv";

//std::string password = "password for the main airlock";

int main()
{
    std::ifstream infile("input.txt");
    std::vector<long long> program;

    std::string code;
    while(std::getline(infile, code, ','))
    {
      program.push_back(std::stoll(code));
    }

    std::string commandString;
    std::vector<long long> input;
    std::vector<long long> output;
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
            input.push_back((long long)c);
        }
        input.push_back((long long)'\n');
    }
    return 0;
}
