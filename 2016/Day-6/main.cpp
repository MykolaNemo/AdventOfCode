#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

int main()
{
    std::fstream infile("input.txt");
    std::vector<std::string> messages;
    {
        std::string message;
        while(std::getline(infile, message))
        {
            messages.push_back(message);
        }
    }
    std::string answer1;
    std::string answer2;
    std::vector<std::vector<std::pair<char, int>>> allLetters;
    for(int i = 0; i < messages[0].size(); ++i)
    {
        allLetters.push_back(std::vector<std::pair<char, int>>());
    }
    for(const auto& message : messages)
    {
        for(int i = 0; i < message.size(); ++i)
        {
            char letter = message[i];
            auto letterPair = std::find_if(allLetters[i].begin(), allLetters[i].end(),[&letter](const std::pair<char, int>& p)->bool{
                return (p.first == letter);
            });
            if(letterPair != allLetters[i].end())
            {
                letterPair->second++;
            }
            else
            {
                std::pair<char, int> pair = {letter, 1};
                allLetters[i].push_back(pair);
            }
        }
    }

    for(auto& column : allLetters)
    {
        std::sort(column.begin(), column.end(), [](const std::pair<char, int>& p1, const std::pair<char, int>& p2)->bool{
            if(p1.second != p2.second)
            {
                return p1.second > p2.second; //greater first
            }
            else
            {
                return p2.first > p1.first; //if same -> alphabetical order
            }
            return true;
        });
        answer1.push_back(column[0].first);
        answer2.push_back(column[column.size()-1].first);
    }
    std::cout<<"[Part1] Answer: "<<answer1<<std::endl;
    std::cout<<"[Part2] Answer: "<<answer2<<std::endl;
    return 0;
}
