#include <iostream>
#include <fstream>
#include <string>
#include <Computer.h>

using namespace std;

int main()
{
    std::ifstream infile("input.txt");
    std::vector<long long> program;

    std::string code;
    while(std::getline(infile, code, ','))
    {
      program.push_back(std::stol(code));
    }

    std::vector<long long> input;
    std::vector<long long> output;
    Computer droneSystem(0);
    int startX = 0;
    int nextStart = 0;
    for(int y = 1000; y < 1000000; ++y)
    {
        std::cout<<y<<": ";
        bool oneStarted = false;
        startX = nextStart;
        for(int x = startX; true; x++)
        {
            input = {x, y};
            droneSystem.setProgram(program);
            droneSystem.clearPointer();
            droneSystem.start(input, output);
            std::cout<<output[0];
//            std::cout.flush();
            if(!oneStarted && output[0] == 1)
            {
                nextStart = x;
                oneStarted = true;
            }
            output.clear();
            if(oneStarted)
            {
                input = {x+99, y};
                droneSystem.setProgram(program);
                droneSystem.clearPointer();
                droneSystem.start(input, output);
                if(output[0] == 0)
                {
                    output.clear();
                    oneStarted = false;
                    break;
                }

                output.clear();
                input = {x, y+99};
                droneSystem.setProgram(program);
                droneSystem.clearPointer();
                droneSystem.start(input, output);
                if(output[0] == 0)
                {
                    output.clear();
                    oneStarted = false;
                    continue;
                }
                output.clear();

                input = {x+99, y+99};
                droneSystem.setProgram(program);
                droneSystem.clearPointer();
                droneSystem.start(input, output);
                if(output[0] == 0)
                {
                    output.clear();
                    oneStarted = false;
                    continue;
                }
                output.clear();
                std::cout<<"\n"<<x*10000+y<<std::endl;
                return 0;
            }
        }
        std::cout<<std::endl;
    }

    return 0;
}
