#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <functional>

//#define PART1

std::vector<std::vector<std::string>> permutations(const std::vector<std::string>& input)
{
    const auto swap = [](std::string& a, std::string& b){
        auto t = a;
        a = b;
        b = t;
    };

    std::vector<std::vector<std::string>> result;
    const std::function<void(size_t, std::vector<std::string>&)> generate =
    [&generate, &swap, &result](size_t k, std::vector<std::string>& input){
        if (k == 1)
        {
            result.push_back(input);
        }
        else
        {
            for (int i = 0; i < k; ++i)
            {
                generate(k - 1, input);
                if (k % 2 == 0)
                {
                    swap(input[i], input[k-1]);
                }
                else
                {
                    swap(input[0], input[k-1]);
                }
            }
        }
        return "";
    };

    std::vector<std::string> temp = input;
    generate(input.size(), temp);

    return result;
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> list;
    int searchBegin = 0;
    size_t pos = str.find(delim);
    while(true)
    {
        list.push_back(str.substr(searchBegin, pos-searchBegin));

        size_t newPos = str.find(delim, pos+delim.size());
        if(newPos == std::string::npos)
        {
            list.push_back(str.substr(pos+delim.size(), str.size()-(pos+delim.size())));
            break;
        }
        searchBegin = pos+1;
        pos = newPos;
    }
    return list;
}

std::map<std::string, std::map<std::string, int> > happinessList;

int main()
{
  std::fstream infile("input.txt");
  std::vector<std::string> lines;
  std::string line;
  while(std::getline(infile, line))
  {
    lines.push_back(line);
  }

  for(auto& line : lines)
  {
    line.erase(line.size()-1);
    std::vector<std::string> words = split(line, " ");
    happinessList[words[0]][words[words.size()-1]] = (words[2] == "lose" ? -1 : 1) * std::stoi(words[3]);
  }

  std::vector<std::string> names;
  for(auto& [name, map] : happinessList)
  {
#ifndef PART1
    happinessList["You"][name] = 0;
    happinessList[name]["You"] = 0;
#endif
    names.push_back(name);
  }

  int32_t maxHappiness = INT32_MIN;
  std::vector<std::vector<std::string> > variants = permutations(names);
  for(auto& variant : variants)
  {
    int32_t happiness = 0;
    for(int i = 0; i < variant.size()-1; ++i)
    {
      happiness += happinessList[variant[i]][variant[i+1]];
      happiness += happinessList[variant[i+1]][variant[i]];
    }
    happiness +=  happinessList[variant[variant.size()-1]][variant[0]];
    happiness +=  happinessList[variant[0]][variant[variant.size()-1]];
    if(happiness > maxHappiness)
    {
      maxHappiness = happiness;
    }
  }


#ifdef PART1
  std::cout<<"[Part 1] Max happiness: "<<maxHappiness<<std::endl;
#else
  std::cout<<"[Part 2] Max happiness: "<<maxHappiness<<std::endl;
#endif
  return 0;
}
