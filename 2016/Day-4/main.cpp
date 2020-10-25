#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Room
{
    std::string name;
    int id;
    std::string checksum;
    bool isReal() const
    {
        bool real = true;
        return real;
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
            std::cout<<roomString<<std::endl;
            Room room;

            size_t lastDashPos = roomString.find_last_of('-');
            room.name = roomString.substr(0, lastDashPos);
            roomString = roomString.substr(lastDashPos+1, roomString.size()-lastDashPos-1);

            size_t startOfChecksumPos = roomString.find_first_of('[');
            room.id = std::stoi(roomString.substr(0,startOfChecksumPos));
            roomString = roomString.substr(startOfChecksumPos+1, roomString.size()-startOfChecksumPos-2);

            room.checksum = roomString;

            rooms.push_back(room);

            std::cout<<room.name<<" "<<room.id<<" "<<room.checksum<<std::endl<<std::endl;
        }
    }
    int idSum = 0;
    for(const auto& room : rooms)
    {
        if(room.isReal())
        {
            idSum += room.id;
        }
    }
    std::cout<<"Answer: "<<idSum<<std::endl;
    return 0;
}
