#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <list>

using namespace std;

std::map<std::string, long long> freeAmount;
std::vector<std::map<std::string, long long> > amounts;

struct Chemical
{
    Chemical(){}
    Chemical(std::string _name, long long _amount):name(std::move(_name)), amount(_amount){}
    long long amount = 0;
//    int freeAmount = 0;
    std::string name;
};

inline bool operator==(const Chemical& r, const Chemical& l)
{
    return r.name == l.name;
}

struct Reaction
{
  std::list<Chemical> ingridients;
  Chemical result;
};

int main()
{
    std::ifstream infile("input.txt");

    std::list<Reaction> allReactions;
    std::string row;
    while(std::getline(infile, row))
    {
        size_t delim = row.find("=>");
        std::string ingridientsStringList = row.substr(0,delim-1);
        std::string resultString = row.substr(delim+3, row.size()-delim+2);

        std::vector<std::string> ingridients;
//        std::cout<<ingridientsStringList<<"   ___   "<<resultString<<std::endl;
        delim = ingridientsStringList.find(',');
        while(delim != std::string::npos)
        {
            ingridients.push_back(ingridientsStringList.substr(0, delim));
            ingridientsStringList = ingridientsStringList.substr(delim+2, ingridientsStringList.size() - delim - 2);
            delim = ingridientsStringList.find(',');
        }
        ingridients.push_back(ingridientsStringList);


        Reaction r;
        for(const auto &ing : ingridients)
        {
            delim = ing.find(" ");
            Chemical ingChemical;
            ingChemical.amount = std::stoi(ing.substr(0, delim));
            ingChemical.name = ing.substr(delim+1, ing.size() - delim-1);
//            std::cout<<ingChemical.name<<": "<<ingChemical.amount<<"\n";
            r.ingridients.push_back(ingChemical);
        }

        delim = resultString.find(" ");
        Chemical resultChemical;
        resultChemical.amount = std::stoi(resultString.substr(0, delim));
        resultChemical.name = resultString.substr(delim+1, resultString.size() - delim-1);
        r.result = resultChemical;
//        std::cout<<resultChemical.name<<": "<<resultChemical.amount<<"\n";
        allReactions.push_back(r);
    }


    for(Reaction& r : allReactions)
    {
        std::cout<<r.result.name<<": "<<r.result.amount<<"<< \n";
        for(auto &i : r.ingridients)
        {
            freeAmount[i.name] = 0;
            std::cout<<i.name<<": "<<i.amount<<"\n";
        }
    }
    std::cout<<"============"<<std::endl;

    auto findReactionByName = [](std::string&& name, std::list<Reaction>& reactions)->Reaction&{
        auto it = std::find_if(reactions.begin(), reactions.end(), [&name](const Reaction& reaction)->bool{
            return (reaction.result.name == name);
        });
        if(it != reactions.end())
        {
            return *it;
        }
        std::cout<<"findReactionByName ERROR"<<std::endl;
        throw std::exception();
    };

    amounts.push_back(freeAmount);
    long long ORE = 1000000000000ll;
    long long FUEL = 0ll;
    std::cout<<"Start "<<ORE<<std::endl;
    while(ORE > 0ll)
    {
        auto allReactionsCopy = allReactions;
        Reaction &fuelReaction = findReactionByName(std::move(std::string("FUEL")), allReactionsCopy);
        std::list<Chemical>& fuelIngridients = fuelReaction.ingridients;
//        for(auto &i : fuelIngridients)
//        {
//            i.amount*=4436981;
//        }
        auto currentFuelIngridientIt = fuelIngridients.begin();
        while(currentFuelIngridientIt != fuelIngridients.end())
        {
            if(currentFuelIngridientIt->name != "ORE")
            {
//                for(auto &i : fuelReaction.ingridients)
//                {
//                    std::cout<<i.name<<": "<<i.amount<<"\n";
//                }
//                std::cout<<"====================="<<std::endl;
                Reaction& ingridientReaction = findReactionByName(std::move(std::string(currentFuelIngridientIt->name)),allReactionsCopy);

                auto& freeAmountRef = freeAmount.at(currentFuelIngridientIt->name);
                decltype (ingridientReaction.result.amount) amountNeeded = 0;
                if(freeAmountRef >= currentFuelIngridientIt->amount)
                {
                    freeAmountRef -= currentFuelIngridientIt->amount;
                }
                else
                {
                    amountNeeded = currentFuelIngridientIt->amount - freeAmountRef;
                    freeAmountRef = 0;
                }

                auto& amountInReaction = ingridientReaction.result.amount;
                auto reactionsCount = amountNeeded/amountInReaction;
                if(amountNeeded % amountInReaction != 0)
                {
                    ++reactionsCount;
                }
                freeAmountRef = reactionsCount*amountInReaction - amountNeeded;
                currentFuelIngridientIt = fuelIngridients.erase(currentFuelIngridientIt);
//                for(auto &i : fuelReaction.ingridients)
//                {
//                    std::cout<<i.name<<": "<<i.amount<<"\n";
//                }
//                std::cout<<"====================="<<std::endl;

                for(const auto &ing : ingridientReaction.ingridients)
                {
                    auto it = std::find(fuelIngridients.begin(), fuelIngridients.end(), ing);
                    if(it != fuelIngridients.end())
                    {
                        it->amount += ing.amount*reactionsCount;
                    }
                    else
                    {
                        fuelIngridients.push_back(std::move(Chemical(ing.name, (ing.amount)*reactionsCount)));
                    }
                }
//                for(auto &i : fuelReaction.ingridients)
//                {
//                    std::cout<<i.name<<": "<<i.amount<<"\n";
//                }
//                std::cout<<"====================="<<std::endl;
            }
            else
            {
                currentFuelIngridientIt++;
            }
        }

        auto result = ORE - fuelIngridients.front().amount;
        if(result >= 0)
        {
            ORE = result;
            FUEL++;
            if(FUEL % 10000 == 0)
            {
                std::cout<<"ORE: "<<ORE<<"\n";
                std::cout.flush();
            }
        }
        else
        {
            std::cout<<"ORE: "<<ORE<<"\n";
            break;
        }
    }
    std::cout<<"FUEL: "<<FUEL<<std::endl;

    return 0;
}
