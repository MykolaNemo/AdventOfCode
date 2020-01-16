#include <iostream>
#include <algorithm>

#define FOR(i,n1,n2) for(int i = n1; i <= n2; ++i)

std::string lookAndSay(const std::string& number)
{
  std::string result;
  int startPos = 0;
  char c = number[startPos];
  int count = 1;
  int endPos = 1;
  while(true)
  {
    if(number[endPos] == c)
    {
      count++;
    }
    else
    {
      result.append(std::to_string(count) + c);
      startPos = endPos;
      c = number[startPos];
      count = 1;
    }
    endPos++;
    if(endPos >= number.size())
    {
      result.append(std::to_string(count) + c);
      break;
    }
  }
  return result;
}

int main()
{
  std::string input = "1113122113";//AoC input
  FOR(i,1,50)
  {
    input = lookAndSay(input);
    if(i == 40)
    {
      std::cout<<"[Part 1] Length: "<<input.size()<<std::endl;
    }
  }
  std::cout<<"[Part 2] Length: "<<input.size()<<std::endl;
  return 0;
}
