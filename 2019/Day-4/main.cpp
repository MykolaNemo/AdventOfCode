#include <iostream>

using namespace std;

bool checkAdjustentDigits(std::string number)
{
  for(int i = 0; i < number.size()-1; ++i)
  {
    if(number[i] == number[i+1])
    {
      bool ok = true;
      if(i > 0)
      {
        if(number[i-1] != number[i])
        {
          ok &= true;
        }
        else
        {
          continue;
        }
      }
      if(ok && (i<number.size()-2))
      {
        if(number[i+1] != number[i+2])
        {
          ok &= true;
        }
        else
        {
          continue;
        }
      }
      if(ok) return true;
    }
  }
  return false;
}

bool checkNeverDecrease(std::string number)
{
  for(int i = 0; i < number.size()-1; ++i)
  {
    std::string s1;
    s1+= number[i];

    std::string s2;
    s2 += number[i+1];

    if(std::stoi(s1) > std::stoi(s2))
    {
      return false;
    }
  }
  return true;
}

int main()
{
  int result = 0;
//  for(int i = 111122; i <= 111122; ++i)
  for(int i = 125730; i <= 579381; ++i)
  {
    bool ok = true;

    ok &= checkAdjustentDigits(std::to_string(i));
    if(!ok) continue;

    ok &= checkNeverDecrease(std::to_string(i));

    if(ok) result++;
  }
  std::cout<<result<<std::endl;
  return 0;
}
