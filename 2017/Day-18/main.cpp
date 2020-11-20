#include <iostream>
#include <map>
#include "../../utils.h"

//#define PART1

enum Operation
{
    SND,
    SET,
    ADD,
    MUL,
    MOD,
    RCV,
    JGZ
};

struct Instruction
{
    Operation op;
    std::string param1;
    std::string param2;
};

std::map<std::string, Operation> opMap = {{"snd", SND}, {"set", SET}, {"add", ADD},
                                          {"mul", MUL}, {"mod", MOD}, {"rcv", RCV},
                                          {"jgz", JGZ}};


/*
    snd X plays a sound with a frequency equal to the value of X.
    set X Y sets register X to the value of Y.
    add X Y increases register X by the value of Y.
    mul X Y sets register X to the result of multiplying the value contained in register X by the value of Y.
    mod X Y sets register X to the remainder of dividing the value contained in register X by the value of Y (that is, it sets X to the result of X modulo Y).
    rcv X recovers the frequency of the last sound played, but only when the value of X is not zero. (If it is zero, the command does nothing.)
    jgz X Y jumps with an offset of the value of Y, but only if the value of X is greater than zero. (An offset of 2 skips the next instruction, an offset of -1 jumps to the previous instruction, and so on.)
*/


struct Computer
{
    enum Mode
    {
        Continuos,
        Interruptable
    };
    enum ExitCode
    {
        OK,
        WAIT,
        END,
    };
    Mode mode = Continuos;
    std::map<std::string, long long> registers;
    std::vector<Instruction> instructions;
    int pos = 0;
    Computer* computerForSend = nullptr;
    std::vector<long long> receivingQueue;
    int valuesSent = 0;

#ifdef PART1
    long long soundPlayed = 0;
    long long recoveredSound = 0;
#else
    void send(long long value)
    {
        receivingQueue.push_back(value);
    }
#endif

    Computer(std::vector<Instruction> _instructions) : instructions(std::move(_instructions)){}

    long long value(std::string v)
    {
        if(v.size() == 1 && v[0] >= 'a' && v[0] <= 'z')
        {
            return registers.at(v);
        }
        return std::stoll(v);
    }

    void reset()
    {
        pos = 0;
        registers.clear();
    }

    ExitCode execute()
    {
        while(true)
        {
            Instruction in = instructions[pos];
            switch (in.op) {
            case SND:
            {
#ifdef PART1
                soundPlayed = value(in.param1);
#else
                computerForSend->send(value(in.param1));
                valuesSent++;
#endif
                pos++;
                break;
            }
            case SET:
            {
                registers[in.param1] = value(in.param2);
                pos++;
                break;
            }
            case ADD:
            {
                registers[in.param1] += value(in.param2);
                pos++;
                break;
            }
            case MUL:
            {
                registers[in.param1] *= value(in.param2);
                pos++;
                break;
            }
            case MOD:
            {
                registers[in.param1] = value(in.param1) % value(in.param2);
                pos++;
                break;
            }
            case RCV:
            {
#ifdef PART1
                if(value(in.param1) != 0)
                {
                    recoveredSound = soundPlayed;
                    std::cout<<"[Part 1] Answer: "<<recoveredSound<<std::endl;
                    exit(0);
                }
#else
                if(!receivingQueue.empty())
                {
                    registers[in.param1] = receivingQueue.front();
                    receivingQueue.erase(receivingQueue.begin());
                }
                else
                {
                    return WAIT;
                }
#endif
                pos++;
                break;
            }
            case JGZ:
            {
                if(value(in.param1) > 0)
                {
                    pos += value(in.param2);
                }
                else
                {
                    pos++;
                }
                break;
            }
            default:break;
            }

            if(pos >= static_cast<int>(instructions.size()))
            {
                return END;
            }
            if(mode == Interruptable)
            {
                return OK;
            }
        }
        return END;
    }
};

int main()
{
    std::vector<Instruction> instructions;
    {
        for(auto str : readFile("input.txt"))
        {
            auto words = split(str, " ");
            Instruction in;
            in.op = opMap[words[0]];
            in.param1 = words[1];
            if(words.size() > 2)
            {
                in.param2 = words[2];
            }
            instructions.push_back(in);
        }
    }

    Computer* comp = new Computer(instructions);
    Computer* comp2 = new Computer(instructions);

#ifndef PART1
    comp->mode = Computer::Interruptable;
    comp2->mode = Computer::Interruptable;

    comp->registers["p"] = 0;
    comp2->registers["p"] = 1;
#endif

    comp->computerForSend = comp2;
    comp2->computerForSend = comp;

    while(true)
    {
        Computer::ExitCode code1 = comp->execute();
        Computer::ExitCode code2 = comp2->execute();
        if(code1 != Computer::OK && code2 != Computer::OK)
        {
            std::cout<<"[Part 2] Answer: "<<comp2->valuesSent<<std::endl;
            break;
        }
    }
    return 0;
}
