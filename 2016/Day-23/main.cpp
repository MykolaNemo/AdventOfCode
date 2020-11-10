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
    NOP
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
                                        {"nop",Type::NOP}};
#ifndef PART2
std::map<std::string, int> registers = {{"a",7},{"b",0},{"c",0},{"d",0}};
#else
std::map<std::string, int> registers = {{"a",12},{"b",0},{"c",0},{"d",0}};
#endif

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

void execute(std::vector<Instruction> instructions)
{
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
        default:
            std::cout<<"Unknown instruction"<<std::endl;
            exit(0);
        }
    }
}

int main()
{
#ifndef PART2
    std::fstream infile("input.txt");
#else
    std::fstream infile("input_modified.txt");
#endif
    std::vector<Instruction> instructions;
    {
        std::string instructionString;
        while(std::getline(infile, instructionString))
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
    execute(instructions);
    std::cout<<"Answer: "<<registers["a"]<<std::endl;
    return 0;
}
