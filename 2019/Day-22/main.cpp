#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "BigInt.h"

//#define PART1

#ifdef PART1
bigint cardPosition = 0; //?
bigint cardNumber = 2019;
const bigint deckSize = 10007;
const bigint shuffleNumber = 1;

#else
bigint cardPosition = 2020;
bigint cardNumber = 0; //?
const bigint deckSize = 119315717514047ll;
bigint shuffleNumber = 101741582076661ll;

#endif

//linear congruential function
struct LCF
{
    LCF(){}
    LCF(bigint _a, bigint _b):a(_a), b(_b){}
    bigint a = 1;
    bigint b = 0;
};

LCF lcf_merge(const LCF& f1, const LCF& f2, bigint mod)
{
//    a*x + b % mod              <- f1
//    c*x + d % mod              <- f2
//    c*(a*x + b) + d % mod
//    a*c*x + c*b + d % mod
//    (a*c)*x + (c*b + d) % mod
//    (a*c)%mod, (c*b + d) % mod <- result
    return LCF((f1.a*f2.a)%mod, (f2.a*f1.b + f2.b)%mod);
}

inline LCF lcf_dealIntoNewStack()
{
    return LCF(-1, -1);
}

inline LCF lcf_cut(bigint N)
{
    return LCF(1, -N);
}

inline LCF lcf_dealWithIncrement(bigint N)
{
    return LCF(N, 0);
}

bigint lcf_apply(bigint& N, LCF lcf, bigint mod)
{
    return (lcf.a*N + lcf.b) % mod;
}

enum ShuffleType
{
    Nothing = 0,
    New,
    Cut,
    Increment
};

struct Shuffle
{
    ShuffleType type;
    bigint N;
};

LCF lcf_pow(LCF lcf, bigint p, bigint mod)
{
    if (p == 0) return LCF();

    if (p % 2 == 0)
    {
        LCF t = lcf_pow(lcf, p/2, mod);
        return lcf_merge(t, t, mod);
    }
    else
    {
        LCF t = lcf_pow(lcf, (p-1)/2, mod);
        return lcf_merge(lcf, lcf_merge(t, t, mod), mod);
    }
}

bigint int_pow(bigint x, bigint p, bigint mod)
{
    if (p == 0) return bigint(1);

    if (p % bigint(2) == 0)
    {
        bigint t = int_pow(x, p/bigint(2), mod);
        return (t * t) % mod;
    }
    else
    {
        bigint t = int_pow(x, (p-1)/bigint(2), mod);
        return (t * t * x) % mod;
    }
}


int main()
{
    std::ifstream infile("input.txt");

    std::vector<std::string> instructions;
    std::string instruction;
    while(std::getline(infile, instruction))
    {
        instructions.push_back(instruction);
    }

    std::vector<Shuffle> shuffles;

    for(auto inst : instructions)
    {
        size_t pos1 = inst.find_first_of(' ');
        size_t pos2 = inst.find_last_of(' ');
        std::string firstWord = inst.substr(0, pos1);
        std::string lastWord = inst.substr(pos2+1, inst.size() - pos2 - 1);
        if(firstWord == "cut")
        {
            shuffles.push_back(Shuffle{Cut, stoi(lastWord)});
        }
        else if(firstWord == "deal")
        {
            if(lastWord == "stack")
            {
                shuffles.push_back(Shuffle{New, 0});
            }
            else
            {
                shuffles.push_back(Shuffle{Increment, stoi(lastWord)});
            }
        }
    }

    LCF totalLCF;
    for(auto& shuffle : shuffles)
    {
        switch (shuffle.type)
        {
        case New:
        {
            totalLCF = lcf_merge(totalLCF, lcf_dealIntoNewStack(), deckSize);
            break;
        }
        case Cut:
        {
            totalLCF = lcf_merge(totalLCF, lcf_cut(shuffle.N), deckSize);
            break;
        }
        case Increment:
        {
            totalLCF = lcf_merge(totalLCF, lcf_dealWithIncrement(shuffle.N), deckSize);
            break;
        }
        default:
            break;
        }
    }

    totalLCF = lcf_pow(totalLCF, shuffleNumber, deckSize);

#ifdef PART1
    cardPosition = lcf_apply(cardNumber, totalLCF, deckSize);
    std::cout<<"Card #"<<cardNumber<<" position: "<<cardPosition<<std::endl;
#else
    cardNumber = (cardPosition - totalLCF.b) * int_pow(totalLCF.a, deckSize-2, deckSize) % deckSize;
    std::cout<<"Card on "<<cardPosition<<" position: "<<cardNumber<<std::endl;
#endif

    return 0;
}