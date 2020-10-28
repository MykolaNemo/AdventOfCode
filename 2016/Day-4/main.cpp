#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

struct Room
{
    std::string name;
    int id;
    std::string checksum;
    bool isReal() const
    {
        std::vector<std::pair<char, int>> lettersCount;
        for(const auto& letter : name)
        {
            if(letter < 'a' || letter > 'z')
            {
                continue;
            }
            auto letterPair = std::find_if(lettersCount.begin(), lettersCount.end(),[&letter](const std::pair<char, int>& p)->bool{
                            return (p.first == letter);
            });
            if(letterPair != lettersCount.end())
            {
                letterPair->second++;
            }
            else
            {
                std::pair<char, int> pair = {letter, 1};
                lettersCount.push_back(pair);
            }
        }
        std::sort(lettersCount.begin(), lettersCount.end(), [](const std::pair<char, int>& p1, const std::pair<char, int>& p2)->bool{
            if(p1.second != p2.second)
            {
                return p1.second > p2.second; //greater first
            }
            else
            {
                return p2.first > p1.first; //if same -> alphabetical order
            }
        });

        std::set<char> checkSumSet;
        for(const auto& c : checksum)
        {
            checkSumSet.insert(c);
        }

        std::set<char> nameSet;
        for(int i = 0; i < checksum.size(); ++i)
        {
            nameSet.insert(lettersCount[i].first);
        }

        return (checkSumSet == nameSet);
    }

    void printRealName() const
    {
        std::cout<<"Id: "<<id<<" ";
        for(char c : name)
        {
            if(c == '-')
            {
                c = ' ';
            }
            else
            {
                c = ((c - 'a') + (id % 26))%26 + 'a';
            }
            std::cout<<c;
        }
        std::cout<<std::endl;
    }
};

int main()
{
    std::fstream infile("input.txt");
    std::vector<Room> rooms;
    {
        std::string roomString;
        while(std::getline(infile, roomString))
        {
//            std::cout<<roomString<<std::endl;
            Room room;

            size_t lastDashPos = roomString.find_last_of('-');
            room.name = roomString.substr(0, lastDashPos);
            roomString = roomString.substr(lastDashPos+1, roomString.size()-lastDashPos-1);

            size_t startOfChecksumPos = roomString.find_first_of('[');
            room.id = std::stoi(roomString.substr(0,startOfChecksumPos));
            roomString = roomString.substr(startOfChecksumPos+1, roomString.size()-startOfChecksumPos-2);

            room.checksum = roomString;

            rooms.push_back(room);

//            std::cout<<room.name<<" "<<room.id<<" "<<room.checksum<<std::endl<<std::endl;
        }
    }
    int idSum = 0;
    for(const auto& room : rooms)
    {
        room.printRealName();
        if(room.isReal())
        {
            idSum += room.id;
        }
    }
    std::cout<<"Answer: "<<idSum<<std::endl;
    return 0;
}
