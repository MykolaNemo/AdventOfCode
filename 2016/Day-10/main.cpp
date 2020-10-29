#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

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

//Examples
//bot 26 gives low to bot 131 and high to bot 149
//value 61 goes to bot 119

int value1 = 61;
int value2 = 17;


struct Instruction
{
    int lowValueBot = -1;
    int highValueBot = -1;
    int lowOutput = -1;
    int highOutput = -1;
};

struct Bot
{
    int number = -1;
    int chip1 = -1;
    int chip2 = -1;
    Instruction in;
};

std::vector<Bot*> bots;
std::map<int, int> outputs;

int main()
{
    std::fstream infile("input.txt");
    std::vector<std::string> instructions;
    {
        std::string instruction;
        while(std::getline(infile, instruction))
        {
            instructions.push_back(instruction);
        }
    }

    for(const auto& in : instructions)
    {
        std::vector<std::string> words = split(in, " ");
        if(words.size() == 6)//value x goes to bot y
        {
            int chip = std::stoi(words[1]);
            int botNum = std::stoi(words[5]);
            auto botIt = std::find_if(bots.begin(), bots.end(), [&botNum](Bot* b)->bool{
                return (b->number == botNum);
            });
            if(botIt == bots.end())
            {
                Bot* bot = new Bot();
                bot->number = botNum;
                bot->chip1 = chip;
                bots.push_back(bot);
            }
            else
            {
                if((*botIt)->chip1 == -1)
                {
                    (*botIt)->chip1 = chip;
                }
                else if((*botIt)->chip2 == -1)
                {
                    (*botIt)->chip2 = chip;
                }
                else
                {
                    std::cout<<"Bot has too much chips! "<<botNum<<std::endl;
                    return 0;
                }
            }
        }

        //bot x gives low to output y and high to bot z
        //bot x gives low to bot y and high to bot z
        else if(words.size() == 12)
        {
            int botNum = std::stoi(words[1]);
            int lowValueDest = std::stoi(words[6]);
            int highValueDest = std::stoi(words[11]);
            auto botIt = std::find_if(bots.begin(), bots.end(), [&botNum](Bot* b)->bool{
                return (b->number == botNum);
            });
            if(botIt == bots.end())
            {
                Bot* bot = new Bot();
                bot->number = botNum;
                if(words[5] == "bot")
                {
                    bot->in.lowValueBot = lowValueDest;
                }
                else if(words[5] == "output")
                {
                    bot->in.lowOutput = lowValueDest;
                }

                if(words[10] == "bot")
                {
                    bot->in.highValueBot = highValueDest;
                }
                else if(words[10] == "output")
                {
                    bot->in.highOutput = highValueDest;
                }
                bots.push_back(bot);
            }
            else
            {
                if(words[5] == "bot")
                {
                    (*botIt)->in.lowValueBot = lowValueDest;
                }
                else if(words[5] == "output")
                {
                    (*botIt)->in.lowOutput = lowValueDest;
                }

                if(words[10] == "bot")
                {
                    (*botIt)->in.highValueBot = highValueDest;
                }
                else if(words[10] == "output")
                {
                    (*botIt)->in.highOutput = highValueDest;
                }
            }
        }
        else
        {
            std::cout<<"Unknown command"<<std::endl;
            return 0;
        }
    }

    auto currentBotIt = std::find_if(bots.begin(), bots.end(), [](Bot* bot)->bool{
        return (bot->chip1 != -1 && bot->chip2 != -1);
    });
    std::vector<Bot*> botsInMovement;
    botsInMovement.push_back(*currentBotIt);
    while(!botsInMovement.empty())
    {
        Bot* currentBot = botsInMovement.back();
        if((currentBot->chip1 == value1 && currentBot->chip2 == value2) ||
                (currentBot->chip1 == value2 && currentBot->chip2 == value1))
        {
            std::cout<<"[Part 1] Answer: "<<currentBot->number<<std::endl;
        }
        botsInMovement.pop_back();
        std::vector<Bot*>::iterator lowValueBotIt = bots.end();
        std::vector<Bot*>::iterator highValueBotIt = bots.end();
        if(currentBot->in.lowValueBot != -1)
        {
            lowValueBotIt = std::find_if(bots.begin(), bots.end(), [&currentBot](Bot* b)->bool{
                    return (b->number == currentBot->in.lowValueBot);
            });
        }
        if(currentBot->in.highValueBot != -1)
        {
            highValueBotIt = std::find_if(bots.begin(), bots.end(), [&currentBot](Bot* b)->bool{
                return (b->number == currentBot->in.highValueBot);
            });
        }
        if(currentBot->in.lowOutput != -1)
        {
            int low = std::min(currentBot->chip1, currentBot->chip2);
            outputs[currentBot->in.lowOutput] = low;
        }
        if(currentBot->in.highOutput != -1)
        {
            int high = std::max(currentBot->chip1, currentBot->chip2);
            outputs[currentBot->in.highOutput] = high;
        }

        if(lowValueBotIt != bots.end())
        {
            if((*lowValueBotIt)->chip1 != -1 && (*lowValueBotIt)->chip2 != -1)
            {
                botsInMovement.insert(botsInMovement.begin(), currentBot);
                continue;
            }

            int low = std::min(currentBot->chip1, currentBot->chip2);
            if((*lowValueBotIt)->chip1 == -1)
            {
                (*lowValueBotIt)->chip1 = low;
            }
            else if((*lowValueBotIt)->chip2 == -1)
            {
                (*lowValueBotIt)->chip2 = low;
            }
            else
            {
                std::cout<<"Bot has too much chips (low)! "<<(*lowValueBotIt)->number<<std::endl;
                return 0;
            }
        }
        if(highValueBotIt != bots.end())
        {
            if((*highValueBotIt)->chip1 != -1 && (*highValueBotIt)->chip2 != -1)
            {
                botsInMovement.insert(botsInMovement.begin(), currentBot);
                continue;
            }

            int high = std::max(currentBot->chip1, currentBot->chip2);
            if((*highValueBotIt)->chip1 == -1)
            {
                (*highValueBotIt)->chip1 = high;
            }
            else if((*highValueBotIt)->chip2 == -1)
            {
                (*highValueBotIt)->chip2 = high;
            }
            else
            {
                std::cout<<"Bot has too much chips! (high) "<<(*highValueBotIt)->number<<std::endl;
                return 0;
            }
        }

        currentBot->chip1 = -1;
        currentBot->chip2 = -1;
        for(Bot* bot : bots)
        {
            auto it = std::find_if(botsInMovement.begin(), botsInMovement.end(), [&bot](Bot* b)->bool{
                    return (b->number == bot->number);
            });
            if(it == botsInMovement.end() && bot->chip1 != -1 && bot->chip2 != -1)
            {
                botsInMovement.insert(botsInMovement.begin(), bot);
            }
        }
//        std::cout<<"Bot in movement: "<<botsInMovement.size()<<std::endl;
    }
    std::cout<<"[Part 2] Answer: "<<outputs[0] * outputs[1] * outputs[2]<<std::endl;
    return 0;
}
