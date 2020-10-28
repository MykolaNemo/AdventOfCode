#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

struct IP7
{
    std::string address;
    bool isTLS()
    {
        std::string a = address;
        bool isNotHyperPartGood = false;
        bool isHyperPartGood = true;
        while(!a.empty())
        {
            auto hyperPartStart = a.find_first_of('[');
            auto hyperPartEnd = a.find_first_of(']');
            if(!isNotHyperPartGood)
            {
                std::string notHyperPart;
                if(hyperPartStart == std::string::npos)
                {
                    notHyperPart = a;
                }
                else
                {
                    notHyperPart = a.substr(0, hyperPartStart);
                }
                for(int i = 0; i < notHyperPart.size()-3; ++i)
                {
                    if(notHyperPart[i] != notHyperPart[i+1] &&
                            notHyperPart[i] == notHyperPart[i+3] &&
                            notHyperPart[i+1] == notHyperPart[i+2])
                    {
                        isNotHyperPartGood = true;
                        break;
                    }
                }
            }
            if(hyperPartStart != std::string::npos && hyperPartEnd != std::string::npos)
            {
                std::string hyperPart = a.substr(hyperPartStart+1, hyperPartEnd - hyperPartStart - 1);
                if(isHyperPartGood)
                {
                    for(int i = 0; i < hyperPart.size()-3; ++i)
                    {
                        if(hyperPart[i] != hyperPart[i+1] &&
                                hyperPart[i] == hyperPart[i+3] &&
                                hyperPart[i+1] == hyperPart[i+2])
                        {
                            isHyperPartGood = false;
                            break;
                        }
                    }
                }
            }

            if(!isHyperPartGood)
            {
                return false;
            }

            if(hyperPartEnd < a.size() - 1)
            {
                a = a.substr(hyperPartEnd+1, a.size() - hyperPartEnd - 1);
            }
            else
            {
                break;
            }
        }
        return isNotHyperPartGood;
    }

    bool isSSL()
    {
        std::string a = address;
        std::vector<std::string> hyperPartsVector;
        std::vector<std::string> abaVector;
        bool isHyperPartGood = false;
        while(!a.empty())
        {
            auto hyperPartStart = a.find_first_of('[');
            auto hyperPartEnd = a.find_first_of(']');
            {
                std::string notHyperPart;
                if(hyperPartStart == std::string::npos)
                {
                    notHyperPart = a;
                }
                else
                {
                    notHyperPart = a.substr(0, hyperPartStart);
                }
                for(int i = 0; i < notHyperPart.size()-2; ++i)
                {
                    if(notHyperPart[i] != notHyperPart[i+1] &&
                            notHyperPart[i] == notHyperPart[i+2])
                    {
                        std::string aba;
                        aba += notHyperPart[i];
                        aba += notHyperPart[i+1];
                        aba += notHyperPart[i+2];
                        abaVector.push_back(aba);
                    }
                }
            }
            if(hyperPartStart != std::string::npos && hyperPartEnd != std::string::npos)
            {
                std::string hyperPart = a.substr(hyperPartStart+1, hyperPartEnd - hyperPartStart - 1);
                hyperPartsVector.push_back(hyperPart);
            }
            if(!isHyperPartGood)
            {
                for(const auto& hyperPart : hyperPartsVector)
                {
                    for(int i = 0; i < hyperPart.size()-2 && !isHyperPartGood; ++i)
                    {
                        for(const auto& aba : abaVector)
                        {
                            if(hyperPart[i] == aba[1] &&
                                    hyperPart[i+1] == aba[0] &&
                                    hyperPart[i+2] == aba[1])
                            {
                                isHyperPartGood = true;
                                break;
                            }
                        }
                    }
                    if(isHyperPartGood == true)
                    {
                        break;
                    }
                }
            }

            if(!abaVector.empty() && isHyperPartGood)
            {
                return true;
            }

            if(hyperPartEnd < a.size() - 1)
            {
                a = a.substr(hyperPartEnd+1, a.size() - hyperPartEnd - 1);
            }
            else
            {
                break;
            }
        }
        return (!abaVector.empty() && isHyperPartGood);
    }
};

int main()
{
    std::fstream infile("input.txt");
    std::vector<IP7> ips;
    {
        std::string ip;
        while(std::getline(infile, ip))
        {
            IP7 ip7;
            ip7.address = ip;
            ips.push_back(ip7);
        }
    }
    int n1 = 0;
    int n2 = 0;
    for(auto& ip7 : ips)
    {
        if(ip7.isTLS())
        {
            n1++;
        }
        if(ip7.isSSL())
        {
            n2++;
        }
    }
    std::cout<<"[Part1] Answer: "<<n1<<std::endl;
    std::cout<<"[Part2] Answer: "<<n2<<std::endl;
    return 0;
}
