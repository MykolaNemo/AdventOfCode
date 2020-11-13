#include <iostream>
#include <map>
#include "../../utils.h"

struct MPoint : public Point
{
    MPoint():Point(){}
    MPoint(int x, int y): Point(x,y){}
    char label;
};
bool operator<(const MPoint& p1, const MPoint& p2)
{
    if(p1.y != p2.y)
    {
        return p1.y < p2.y;
    }
    if(p1.x != p2.x)
    {
        return p1.x < p2.x;
    }
    return false;
}

using Maze = std::vector<std::string>;
std::set<MPoint> allPoints;
std::map<char, std::map<char, int>> pathLengths;

bool isWall(char c)
{
    return c == '#';
}

int findPath(const Maze& maze, const MPoint& start, const MPoint& end)
{
    if(pathLengths[start.label][end.label] != 0)
    {
        return pathLengths[start.label][end.label];
    }

    int steps = 0;
    std::vector<MPoint> visitedPoints;
    std::vector<std::pair<MPoint,int>> unvisitedPoints = {{start,0}};
    while(!unvisitedPoints.empty())
    {
        std::pair<MPoint,int> currentPointSteps = unvisitedPoints.front();
        erase<std::pair<MPoint,int>>(unvisitedPoints, currentPointSteps);
        visitedPoints.push_back(currentPointSteps.first);
        if(currentPointSteps.first == end)
        {
            pathLengths[start.label][end.label] = currentPointSteps.second;
            pathLengths[end.label][start.label] = currentPointSteps.second;
            return currentPointSteps.second;
        }

        auto searchAround = [&maze, &visitedPoints, &unvisitedPoints, &currentPointSteps](const MPoint& p){
            if(!isWall(maze[p.y][p.x]) && !contains<MPoint>(visitedPoints, p))
            {
                unvisitedPoints.push_back({p, currentPointSteps.second + 1});
            }
        };
        int x = currentPointSteps.first.x;
        int y = currentPointSteps.first.y;
        searchAround(MPoint(x+1, y)); //right
        searchAround(MPoint(x-1, y)); //left
        searchAround(MPoint(x, y-1)); //top
        searchAround(MPoint(x, y+1)); //bottom
    }
    return steps;
}

struct State
{
    std::set<MPoint> pointsVisited;
    MPoint currentPoint;
    int steps = 0;
    friend bool operator==(const State& s1, const State& s2)
    {
        return (s1.currentPoint == s2.currentPoint) && (s1.pointsVisited == s2.pointsVisited);
    }
};

int main()
{
    MPoint startPoint;
    Maze maze = readFile("input.txt");
    for(int y = 0; y < static_cast<int>(maze.size()); ++y)
    {
        const std::string& row = maze[y];
        for(int x = 0; x < static_cast<int>(row.size()); ++x)
        {
            if(row[x] != '#' && row[x] != '.')
            {
                MPoint p(x,y);
                p.label = row[x];
                allPoints.insert(p);
                if(row[x] == '0')
                {
                    startPoint = p;
                }
            }
        }
    }

    {
        int minSteps = -1;
        int minStepsToComeBack = -1;

        State startState;
        startState.currentPoint = startPoint;
        startState.pointsVisited.insert(startPoint);

        std::vector<State> allStates;
        allStates.push_back(startState);

        std::vector<State> states;
        states.push_back(startState);

        while(!states.empty())
        {
            State oldState = states.front();
            erase<State>(states, oldState);
            for(const auto& nextPoint : allPoints)
            {
                if(contains<MPoint>(oldState.pointsVisited, nextPoint)) continue;

                int steps = findPath(maze, oldState.currentPoint, nextPoint);
                State newState = oldState;
                newState.currentPoint = nextPoint;
                newState.pointsVisited.insert(nextPoint);
                newState.steps += steps;

                if(newState.pointsVisited.size() == allPoints.size())
                {
                    if(minSteps == -1 || minSteps > newState.steps)
                    {
                        minSteps = newState.steps;
                    }
                    int stepsToComeBack = newState.steps + findPath(maze, newState.currentPoint, startPoint);
                    if(minStepsToComeBack == -1 || minStepsToComeBack > stepsToComeBack)
                    {
                        minStepsToComeBack = stepsToComeBack;
                    }
                    continue;
                }

                auto allIt = std::find(allStates.begin(), allStates.end(), newState);
                if(allIt == allStates.end())
                {
                    allStates.push_back(newState);
                    states.push_back(newState);
                }
                else if(newState.steps < allIt->steps)
                {
                    allIt->steps = newState.steps;

                    auto it = std::find(states.begin(), states.end(), newState);
                    if(it != states.end())
                    {
                        it->steps = newState.steps;
                    }
                }
            }
        }
        std::cout<<"[Part 1] Answer: "<<minSteps<<std::endl;
        std::cout<<"[Part 2] Answer: "<<minStepsToComeBack<<std::endl;
    }
    return 0;
}
