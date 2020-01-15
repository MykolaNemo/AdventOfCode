#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

#define PART1

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> list;
    int searchBegin = 0;
    size_t pos = str.find(delim);
    while(true)
    {
        list.push_back(str.substr(searchBegin, pos));

        size_t newPos = str.find(delim, pos+delim.size());
        if(newPos == std::string::npos)
        {
            list.push_back(str.substr(pos+delim.size(), str.size()-(pos+delim.size())));
            break;
        }
        pos = newPos;
    }
    return list;
}

std::map<std::string, std::map<std::string, int> > distancesGraph;

struct State
{
    std::string currentPlace;
    std::set<std::string> visitedPlaces;
    int distance = 0;
};

int BFS()
{
    int result = -1;
    std::vector<State> queue;
    for(auto& [place1, place2Map] : distancesGraph)
    {
        queue.insert(queue.begin(), {place1, {place1}, 0});
    }
    while(!queue.empty())
    {
        State state = queue.back();
        queue.pop_back();
        if(state.visitedPlaces.size() == distancesGraph.size())
        {
#ifdef PART1
            if((result == -1) || (state.distance < result))
            {
                result = state.distance;
                continue;
            }
#else
            if((result == -1) || (state.distance > result))
            {
                result = state.distance;
                continue;
            }
#endif
        }
        for(auto& [nextPlace, distance] : distancesGraph[state.currentPlace])
        {
            if(nextPlace == state.currentPlace)
            {
                continue;
            }

            auto visitedIt = std::find(state.visitedPlaces.begin(),
                                       state.visitedPlaces.end(),
                                       nextPlace);
            if(visitedIt != state.visitedPlaces.end())
            {
                continue;
            }

            State newState = state;
            newState.currentPlace = nextPlace;
            newState.visitedPlaces.insert(nextPlace);
            newState.distance += distancesGraph[nextPlace][state.currentPlace];

            auto existsIt = std::find_if(queue.begin(), queue.end(),
                                         [&newState](const State& stateToCheck)->bool{
                return (newState.currentPlace == stateToCheck.currentPlace) &&
                       (newState.visitedPlaces == stateToCheck.visitedPlaces);
            });
            if(existsIt != queue.end())
            {
#ifdef PART1
                if(existsIt->distance > newState.distance)
                {
                    existsIt->distance = newState.distance;
                }
#else
                if(existsIt->distance < newState.distance)
                {
                    existsIt->distance = newState.distance;
                }
#endif
            }
            else
            {
                queue.insert(queue.begin(), newState);
            }
        }
    }
    return result;
}

int main()
{
    std::vector<std::string> distancesList;

    std::string distance;
    std::fstream infile("input.txt");
    while(std::getline(infile, distance))
    {
      distancesList.push_back(distance);
    }

    for(auto& d : distancesList)
    {
        auto list = split(d, " = ");
        auto places = split(list[0], " to ");
        int dist = std::stoi(list[1]);

        distancesGraph[places[0]][places[1]] = dist;
        distancesGraph[places[1]][places[0]] = dist;
    }

    for(auto& [place1, place2Map] : distancesGraph)
    {
        for(auto& [place2, dist] : place2Map)
        {
            std::cout<<place1<<"->"<<place2<<"="<<dist<<std::endl;
        }
    }

    int result = BFS();
    std::cout<<"[Part 1] Shortest distance: "<<result<<std::endl;
    return 0;
}
