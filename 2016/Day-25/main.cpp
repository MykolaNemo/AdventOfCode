#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#include "../../utils.h"

#define PART2

enum class Type
{
    CPY,
    INC,
    DEC,
    JNZ,
    TGL,
    MUL,
    NOP,
    OUT
};

struct Instruction
{
    Type type;
    std::string param1;
    std::string param2;
    std::string param3;
};

std::map<std::string, Type> commands = {{"cpy",Type::CPY},{"inc",Type::INC},{"dec",Type::DEC},
                                        {"jnz",Type::JNZ},{"tgl",Type::TGL},{"mul",Type::MUL},
                                        {"nop",Type::NOP},{"out",Type::OUT}};
std::map<std::string, int> registers = {{"a",0},{"b",0},{"c",0},{"d",0}};

bool isRegister(std::string& param)
{
    return (std::find_if(registers.begin(),registers.end(), [&param](const auto& pair)->bool{
        return pair.first == param;}) != registers.end());
}

int value(std::string& param)
{
    if(std::find_if(registers.begin(),registers.end(), [&param](const auto& pair)->bool{
        return pair.first == param;}) != registers.end())
    {
        return registers[param];
    }
    return std::stoi(param);
}

int goldenValue = 0;

void execute(std::vector<Instruction> instructions)
{
    std::string output;
    int pos = 0;
    while(pos > -1 && pos < static_cast<int>(instructions.size()))
    {
        Instruction in = instructions[pos];
        switch (in.type)
        {
        case Type::CPY:
        {
            if(!isRegister(in.param2))
            {
                pos++;
                break;
            }
            registers[in.param2] = value(in.param1);
            pos++;
            break;
        }
        case Type::INC:
        {
            if(!isRegister(in.param1))
            {
                pos++;
                break;
            }
            registers[in.param1]++;
            pos++;
            break;
        }
        case Type::DEC:
        {
            if(!isRegister(in.param1))
            {
                pos++;
                break;
            }
            registers[in.param1]--;
            pos++;
            break;
        }
        case Type::JNZ:
        {
            if(value(in.param1) != 0)
            {
                pos += value(in.param2);
                break;
            }
            pos++;
            break;
        }
        case Type::TGL:
        {
            int pos2 = pos + value(in.param1);
            if(pos2 >= static_cast<int>(instructions.size()))
            {
                pos++;
                break;
            }
            Instruction& in2 = instructions[pos2];
            Type type = in2.type;
            switch (type) {
            case Type::INC:
            {
                in2.type = Type::DEC;
                break;
            }
            case Type::DEC:
            case Type::TGL:
            case Type::OUT:
            {
                in2.type = Type::INC;
                break;
            }
            case Type::JNZ:
            {
                in2.type = Type::CPY;
                break;
            }
            case Type::CPY:
            {
                in2.type = Type::JNZ;
                break;
            }
            case Type::MUL:
            case Type::NOP:
            {
                break;
            }
            default:
            {
                std::cout<<"TGL: unknown instruction type"<<std::endl;
                exit(0);
            }
            }
            pos++;
            break;
        }
        case Type::MUL:
        {
            registers[in.param1] = value(in.param2) * value(in.param3);
            pos++;
            break;
        }
        case Type::NOP:
        {
            pos++;
            break;
        }
        case Type::OUT:
        {
//            std::cout<<value(in.param1)<<" ";
            output += std::to_string(value(in.param1));
            if(output.size() == 100)
            {
                bool good = true;
                for(int i = 0; i < output.size(); ++i)
                {
                    if(i % 2 == 0 && output[i] != '0')
                    {
                        good = false;
                        break;
                    }
                    if(i % 2 == 1 && output[i] != '1')
                    {
                        good = false;
                        break;
                    }
                }
                if(good)
                {
                    std::cout<<"[Part 1] Answer: "<<goldenValue<<std::endl;
                    exit(0);
                }
                else
                {
                    return;
                }
            }
            pos++;
            break;
        }
        default:
            std::cout<<"Unknown instruction"<<std::endl;
            exit(0);
        }
    }
}

int main()
{
    auto content = readFile("input.txt");
    std::vector<Instruction> instructions;
    {
        for(const auto& instructionString : content)
        {
            Instruction instruction;
            std::vector<std::string> words = split(instructionString, " ");
            instruction.type = commands[words[0]];
            instruction.param1 = words[1];
            if(words.size() > 2)
            {
                instruction.param2 = words[2];
            }
            if(words.size() > 3)
            {
                instruction.param3 = words[3];
            }
            instructions.push_back(instruction);
        }
    }
    for(int i = 0; i < 5000; ++i)
    {
        goldenValue = i;
        registers["a"] = goldenValue;
        registers["b"] = 0;
        registers["c"] = 0;
        registers["d"] = 0;
        execute(instructions);
    }
    std::cout<<"Answer: "<<registers["a"]<<std::endl;
    return 0;
}
