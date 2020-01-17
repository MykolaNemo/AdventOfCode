#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

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
// AoC input
const std::string medicineMolecule = "CRnSiRnCaPTiMgYCaPTiRnFArSiThFAr" \
                                     "CaSiThSiThPBCaCaSiRnSiRnTiTiMgAr" \
                                     "PBCaPMgYPTiRnFArFArCaSiRnBPMgArP" \
                                     "RnCaPTiRnFArCaSiThCaCaFArPBCaCaP" \
                                     "TiTiRnFArCaSiRnSiAlYSiThRnFArArC" \
                                     "aSiRnBFArCaCaSiRnSiThCaCaCaFYCaP" \
                                     "TiBCaSiThCaSiThPMgArSiRnCaPBFYCa" \
                                     "CaFArCaCaCaCaSiThCaSiRnPRnFArPBS" \
                                     "iThPRnFArSiRnMgArCaFYFArCaSiRnSi" \
                                     "AlArTiTiTiTiTiTiTiRnPMgArPTiTiTi" \
                                     "BSiRnSiAlArTiTiRnPMgArCaFYBPBPTi" \
                                     "RnSiRnMgArSiThCaFArCaSiThFArPRnF" \
                                     "ArCaSiRnTiBSiThSiRnSiAlYCaFArPRn" \
                                     "FArSiThCaFArCaCaSiThCaCaCaSiRnPR" \
                                     "nCaFArFYPMgArCaPBCaPBSiRnFYPBCaF" \
                                     "ArCaSiAl";

#ifndef PART1

struct State
{
  std::string currentMolecule;
};

int findMolecule()
{
  std::vector<std::vector<std::string>>
  return 0;
}
#endif

int main()
{
  std::fstream infile("input.txt");

  //Ca => SiRnMgAr
  std::map<std::string, std::vector<std::string>> allTransforms;
  {
    std::string transform;
    std::vector<std::string> words;
    while(std::getline(infile, transform))
    {
      words = split(transform, " => ");
      allTransforms[words[0]].push_back(words[1]);
      words.clear();
    }
  }

#ifdef PART1
  std::set<std::string> resultMolecules;
  for(int pos = 0; pos < medicineMolecule.size(); ++pos)
  {
    std::string reactionString;
    auto lambda = [&medicineMolecule, &resultMolecules, &pos, &reactionString]
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
      lambda(transformIt->second);
    }

    if(pos < medicineMolecule.size()-1)
    {
      reactionString += medicineMolecule[pos+1];
      auto transformIt = allTransforms.find(reactionString);
      if(transformIt != allTransforms.end())
      {
        lambda(transformIt->second);
      }
    }
  }
  std::cout<<"[Part 1] Molecules number: "<<resultMolecules.size()<<std::endl;
#else
  int steps = findMolecule();
  std::cout<<"[Part 2] Molecules number: "<<steps<<std::endl;
#endif

  return 0;
}
