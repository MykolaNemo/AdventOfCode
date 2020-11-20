#include <iostream>
#include <bitset>
#include <vector>

int main()
{
    long long startA = 591; //input
    long long startB = 393; //input
    long long factorA = 16807;
    long long factorB = 48271;
    long long div = 2147483647;

    {
        //Part 1
        auto a = startA;
        auto b = startB;
        int n = 0;
        for(int i = 0; i < 40000000; ++i)
        {
            a = a*factorA % div;
            b = b*factorB % div;
            if(std::bitset<16>(a) == std::bitset<16>(b))
            {
                n++;
            }
        }
        std::cout<<"[Part 1] Answer: "<<n<<std::endl;
    }

    {
        //Part 2
        std::vector<std::bitset<16>> aValues;
        std::vector<std::bitset<16>> bValues;
        auto a = startA;
        auto b = startB;
        for(long long i = 0; (aValues.size() < 5000000) || (bValues.size() < 5000000); ++i)
        {
            a = a*factorA % div;
            b = b*factorB % div;

            if((a % 4 == 0) && aValues.size() < 5000000)
            {
                aValues.push_back(std::bitset<16>(a));
            }
            if((b % 8 == 0) && bValues.size() < 5000000)
            {
                bValues.push_back(std::bitset<16>(b));
            }
        }
        int n = 0;
        for(int i = 0; i < 5000000; ++i)
        {
            n += (aValues[i] == bValues[i]);
        }
        std::cout<<"[Part 2] Answer: "<<n<<std::endl;
    }
    return 0;
}
