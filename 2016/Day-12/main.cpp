#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#define PART2

enum class Type
{
    CPY,
    INC,
    DEC,
    JNZ
};

struct Instruction
{
    Type type;
    std::string param1;
    std::string param2;
};

std::map<std::string, Type> commands = {{"cpy",Type::CPY},{"inc",Type::INC},{"dec",Type::DEC},{"jnz",Type::JNZ}};
std::map<std::string, int> registers = {{"a",0},{"b",0},{"c",0},{"d",0}};

int value(std::string& param)
{
    if(std::find_if(registers.begin(),registers.end(), [&param](const auto& pair)->bool{
        return pair.first == param;}) != registers.end())
    {
        return registers[param];
    }
    return std::stoi(param);
}

void execute(const std::vector<Instruction>& instructions)
{
    int pos = 0;
    while(pos > -1 && pos < instructions.size())
    {
        Instruction in = instructions[pos];
        switch (in.type)
        {
        case Type::CPY:
        {
            registers[in.param2] = value(in.param1);
            pos++;
            break;
        }
        case Type::INC:
        {
            registers[in.param1]++;
            pos++;
            break;
        }
        case Type::DEC:
        {
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
        }
    }
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> list;
    size_t searchBegin = 0;
    size_t pos = str.find(delim);
    while(true)
    {
        list.push_back(str.substr(searchBegin, pos-searchBegin));

        size_t newPos = str.find(delim, pos+delim.size());
        if(newPos == std::string::npos)
        {
            list.push_back(str.substr(pos+delim.size(), str.size()-(pos+delim.size())));
            break;
        }
        searchBegin = pos+1;
        pos = newPos;
    }
    return list;
}

int main()
{
    std::fstream infile("input.txt");
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
            instructions.push_back(instruction);
        }
    }
#ifdef PART2
    registers["c"] = 1;
#endif
    execute(instructions);
    std::cout<<"Answer: "<<registers["a"]<<std::endl;
    return 0;
}
