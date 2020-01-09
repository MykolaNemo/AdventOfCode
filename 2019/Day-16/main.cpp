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

  std::vector<int> inputSignalSequence;
  for(auto& c : signal)
  {
    inputSignalSequence.push_back(c - '0');
  }
  std::cout<<inputSignalSequence.size()<<std::endl;

  const std::vector<int> basePattern = {0,1,0,-1};
  const int basePatternSize = basePattern.size();

  std::string offset;;
  for(int i = 0; i < 7; ++i)
  {
    offset += to_string(inputSignalSequence[i]);
  }
  std::cout<<"offset: "<<offset<<std::endl;

  auto baseInputSequence = inputSignalSequence;
  for(int i = 0; i < 10000; ++i)
  {
    for(auto& s : baseInputSequence)
    {
      inputSignalSequence.push_back(s);
    }
  }
//  std::cout<<inputSignalSequence.size()<<std::endl;
//  inputSignalSequence.erase(inputSignalSequence.begin(), inputSignalSequence.begin()+stoi(offset));
//  std::cout<<inputSignalSequence.size()<<std::endl;

  int phase = 0;
  while(phase < 100)
  {
    phase++;
    std::cout<<"phase:"<<phase<<std::endl;
    std::vector<int> outputSignalSequence;
    const int inputSize = inputSignalSequence.size();
    outputSignalSequence.resize(inputSize);

//    bool calculateLong = true;
//    int length = 0;
    int firstSum = -1;
    for(int i1 = stoi(offset); i1 < inputSize; ++i1)
//    for(int i1 = 4; i1 < inputSize; ++i1)
    {
//        std::cout<<i1<<std::endl;
        long long sum = 0;
        if(firstSum != -1)
        {
            sum = firstSum-inputSignalSequence[i1-1];
            firstSum = sum;
        }
        else
//        if(i1 < stoi(offset))
//        {
//            outputSignalSequence.push_back(0);
//            continue;
//        }
//        if(i1%1000 == 0)
//        {
//            std::cout<<i1<<std::endl;
//        }
//        long long sum = 0;
//        int indexForLongCalculations = 0;
//        {
//            if(i1 < inputSize/2)
//            {
//                ++length;
//            }
//            for(int l = i1; l < i1+length; ++l)
//            {
//                sum+=inputSignalSequence[l%inputSize];
//                std::cout<<inputSignalSequence[l%inputSize];
//            }
//            indexForLongCalculations = ((i1+length)%inputSize);
//            if((i1 >= inputSize/2) && (i1 < inputSize))
//            {
//                --length;
//            }
//        }
//        if(calculateLong)
        {
//            bool start = false;
            const int patternSequenceSize = basePatternSize*(i1+1);
            const int patternChunkSize = patternSequenceSize/basePatternSize;
            int patternNumber = 1;
            for(int i2 = 0; i2 < inputSize; ++i2, ++patternNumber)
            {
//                std::cout<<i2<<std::endl;
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
//                if(!start && i2 >= indexForLongCalculations)
//                {
//                    start = true;
//                }
//                if(start)
                {
                    sum+=inputSignalSequence[i2]*basePattern[basePatternIndex];
//                    std::cout<<inputSignalSequence[i2]*basePattern[basePatternIndex];
                }
            }
//            if(!start)
//            {
//                calculateLong = false;
//            }
        }
        if(firstSum == -1)
        {
            firstSum = sum;
        }
//        std::cout<<inputSignalSequence[i1-1]<<std::endl;
//        std::cout<<"-"<<sum<<std::endl;
        outputSignalSequence[i1] = std::abs(sum)%10;
    }
//    std::cout<<std::endl;
    inputSignalSequence = outputSignalSequence;

//    std::cout<<inputSignalSequence.size()<<std::endl;
    for(int i = stoi(offset); i < stoi(offset)+8; ++i)
    {
      std::cout<<inputSignalSequence[i];
    }
    std::cout<<std::endl;

//    for(int i = 0; i < 8; ++i)
//    {
//      std::cout<<inputSignalSequence[i];
//    }
//    std::cout<<std::endl;
  }
  return 0;
}
