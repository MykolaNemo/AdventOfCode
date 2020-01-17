#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

//#define PART1

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

struct Sue
{
  Sue()
  {
    things["children"] = -1;
    things["cats"] = -1;
    things["samoyeds"] = -1;
    things["pomeranians"] = -1;
    things["akitas"] = -1;
    things["vizslas"] = -1;
    things["goldfish"] = -1;
    things["trees"] = -1;
    things["cars"] = -1;
    things["perfumes"] = -1;
  }
  std::string name;
  std::map<std::string, int> things;
};

int main()
{
  std::fstream infile("input.txt");
  std::vector<std::string> auntiesStrings;
  std::string sueString;
  while(std::getline(infile, sueString))
  {
    auntiesStrings.push_back(sueString);
  }

  //Sue 12: pomeranians: 4, akitas: 6, goldfish: 8
  std::vector<Sue> sues;
  for(auto& sueString : auntiesStrings)
  {
    std::vector<std::string> words = split(sueString, " ");
    words[1].erase(words[1].size()-1);
    words[2].erase(words[2].size()-1);
    words[3].erase(words[3].size()-1);
    words[4].erase(words[4].size()-1);
    words[5].erase(words[5].size()-1);
    words[6].erase(words[6].size()-1);

    std::map<std::string, int> things;
    things[words[2]] = std::stoi(words[3]);
    things[words[4]] = std::stoi(words[5]);
    things[words[6]] = std::stoi(words[7]);

    Sue sue;
    sue.name = words[0] + ' ' + words[1];
    for(auto& [thing, amount] : things)
    {
      sue.things[thing] = amount;
    }
    sues.push_back(sue);
  }

  //AoC input
  std::map<std::string, int> neededThings;
  neededThings["children"] = 3;
  neededThings["cats"] = 7;
  neededThings["samoyeds"] = 2;
  neededThings["pomeranians"] = 3;
  neededThings["akitas"] = 0;
  neededThings["vizslas"] = 0;
  neededThings["goldfish"] = 5;
  neededThings["trees"] = 3;
  neededThings["cars"] = 2;
  neededThings["perfumes"] = 1;

  std::string name;
  for(auto& sue : sues)
  {
    bool found = true;
    for(auto& [thing, amount] : sue.things)
    {
      if(amount != -1)
      {
#ifndef PART1
        if(thing == "cats" || thing == "trees")
        {
          if(amount <= neededThings[thing])
          {
            found = false;
            break;
          }
        }
        else if(thing == "pomeranians" || thing == "goldfish")
        {
          if(amount >= neededThings[thing])
          {
            found = false;
            break;
          }
        }
        else
#endif
        if(amount != neededThings[thing])
        {
          found = false;
          break;
        }
      }
    }
    if(found)
    {
      name = sue.name;
      break;
    }
  }
  std::cout<<"[Part 1] Auntie name: "<<name<<std::endl;
  return 0;
}
