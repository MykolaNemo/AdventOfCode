#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

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

// AoC input
//const std::string medicineMolecule = "CRnSiRnCaPTiMgYCaPTiRnFArSiThFAr" \
//                                     "CaSiThSiThPBCaCaSiRnSiRnTiTiMgAr" \
//                                     "PBCaPMgYPTiRnFArFArCaSiRnBPMgArP" \
//                                     "RnCaPTiRnFArCaSiThCaCaFArPBCaCaP" \
//                                     "TiTiRnFArCaSiRnSiAlYSiThRnFArArC" \
//                                     "aSiRnBFArCaCaSiRnSiThCaCaCaFYCaP" \
//                                     "TiBCaSiThCaSiThPMgArSiRnCaPBFYCa" \
//                                     "CaFArCaCaCaCaSiThCaSiRnPRnFArPBS" \
//                                     "iThPRnFArSiRnMgArCaFYFArCaSiRnSi" \
//                                     "AlArTiTiTiTiTiTiTiRnPMgArPTiTiTi" \
//                                     "BSiRnSiAlArTiTiRnPMgArCaFYBPBPTi" \
//                                     "RnSiRnMgArSiThCaFArCaSiThFArPRnF" \
//                                     "ArCaSiRnTiBSiThSiRnSiAlYCaFArPRn" \
//                                     "FArSiThCaFArCaCaSiThCaCaCaSiRnPR" \
//                                     "nCaFArFYPMgArCaPBCaPBSiRnFYPBCaF" \
//                                     "ArCaSiAl";
// AoC input 2
const std::string medicineMolecule = "ORnPBPMgArCaCaCaSiThCaCaSiThCaCa" \
                                     "PBSiRnFArRnFArCaCaSiThCaCaSiThCa" \
                                     "CaCaCaCaCaSiRnFYFArSiRnMgArCaSiR" \
                                     "nPTiTiBFYPBFArSiRnCaSiRnTiRnFArS" \
                                     "iAlArPTiBPTiRnCaSiAlArCaPTiTiBPM" \
                                     "gYFArPTiRnFArSiRnCaCaFArRnCaFArC" \
                                     "aSiRnSiRnMgArFYCaSiRnMgArCaCaSiT" \
                                     "hPRnFArPBCaSiRnMgArCaCaSiThCaSiR" \
                                     "nTiMgArFArSiThSiThCaCaSiRnMgArCa" \
                                     "CaSiRnFArTiBPTiRnCaSiAlArCaPTiRn" \
                                     "FArPBPBCaCaSiThCaPBSiThPRnFArSiT" \
                                     "hCaSiThCaSiThCaPTiBSiRnFYFArCaCa" \
                                     "PRnFArPBCaCaPBSiRnTiRnFArCaPRnFA" \
                                     "rSiRnCaCaCaSiThCaRnCaFArYCaSiRnF" \
                                     "ArBCaCaCaSiThFArPBFArCaSiRnFArRn" \
                                     "CaCaCaFArSiRnFArTiRnPMgArF";
std::map<std::string, std::vector<std::string>> allTransforms;
std::map<std::string, std::vector<std::string>> allReverseTransforms;
size_t minProductLength = SIZE_MAX;
size_t maxProductLength = 0;

#ifndef PART1

struct State
{
    State(std::string currMol, int steps): currentMolecule(currMol), numberOfSteps(steps){}
    std::string currentMolecule;
    int numberOfSteps = 0;
};

bool operator<(const State& s1,const State& s2)
{
    return s1.currentMolecule < s2.currentMolecule;
}

bool operator==(const State& s1,const State& s2)
{
    return s1.currentMolecule < s2.currentMolecule;
}

int32_t findMolecule()
{
    int32_t result = INT32_MAX;
    std::vector<State> queue = {State(medicineMolecule, 0)};
    std::map<State, bool> queueMap = {{{State(medicineMolecule, 0)}, true}};
    while(!queue.empty())
    {
        State state = queue.back();
        queue.pop_back();
        queueMap.erase(state);

        if(state.currentMolecule == "e")//AoC input
        {
            result = std::min(state.numberOfSteps, result);
            std::cout<<"[Part 2] Result: "<<result<<std::endl;
        }
        for(size_t pos = 0; pos < state.currentMolecule.size(); ++pos)
        {
            if(state.currentMolecule[pos] < 'A' || state.currentMolecule[pos] > 'Z')
            {
                continue;
            }

            std::string partMolecule;
            for(size_t i = pos; (i < pos+maxProductLength) && (i < state.currentMolecule.size()); ++i)
            {
                partMolecule += state.currentMolecule[i];
                if(partMolecule.size() < minProductLength) continue;

                auto rTransformIt = allReverseTransforms.find(partMolecule);
                if(rTransformIt == allReverseTransforms.end())
                {
                    continue;
                }

                for(const auto& rResult : rTransformIt->second)
                {
                    State newState = state;
                    newState.currentMolecule.erase(pos, partMolecule.size());
                    newState.currentMolecule.insert(pos, rResult);
                    newState.numberOfSteps++;

                    auto queueIt = std::find_if(queue.begin(), queue.end(), [&newState](const State& state)->bool{
                        return (newState.currentMolecule == state.currentMolecule);
                    });
                    if(queueIt != queue.end())
                    {
                        if(newState.numberOfSteps < queueIt->numberOfSteps)
                        {
                            queueIt->numberOfSteps = newState.numberOfSteps;
                        }
                    }
                    else
                    {
                        queue.push_back(newState);
                    }
                }
            }
        }
    }
    return result;
}
#endif

int main()
{
  std::fstream infile("input2.txt");

  //Ca => SiRnMgAr
  {
    std::string transform;
    std::vector<std::string> words;
    while(std::getline(infile, transform))
    {
      words = split(transform, " => ");
      allTransforms[words[0]].push_back(words[1]);
      allReverseTransforms[words[1]].push_back(words[0]);
//      maxProductLength = std::max(words[0].size(), maxProductLength);
//      minProductLength = std::min(words[0].size(), minProductLength);
      maxProductLength = std::max(words[1].size(), maxProductLength);
      minProductLength = std::min(words[1].size(), minProductLength);
      words.clear();
    }
  }

#ifdef PART1
  std::set<std::string> resultMolecules;
  for(size_t pos = 0; pos < medicineMolecule.size(); ++pos)
  {
    std::string reactionString;
    auto transformLambda = [&resultMolecules, &pos, &reactionString]
                     (const std::vector<std::string>& transformedMolecules)
    {
      for(auto& m : transformedMolecules)
      {
        std::string newMolecule = medicineMolecule;
        newMolecule.erase(pos, reactionString.size());
        newMolecule.insert(pos, m);
        resultMolecules.insert(newMolecule);
      }
    };

    reactionString += medicineMolecule[pos];
    auto transformIt = allTransforms.find(reactionString);
    if(transformIt != allTransforms.end())
    {
      transformLambda(transformIt->second);
    }

    if(pos < medicineMolecule.size()-1)
    {
      reactionString += medicineMolecule[pos+1];
      auto transformIt = allTransforms.find(reactionString);
      if(transformIt != allTransforms.end())
      {
        transformLambda(transformIt->second);
      }
    }
  }
  std::cout<<"[Part 1] Molecules number: "<<resultMolecules.size()<<std::endl;
#else
  int32_t steps = findMolecule();
  std::cout<<"[Part 2] Molecules number: "<<steps<<std::endl;
#endif

  return 0;
}
