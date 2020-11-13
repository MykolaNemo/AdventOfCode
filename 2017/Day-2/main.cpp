#include <iostream>
#include "../../utils.h"

#define PART1

int main()
{
    std::vector<std::vector<int>> numTable;
    {
        auto content = readFile("input.txt");
        for(const auto& row : content)
        {
            numTable.push_back(vectorSTOI(split(row, "\t")));
        }
    }

    {
        int checksum = 0;
        for(const auto& numVector : numTable)
        {
            int max = 0;
            int min = -1;
            for(int n : numVector)
            {
                max = std::max(n, max);
                min = (min == -1) ? n : std::min(n, min);
            }
            checksum += (max-min);
        }
        std::cout<<"[Part 1] Answer: "<<checksum<<std::endl;
    }

    {
        int sum = 0;
        for(const auto& numVector : numTable)
        {
            for(int i = 0; i < static_cast<int>(numVector.size())-1; ++i)
            {
                bool found = false;
                for(int j = i+1; j < static_cast<int>(numVector.size()); ++j)
                {
                    int n1 = numVector[i];
                    int n2 = numVector[j];
                    if(n1 == (n1 / n2)*n2)
                    {
                        found = true;
                        sum += n1 / n2;
                        break;
                    }
                    if(n2 == (n2 / n1)*n1)
                    {
                        found = true;
                        sum += n2 / n1;
                        break;
                    }
                }
                if(found) break;
            }
        }
        std::cout<<"[Part 2] Answer: "<<sum<<std::endl;
    }
    return 0;
}
