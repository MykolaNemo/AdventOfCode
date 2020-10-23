#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//hlf r sets register r to half its current value, then continues with the next instruction.
//tpl r sets register r to triple its current value, then continues with the next instruction.
//inc r increments register r, adding 1 to it, then continues with the next instruction.
//jmp offset is a jump; it continues with the instruction offset away relative to itself.
//jie r, offset is like jmp, but only jumps if register r is even ("jump if even").
//jio r, offset is like jmp, but only jumps if register r is 1 ("jump if one", not odd).


void sanitize(std::string& str)
{
    if(str.back() == ',')
    {
        str.erase(str.size()-1);
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

enum class InstructionName
{
    HLF = 0,
    TPL,
    INC,
    JMP,
    JIE,
    JIO
};

struct Instruction
{
    InstructionName name;
    long long param1;
    long long param2;
};

std::vector<long long> regs = {0, 0};

bool execute(const std::vector<Instruction>& program)
{
    if(program.empty())
    {
        std::cout<<"Program is empty"<<std::endl;
        return false;
    }

    int pos = 0;
    while(true)
    {
        if(pos >= program.size())
        {
            return true;
        }

        const Instruction& in = program[pos];
        switch(in.name)
        {
        case InstructionName::HLF:
        {
            if(in.param1 == 0 || in.param1 == 1)
            {
                regs[in.param1] /= 2;
            }
            else
            {
                std::cout<<"hlf: parameter is incorrect ("<<in.param1<<")"<<std::endl;
                return false;
            }
            pos += 1;
            break;
        }
        case InstructionName::TPL:
        {
            if(in.param1 == 0 || in.param1 == 1)
            {
                regs[in.param1] *= 3;
            }
            else
            {
                std::cout<<"tpl: parameter is incorrect ("<<in.param1<<")"<<std::endl;
                return false;
            }
            pos += 1;
            break;
        }
        case InstructionName::INC:
        {
            if(in.param1 == 0 || in.param1 == 1)
            {
                regs[in.param1] += 1;
            }
            else
            {
                std::cout<<"inc: parameter is incorrect ("<<in.param1<<")"<<std::endl;
                return false;
            }
            pos += 1;
            break;
        }
        case InstructionName::JMP:
        {
            pos += in.param1;
            break;
        }
        case InstructionName::JIE:
        {
            bool jump = true;
            if(in.param1 == 0 || in.param1 == 1)
            {
                jump &= (regs[in.param1] % 2 == 0);
            }
            else
            {
                std::cout<<"jie: parameter 1 is incorrect ("<<in.param1<<")"<<std::endl;
                return false;
            }
            if(jump)
            {
                pos += in.param2;
            }
            else
            {
                pos += 1;
            }
            break;
        }
        case InstructionName::JIO:
        {
            bool jump = true;
            if(in.param1 == 0 || in.param1 == 1)
            {
                jump &= (regs[in.param1] == 1);
            }
            else
            {
                std::cout<<"jio: parameter 1 is incorrect ("<<in.param1<<")"<<std::endl;
                return false;
            }
            if(jump)
            {
                pos += in.param2;
            }
            else
            {
                pos += 1;
            }
            break;
        }
        default:
        {
            std::cout<<"instruction is incorrect: "<<static_cast<int>(in.name)<<std::endl;
            return false;
        }
        }
    }
    return true;
}

int main()
{
    std::vector<Instruction> program;
    {
      std::fstream infile("input.txt");
      std::string instructionString;
      while(std::getline(infile, instructionString))
      {
        std::vector<std::string> parts = split(instructionString, " ");

        Instruction instruction;
        if(parts[0] == "hlf")
        {instruction.name = InstructionName::HLF;}
        else if(parts[0] == "tpl")
        {instruction.name = InstructionName::TPL;}
        else if(parts[0] == "inc")
        {instruction.name = InstructionName::INC;}
        else if(parts[0] == "jmp")
        {instruction.name = InstructionName::JMP;}
        else if(parts[0] == "jie")
        {instruction.name = InstructionName::JIE;}
        else if(parts[0] == "jio")
        {instruction.name = InstructionName::JIO;}

        sanitize(parts[1]);
        if(parts[1] == "a")
        {
            instruction.param1 = 0;
        }
        else if (parts[1] == "b")
        {
            instruction.param1 = 1;
        }
        else
        {
            instruction.param1 = std::stoll(parts[1]);
        }

        if(parts.size() == 3)
        {
            instruction.param2 = std::stoll(parts[2]);
        }
        program.push_back(instruction);
      }
    }

    regs[0] = 0;
    regs[1] = 0;
    if(execute(program))
    {
        std::cout<<"[Part 1] Answer: "<<regs[1]<<std::endl;
    }
    else
    {
        std::cout<<"Can not execute program"<<std::endl;
    }

    regs[0] = 1;
    regs[1] = 0;
    if(execute(program))
    {
        std::cout<<"[Part 2] Answer: "<<regs[1]<<std::endl;
    }
    else
    {
        std::cout<<"Can not execute program"<<std::endl;
    }
    return 0;
}
