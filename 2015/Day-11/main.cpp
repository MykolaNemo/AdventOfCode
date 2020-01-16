#include <iostream>

using namespace std;

void next(std::string& str)
{
  int pos = str.size()-1;
  while(str[pos] == 'z')
  {
    str[pos] = 'a';
    pos--;
  }
  if(pos >= 0)
  {
    str[pos]++;
  }
}

bool checkPass(const std::string& pass)
{
  bool threeInRow = false;
  std::string fordiddenLetters = "iol";
  bool twoPairs = false;
  int pos = 0;
  while(pos < pass.size())
  {
    if(fordiddenLetters.find(pass[pos]) != std::string::npos)
    {
      return false;
    }
    if(!threeInRow && (pos <= pass.size()-3))
    {
      if(pass[pos] + 1 == pass[pos+1] &&
         pass[pos] + 2 == pass[pos+2])
      {
        threeInRow = true;
      }
    }

    if(!twoPairs && (pos <= pass.size()-4))
    {
      if(pass[pos] == pass[pos+1])
      {
        for(int j = pos+2; j < pass.size(); ++j)
        {
          if(pass[j] == pass[j+1])
          {
            twoPairs = true;
            break;
          }
        }
      }
    }
    pos++;
  }
  return threeInRow && twoPairs;
}

int main()
{
  std::string input("vzbxkghb");// AoC input
  while(!checkPass(input))
  {
    next(input);
  }
  std::cout<<"[Part 1] "<<input<<std::endl;
  next(input);
  while(!checkPass(input))
  {
    next(input);
  }
  std::cout<<"[Part 2] "<<input<<std::endl;
  return 0;
}
