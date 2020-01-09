#include <iostream>
#include <string>

//#define PART1

bool checkDigitsRules(const std::string& number)
{
  bool adjacentOk = false;
  for(int i = 0; i < number.size()-1; ++i)
  {
    //check non-decreasing
    if(number[i] > number[i+1])
    {
      return false;
    }

    //check adjacent digits
    if(!adjacentOk && (number[i] == number[i+1]))
    {
#ifndef PART1
      if(((i > 0)               && (number[i-1] == number[i]  )) ||
         ((i < number.size()-2) && (number[i+1] == number[i+2])))
      {
        continue;
      }
#endif
      adjacentOk = true;
    }
  }
  return adjacentOk;
}

int main()
{
  int result = 0;
  for(int i = 125730; i <= 579381; ++i)//range from input
  {
    if(checkDigitsRules(std::to_string(i)))
    {
      result++;
    }
  }

#ifdef PART1
  std::cout<<"[Part 1] Answer: "<<result<<std::endl;
#else
  std::cout<<"[Part 2] Answer: "<<result<<std::endl;
#endif
  return 0;
}
