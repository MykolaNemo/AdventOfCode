#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <set>

//#define PART1

//std::vector<std::vector<int>> permutations(const std::vector<int>& input)
//{
//    const auto swap = [](int& a, int& b){
//        int t = a;
//        a = b;
//        b = t;
//    };

//    std::vector<std::vector<int>> result;
//    const std::function<void(size_t, std::vector<int>&)> generate =
//    [&generate, &swap, &result](size_t k, std::vector<int>& input){
//        if (k == 1)
//        {
//            result.push_back(input);
//        }
//        else
//        {
//            for (int i = 0; i < k; ++i)
//            {
//                generate(k - 1, input);
//                if (k % 2 == 0)
//                {
//                    swap(input[i], input[k-1]);
//                }
//                else
//                {
//                    swap(input[0], input[k-1]);
//                }
//            }
//        }
//        return std::vector<int>();
//    };

//    std::vector<int> temp = input;
//    generate(input.size(), temp);

//    return result;
//}

//needed: 512kg
//Size: 6!

#ifdef PART1
int numberNeeded = 6;
#else
int numberNeeded = 4;
#endif

std::vector<long long> weights;

long long sum(const std::set<long long>& vect)
{
    long long s = 0;
    for(long long i : vect)
    {
        s += weights[i];
    }
    return s;
}

struct Configuration
{
    std::set<long long> indexes;
    long long entanglement = 1;
};

void findPackages(long long neededWeight)
{
    long long minEntanglement = 0;
    std::vector<Configuration> foundConfigurations;

    std::vector<std::set<long long>> allVariants;
    for(int i = 0; i < weights.size(); ++i)
    {
        allVariants.insert(allVariants.begin(), {i});
    }
    int n = 0;
    while(!allVariants.empty())
    {
        std::set<long long> oldVariant = allVariants.back();
        allVariants.pop_back();
        for(int i = 0; i < weights.size(); ++i)
        {
            n++;
            std::set<long long> newVariant = oldVariant;
            if(std::find(newVariant.begin(), newVariant.end(), i) != newVariant.end()) continue;

            newVariant.insert(i);

            {
                long long s = sum(newVariant);
                int elementsToGo = numberNeeded - newVariant.size();
                int elementIndex = 0;
                while(elementsToGo > 0 && elementIndex < weights.size())
                {
                    if (std::find(newVariant.begin(), newVariant.end(), elementIndex) == newVariant.end())
                    {
                        s += weights[elementIndex];
                        elementsToGo--;
                    }
                    elementIndex++;
                }
                if(s < neededWeight)
                {
                    continue;
                }
            }

            if(!foundConfigurations.empty())
            {
                if(foundConfigurations[0].indexes.size() < newVariant.size())
                {
                    std::cout<<"Already found configuration with less packages -> skip"<<std::endl;
                    break;
                }
                if(foundConfigurations[0].indexes.size() > newVariant.size())
                {
                    std::cout<<"Found configuration that is better than before!"<<std::endl;
                    return;
                }
            }

            int s = sum(newVariant);
            if(s < neededWeight)
            {
                if(std::find(allVariants.begin(), allVariants.end(), newVariant) == allVariants.end())
                {
                    allVariants.insert(allVariants.begin(), newVariant);
                }
            }
            else if(s == neededWeight)
            {
                std::cout<<"Found configuration!!! Size: "<<newVariant.size()<<std::endl;
                Configuration conf;
                conf.indexes = newVariant;
                for(int i : newVariant)
                {
                    conf.entanglement *= weights[i];
                }
                std::cout<<"entanglement: "<<conf.entanglement<<std::endl;
                foundConfigurations.push_back(conf);
                if(minEntanglement == 0)
                {
                    minEntanglement = conf.entanglement;
                }
                else
                {
                    minEntanglement = (conf.entanglement < minEntanglement) ? conf.entanglement : minEntanglement;
                }
            }
            if(n % 50000 == 0)
            {
//                std::cout<<s<<" "<<allVariants.size()<<" "<<n<<" "<<newVariant.size()<<std::endl;
            }
        }
    }
    std::cout<<"Answer: "<<minEntanglement<<std::endl;
}

int main()
{
    {
        std::fstream infile("input.txt");
        std::string weightString;
        while(std::getline(infile, weightString))
        {
            weights.push_back(std::stoi(weightString));
        }
    }
    std::sort(weights.begin(), weights.end(), std::greater<int>());

    long long neededWeight = []()->int{
        int sum = 0;
        for(long long& weight : weights)
        {
            sum += weight;
        }
        std::cout<<sum<<std::endl;
#ifdef PART1
        return sum/3;
#else
        return sum/4;
#endif
    }();
    std::cout<<neededWeight<<std::endl;

    findPackages(neededWeight);

    return 0;
}
