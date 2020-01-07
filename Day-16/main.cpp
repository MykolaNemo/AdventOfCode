#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main()
{
  std::ifstream infile("input.txt");
  std::string signal;
  std::string s;
  while(std::getline(infile, s))
  {
    signal += s;
  }

  std::vector<int> baseInputSequence;
  for(auto& c : signal)
  {
    baseInputSequence.push_back(c - '0');
  }
  std::cout<<baseInputSequence.size()<<std::endl;

  const std::vector<int> basePattern = {0,1,0,-1};
  const int basePatternSize = basePattern.size();

  std::string offsetString;;
  for(int i = 0; i < 7; ++i)
  {
    offsetString += to_string(baseInputSequence[i]);
  }
  long long offset = stol(offsetString);
  std::cout<<"offset: "<<offset<<std::endl;

  std::vector<int> inputSignalSequence;
  for(int i = 0; i < 10000; ++i)
  {
    for(auto& s : baseInputSequence)
    {
      inputSignalSequence.push_back(s);
    }
  }

  int phase = 0;
  while(phase < 100)
  {
    phase++;
    const int inputSize = inputSignalSequence.size();
    std::vector<int> outputSignalSequence;
    outputSignalSequence.resize(inputSize);

    int firstSum = -1;
    for(int i1 = offset; i1 < inputSize; ++i1)
    {
        long long sum = 0;
        if(firstSum != -1)
        {
            sum = firstSum-inputSignalSequence[i1-1];
            firstSum = sum;
        }
        else
        {
            const int patternSequenceSize = basePatternSize*(i1+1);
            const int patternChunkSize = patternSequenceSize/basePatternSize;
            int patternNumber = i1+1;
            for(int i2 = i1; i2 < inputSize; ++i2, ++patternNumber)
            {
                if(i1 > i2)
                {
                    continue;
                }
                if(patternNumber >= patternSequenceSize)
                {
                    patternNumber = patternSequenceSize % patternNumber;
                }
                const int basePatternIndex = patternNumber/patternChunkSize;
                if(basePatternIndex == 0 || basePatternIndex == 2)
                {
                    continue;
                }
                sum+=inputSignalSequence[i2]*basePattern[basePatternIndex];
            }
        }
        if(firstSum == -1)
        {
            firstSum = sum;
        }
        outputSignalSequence[i1] = std::abs(sum)%10;
    }
    inputSignalSequence = outputSignalSequence;

//    for(int i = 0; i < 8; ++i)
//    {
//      std::cout<<inputSignalSequence[i];
//    }
//    std::cout<<std::endl;
  }

  for(int i = offset; i < offset+8; ++i)
  {
    std::cout<<inputSignalSequence[i];
  }
  std::cout<<std::endl;
  return 0;
}
