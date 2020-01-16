#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

//#define PART1

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> list;
    size_t searchBegin = 0;
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

//Dancer can fly 37 km/s for 1 seconds, but then must rest for 36 seconds.

struct Reindeer
{
    std::string name;
    int speed = 0;
    int moveTime = 0;
    int restTime = 0;
};

bool operator<(const Reindeer& r1, const Reindeer& r2)
{
    return r1.name < r2.name;
}

struct RaceInfo
{
    enum State
    {
        Flying = 0,
        Resting
    };
    State mState = Flying;
    int timeInCurrentState = 0;
    int distanceTraveled = 0;
#ifndef PART1
    int points = 0;
#endif
};

std::map<Reindeer, RaceInfo*> reindeers;

int main()
{
  std::fstream infile("input.txt");
  std::vector<std::string> reindeerInfos;
  std::string reindeerInfo;
  while(std::getline(infile, reindeerInfo))
  {
    reindeerInfos.push_back(reindeerInfo);
  }

  for(auto& info : reindeerInfos)
  {
    std::vector<std::string> words = split(info, " ");
    Reindeer reindeer;
    reindeer.name = words[0];
    reindeer.speed = std::stoi(words[3]);
    reindeer.moveTime = std::stoi(words[6]);
    reindeer.restTime = std::stoi(words[13]);
    reindeers.insert({reindeer, new RaceInfo()});
  }

  for(int i = 1; i <= 2503; ++i)// 2503 seconds, input from AoC
  {
#ifndef PART1
      std::vector<RaceInfo*> leaders;
#endif
      for(auto& [reindeer, info] : reindeers)
      {
          info->timeInCurrentState++;
          if(info->mState == RaceInfo::Flying)
          {
              info->distanceTraveled += reindeer.speed;
              if(info->timeInCurrentState == reindeer.moveTime)
              {
                  info->mState = RaceInfo::Resting;
                  info->timeInCurrentState = 0;
              }
          }
          else if(info->mState == RaceInfo::Resting)
          {
              if(info->timeInCurrentState == reindeer.restTime)
              {
                  info->mState = RaceInfo::Flying;
                  info->timeInCurrentState = 0;
              }
          }
#ifndef PART1
          if(leaders.empty() || info->distanceTraveled > leaders[0]->distanceTraveled)
          {
              leaders.clear();
              leaders.push_back(info);
          }
          else if(!leaders.empty() && info->distanceTraveled == leaders[0]->distanceTraveled)
          {
              leaders.push_back(info);
          }
#endif
      }
#ifndef PART1
      for(auto leaderInfo : leaders)
      {
        leaderInfo->points++;
      }
#endif
  }
#ifdef PART1
  int32_t maxDistance = INT32_MIN;
  for(auto& [reindeer, info] : reindeers)
  {
    if(info->distanceTraveled > maxDistance)
    {
        maxDistance = info->distanceTraveled;
    }
  }
  std::cout<<"[Part 1] Distance traveled: "<<maxDistance<<std::endl;
#else
  int32_t maxPoints = INT32_MIN;
  for(auto& [reindeer, info] : reindeers)
  {
    if(info->points > maxPoints)
    {
        maxPoints = info->points;
    }
  }
  std::cout<<"[Part 2] Max points: "<<maxPoints<<std::endl;
#endif
  return 0;
}
