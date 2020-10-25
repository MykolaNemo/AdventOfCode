#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
        searchBegin = pos+delim.size();
        pos = newPos;
    }
    return list;
}

template<typename T>
void printVector(const std::vector<T>& v)
{
    for(const auto& e : v)
    {
        std::cout<<e<<",";
    }
    std::cout<<std::endl;
}

enum class Direction
{
    N,
    E,
    S,
    W
};

enum class Instruction
{
    Left,
    Right
};


Direction findDirection(Direction faceDirection, Instruction instruction)
{
    switch (faceDirection)
    {
    case Direction::N:
    {
        if(instruction == Instruction::Right)
        {
            return Direction::E;
        }
        else if(instruction == Instruction::Left)
        {
            return Direction::W;
        }
        break;
    }
    case Direction::E:
    {
        if(instruction == Instruction::Right)
        {
            return Direction::S;
        }
        else if(instruction == Instruction::Left)
        {
            return Direction::N;
        }
        break;
    }
    case Direction::S:
    {
        if(instruction == Instruction::Right)
        {
            return Direction::W;
        }
        else if(instruction == Instruction::Left)
        {
            return Direction::E;
        }
        break;
    }
    case Direction::W:
    {
        if(instruction == Instruction::Right)
        {
            return Direction::N;
        }
        else if(instruction == Instruction::Left)
        {
            return Direction::S;
        }
        break;
    }
    }
    return faceDirection;
}

std::vector<std::pair<int,int>> locations;

bool checkIfLocationIsVisited(const std::pair<int,int>& location)
{
    return (std::find(locations.begin(), locations.end(), location) != locations.end());
}


int main()
{
    Direction faceDirection = Direction::N;
    std::pair<int, int> location = {0,0};
    locations.push_back(location);

    std::vector<std::string> instructions;
    {
        std::string instructionString;
        std::fstream infile("input.txt");
        while(std::getline(infile, instructionString)) {}
        instructions = split(instructionString, ", ");
    }
    printVector<std::string>(instructions);
    bool found = false;
    for(const auto& instruction : instructions)
    {
        std::pair<int, int> oldLocation = location;
        if(instruction.front() == 'R')
        {
            faceDirection = findDirection(faceDirection, Instruction::Right);
        }
        else if(instruction.front() == 'L')
        {
            faceDirection = findDirection(faceDirection, Instruction::Left);
        }
        int numberOfBlocks = std::stoi(instruction.substr(1,instruction.size()-1));
        switch (faceDirection) {
        case Direction::N:
        {
            location.first += numberOfBlocks;
            for(int i = oldLocation.first+1; i <= location.first; ++i)
            {
                auto l = std::make_pair(i, location.second);
                if(checkIfLocationIsVisited(l))
                {
                    location = l;
                    found = true;
                    break;
                }
                locations.push_back(l);
            }
            break;
        }
        case Direction::E:
        {
            location.second += numberOfBlocks;
            for(int i = oldLocation.second+1; i <= location.second; ++i)
            {
                auto l = std::make_pair(location.first, i);
                if(checkIfLocationIsVisited(l))
                {
                    location = l;
                    found = true;
                    break;
                }
                locations.push_back(l);
            }
            break;
        }
        case Direction::S:
        {
            location.first -= numberOfBlocks;
            for(int i = oldLocation.first-1; i >= location.first; --i)
            {
                auto l = std::make_pair(i, location.second);
                if(checkIfLocationIsVisited(l))
                {
                    location = l;
                    found = true;
                    break;
                }
                locations.push_back(l);
            }
            break;
        }
        case Direction::W:
        {
            location.second -= numberOfBlocks;
            for(int i = oldLocation.second-1; i >= location.second; --i)
            {
                auto l = std::make_pair(location.first, i);
                if(checkIfLocationIsVisited(l))
                {
                    location = l;
                    found = true;
                    break;
                }
                locations.push_back(l);
            }
            break;
        }
        }
        if(found)
        {
            std::cout<<"[Part 2] Answer: "<<std::abs(location.first) + std::abs(location.second)<<std::endl;
            return 0;
        }
    }
    std::cout<<"[Part 1] Answer: "<<std::abs(location.first) + std::abs(location.second)<<std::endl;
    return 0;
}
