#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include "Computer.h"

//#define PART1

int main()
{
    std::ifstream infile("input.txt");
    std::vector<long long> program;

    std::string code;
    while(std::getline(infile, code, ','))
    {
      program.push_back(std::stoll(code));
    }

    std::vector<std::vector<long long>> inputs;
    std::vector<std::vector<long long>> outputs;
    std::vector<Computer*> computers;
    for(int i = 0; i < 50; ++i)
    {
        Computer* comp = new Computer(i);
        comp->setProgram(program);
        comp->setMultiThreading(true);
        inputs.push_back({i});
        outputs.push_back({});
        computers.push_back(comp);
    }

    long long NATx = 0;
    long long NATy = 0;
    int NATIdleCount = 0;
    int NATIdleMaxCount = 1000;
    std::optional<long long> lastNATy;

    while(true)
    {
        int NATIdleMonitor = 0;
        for(int i = 0; i < 50; ++i)
        {
            auto& comp = computers[i];
            auto& input = inputs[i];
            auto& output = outputs[i];
            if(input.empty())
            {
                input = {-1};
            }

            if(input.size() == 1 && input[0] == -1)
            {
                NATIdleMonitor++;
            }
            comp->start(input, output);
            if(!output.empty() && (output.size() == 3))
            {
                if(output[0] == 255)
                {
#ifdef PART1
                    std::cout<<"Part1 answer: "<<output[2]<<std::endl;
                    return 0;
#endif
                    NATx = output[1];
                    NATy = output[2];
                }
                else
                {
                    inputs[output[0]].push_back(output[1]);
                    inputs[output[0]].push_back(output[2]);
                }
                output.clear();
            }
        }
        if(NATIdleMonitor == 50)
        {
            NATIdleCount++;
            if(NATIdleCount == NATIdleMaxCount)
            {
                if(lastNATy.has_value() && lastNATy == NATy)
                {
                    std::cout<<"Part2 answer: "<<lastNATy.value()<<std::endl;
                    return 0;
                }
                lastNATy = NATy;
                NATIdleCount = 0;
                inputs[0].push_back(NATx);
                inputs[0].push_back(NATy);
            }
        }
        else
        {
            NATIdleCount = 0;
        }
    }

    return  0;
}
