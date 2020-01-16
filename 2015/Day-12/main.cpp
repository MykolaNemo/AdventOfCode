#include <iostream>
#include <fstream>
#include <string>

int part1Sum(const std::string& json)
{
  int sum = 0;
  int pos = 0;
  std::string numString;
  while(pos < json.size())
  {
    if(json[pos]>='0' && json[pos] <='9')
    {
      numString += json[pos];
    }
    else if(json[pos] == '-' && (pos < json.size()-1) && (json[pos+1] >='0' && json[pos+1] <='9'))
    {
      numString += json[pos];
    }
    else
    {
      if(!numString.empty())
      {
        sum += std::stoi(numString);
        numString = "";
      }
    }
    pos++;
  }
  return sum;
}

int part2Sum(std::string json)
{
  int begin = 0;
  size_t redPos = json.substr(begin).find("\"red\"");
  while(redPos != std::string::npos)
  {
    size_t objBegin = std::string::npos;
    {
      int temp = redPos;
      int parenCount = 0;
      while(temp >= 0 && parenCount >= 0)
      {
        if(json[temp] == '{')
        {
          parenCount--;
        }
        else if(json[temp] == '}')
        {
          parenCount++;
        }
        temp--;
      }
      if(parenCount < 0)
      {
        objBegin = temp+1;
      }
    }

    size_t arrayBegin = std::string::npos;
    {
      int temp = redPos;
      int parenCount = 0;
      while(temp >= 0 && parenCount >= 0)
      {
        if(json[temp] == '[')
        {
          parenCount--;
        }
        else if(json[temp] == ']')
        {
          parenCount++;
        }
        temp--;
      }
      if(parenCount < 0)
      {
        arrayBegin = temp+1;
      }
    }

    if(objBegin != std::string::npos && (arrayBegin == std::string::npos || objBegin > arrayBegin))
    {
      int objEnd = redPos;
      {
        int temp = redPos;
        int parenCount = 1;
        while(temp < json.size() && parenCount > 0)
        {
          if(json[temp] == '{')
          {
            parenCount++;
          }
          else if(json[temp] == '}')
          {
            parenCount--;
          }
          temp++;
        }
        objEnd = temp-1;
      }
      json.erase(json.begin() + objBegin, json.begin() + objEnd + 1);
      begin = objBegin;
    }
    else
    {
      begin = redPos + 5;
    }
    redPos = json.substr(begin).find("\"red\"");
    if(redPos != std::string::npos)
    {
      redPos += begin;
    }
  }
  return part1Sum(json);
}

int main()
{
  std::fstream infile("input.txt");
  std::string json;
  std::getline(infile, json);

  int sum1 = part1Sum(json);
  std::cout<<"[Part 1] Sum: "<<sum1<<std::endl;

  int sum2 = part2Sum(json);
  std::cout<<"[Part 2] Sum: "<<sum2<<std::endl;
  return 0;
}
