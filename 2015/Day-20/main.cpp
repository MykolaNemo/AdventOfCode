#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

//#define PART1

#ifdef PART1
std::vector<long long> findDivisors(long long num)
{
    std::vector<long long> v;
    long long limit = num;
    for(long long i = 1L; i <= limit; ++i)
    {
        if(num % i == 0L)
        {
            v.push_back(i);

            long long anotherOne = num/i;
            if(anotherOne > i)
            {
                v.push_back(anotherOne);
                limit = anotherOne-1L;
            }
        }
    }
    return v;
}
#else

std::map<int, int> numberOfHousesVisited;

std::vector<long long> findDivisors(long long num)
{
    std::vector<long long> v;
    long long limit = num;
    for(long long div = 1L; div <= limit; ++div)
    {
        if(num % div == 0L)
        {
            if(numberOfHousesVisited[div] < 50)
            {
                numberOfHousesVisited[div]++;
                v.push_back(div);
            }

            long long anotherDiv = num/div;
            if(anotherDiv > div)
            {
                if(numberOfHousesVisited[anotherDiv] < 50)
                {
                    numberOfHousesVisited[anotherDiv]++;
                    v.push_back(anotherDiv);
                }
                limit = anotherDiv-1L;
            }
        }
    }
    return v;
}
#endif

int main()
{
    long long numberOfPresents = 34000000; // AoC input

#ifdef PART1
    for(int i = 700000; i < 800000; ++i)
    {
        long long sum = 0;
        for(auto& divisor : findDivisors(i))
        {
            sum += divisor;
        }
        std::cout<<i<<", "<<sum<<std::endl;
        if(sum*10 >= numberOfPresents)
        {
            std::cout<<"[Part 1] Result: "<<i<<std::endl;
            //Answer: 786240
            return 0;
        }
    }
#else
    for(int i = 1; i <= 1000000; ++i)
    {
        long long sum = 0;
        for(auto& divisor : findDivisors(i))
        {
            sum += divisor*11;
        }
        if(i % 10000 == 0)
        {
            std::cout<<i<<", "<<sum<<std::endl;
        }
        if(sum >= numberOfPresents)
        {
            std::cout<<"[Part 2] Result: "<<i<<std::endl;
            //Answer:
            return 0;
        }
    }
    for(auto& [key, value] : numberOfHousesVisited)
    {
        if(value < 50)
        {
            std::cout<<key<<" still works, "<<value<<std::endl;
            break;
        }
    }
    std::cout.flush();
#endif
    return 0;
}
