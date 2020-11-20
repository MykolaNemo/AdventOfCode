#include <iostream>
#include <map>
#include "../../utils.h"

std::map<std::string, int> registers;

enum class Operation
{
    Dec,
    Inc
};

enum class ConditionType
{
    Greater,
    Less,
    Equal,
    GreaterEqual,
    LessEqual,
    NotEqual
};

struct Operation
{
    std::string regName;
    Operation operation;
    int operationValue;

    ConditionType conditionType;
    std::string conditionRegister;
    int conditionValue;
};

std::vector<Operation> instructions;

int main()
{
    auto content = readFile("input.txt");
    for(auto str : content)
    {
        //riz dec -780 if gm != -1041
        std::vector<std::string> words = split(str, " ");
        Operation in;
        in.regName = words[0];
        in.operation = (words[1] == "dec") ? Operation::Dec : Operation::Inc;
        in.operationValue = std::stoi(words[2]);
        in.conditionRegister = words[4];
        in.conditionValue = std::stoi(words[6]);

        std::string conditionTypeStr = words[5];
        if(conditionTypeStr == ">")
        {
            in.conditionType = ConditionType::Greater;
        }
        else if(conditionTypeStr == "<")
        {
            in.conditionType = ConditionType::Less;
        }
        else if(conditionTypeStr == "==")
        {
            in.conditionType = ConditionType::Equal;
        }
        else if(conditionTypeStr == ">=")
        {
            in.conditionType = ConditionType::GreaterEqual;
        }
        else if(conditionTypeStr == "<=")
        {
            in.conditionType = ConditionType::LessEqual;
        }
        else if(conditionTypeStr == "!=")
        {
            in.conditionType = ConditionType::NotEqual;
        }
        else
        {
            std::cout<<"unknown condition type: "<<conditionTypeStr<<std::endl;
        }
        instructions.push_back(in);
    }

    int maxIntermediateValue = 0;
    for(const auto& in : instructions)
    {
        bool execute = false;
        ConditionType condType = in.conditionType;
        switch (condType)
        {
        case ConditionType::Greater:
            execute = (registers[in.conditionRegister] > in.conditionValue);
            break;
        case ConditionType::Less:
            execute = (registers[in.conditionRegister] < in.conditionValue);
            break;
        case ConditionType::Equal:
            execute = (registers[in.conditionRegister] == in.conditionValue);
            break;
        case ConditionType::GreaterEqual:
            execute = (registers[in.conditionRegister] >= in.conditionValue);
            break;
        case ConditionType::LessEqual:
            execute = (registers[in.conditionRegister] <= in.conditionValue);
            break;
        case ConditionType::NotEqual:
            execute = (registers[in.conditionRegister] != in.conditionValue);
            break;
        default:
            break;
        }
        if(execute)
        {
            switch (in.operation) {
            case Operation::Dec:
                registers[in.regName] -= in.operationValue;
                break;
            case Operation::Inc:
                registers[in.regName] += in.operationValue;
                break;
            default:
                break;
            }
            if(registers[in.regName] > maxIntermediateValue)
            {
                maxIntermediateValue = registers[in.regName];
            }
        }
    }
    int maxValue = registers.begin()->second;
    for(const auto& regPair : registers)
    {
        if(regPair.second > maxValue)
        {
            maxValue = regPair.second;
        }
    }
    std::cout<<"[Part 1] Answer: "<<maxValue<<std::endl;
    std::cout<<"[Part 2] Answer: "<<maxIntermediateValue<<std::endl;
    return 0;
}
