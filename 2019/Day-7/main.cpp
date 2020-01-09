#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../Computer.h"

#define PART1

std::vector<Computer> amplifiers;
std::vector<intmax_t> program;

std::vector<std::vector<intmax_t>> generatePhases(int from, int to)
{
    std::vector<std::vector<intmax_t>> list;
    for(int p1 = from; p1 <= to; ++p1)
    {
        for(int p2 = from; p2 <= to; ++p2)
        {
            if(p2 == p1) continue;
            for(int p3 = from; p3 <= to; ++p3)
            {
                if(p3 == p2 || p3 == p1) continue;
                for(int p4 = from; p4 <= to; ++p4)
                {
                    if(p4 == p3 || p4 == p2 || p4 == p1) continue;
                    for(int p5 = from; p5 <= to; ++p5)
                    {
                        if(p5 == p4 || p5 == p3 || p5 == p2 || p5 == p1) continue;
                        list.push_back({p1,p2,p3,p4,p5});
                    }
                }
            }
        }
    }
    return list;
}

intmax_t runAmplifiers(const std::vector<std::vector<intmax_t>>& phases)
{
    intmax_t maxSignal = 0;
    for(auto& phases : phases)
    {
        std::vector<CompInput> inputs;
        std::vector<CompInput> outputs;
        for(int ampNo = 0; ampNo < amplifiers.size(); ++ampNo)
        {
            CompInput inputVector;
            inputVector.push_back(phases[ampNo]);

            inputs.push_back(inputVector);
            outputs.push_back(CompOutput());

            amplifiers[ampNo].setProgram(program);
            amplifiers[ampNo].clearPointer();
        }

        int ampNo = 0;
        intmax_t signalValue = 0;
        while(amplifiers[4].state() != Computer::Halted)
        {
            auto& amplifier = amplifiers[ampNo];
            auto& input = inputs[ampNo];
            auto& output = outputs[ampNo];

            input.push_back(signalValue);
            amplifier.start(input, output);
            signalValue = output[0];
            output.clear();

            ampNo++;
            if(ampNo > 4)
            {
                ampNo = 0;
            }
        }
        if(outputs.back()[0] > maxSignal)
        {
            maxSignal = outputs.back()[0];
        }
    }
    return maxSignal;
}

int main()
{
    std::ifstream infile("input.txt");

    std::string code;
    while(std::getline(infile, code, ','))
    {
      program.push_back(std::stoll(code));
    }

    for(int i = 0; i < 5; ++i)
    {
        Computer amplifier;
        amplifier.setProgram(program);
        amplifier.setWaitAfterOutput(true);
        amplifiers.push_back(amplifier);
    }
    std::vector<std::vector<intmax_t>> phasesList;

    phasesList = generatePhases(0, 4);
    std::cout<<"[Part 1] Maximal signal: "<<runAmplifiers(phasesList)<<std::endl;

    phasesList = generatePhases(5, 9);
    std::cout<<"[Part 2] Maximal signal: "<<runAmplifiers(phasesList)<<std::endl;
    return 0;
}
