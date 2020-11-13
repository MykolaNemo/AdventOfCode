#include <iostream>
#include "../../utils.h"

bool isValid1(const std::vector<std::string>& words)
{
    for(int i = 1; i < static_cast<int>(words.size()); ++i)
    {
        auto word = words[i-1];
        auto it = std::find(words.begin()+i, words.end(), word);
        if(it != words.end())
        {
            return false;
        }
    }
    return true;
}

bool isValid2(const std::vector<std::string>& words)
{
    for(int i = 1; i < static_cast<int>(words.size()); ++i)
    {
        auto word = words[i-1];
        auto it = std::find_if(words.begin()+i, words.end(), [&word](std::string w)->bool{
            for(char c : word)
            {
                auto pos = w.find_first_of(c);
                if(pos != std::string::npos)
                {
                    w.erase(pos, 1);
                    continue;
                }
                return false;
            }
            return w.empty();
        });
        if(it != words.end())
        {
            return false;
        }
    }
    return true;
}

int main()
{
    auto content = readFile("input.txt");
    int answer1 = 0;
    int answer2 = 0;
    for(const auto& row : content)
    {
        auto words = split(row, " ");
        answer1 += isValid1(words) ? 1 : 0;
        answer2 += isValid2(words) ? 1 : 0;
    }
    std::cout<<"[Part 1] Answer: "<<answer1<<std::endl;
    std::cout<<"[Part 2] Answer: "<<answer2<<std::endl;
    return 0;
}
