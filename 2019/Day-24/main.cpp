#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

//#define PART1

const int layerSize = 5;
int centerRow = 2;
int centerCol = 2;
std::map<int, std::vector<std::string>> field;
int minDepth = 0;
int maxDepth = 0;

bool isTileEmpty(char c)
{
    return (c == '.');
}

std::vector<std::string>& getLayer(int depth)
{
    if(depth < minDepth)
    {
        minDepth = depth;
    }
    if(depth > maxDepth)
    {
        maxDepth = depth;
    }
    auto layerIt = field.find(depth);
    if(layerIt != field.end())
    {
        return layerIt->second;
    }
    else
    {
        std::vector<std::string> fieldLayer;
        for(int row = 0; row < layerSize; ++row)
        {
            fieldLayer.push_back(".....");
        }
        field.insert({depth, fieldLayer});
        return field[depth];
    }
}

int adjacentBugsCount(int row, int col, int depth)
{
    int count = 0;
    std::vector<std::string> fieldLayer;
    auto layerIt = field.find(depth);
    if(layerIt != field.end())
    {
        fieldLayer = layerIt->second;
    }
//#ifdef PART1
//    if((row < fieldLayer.size()-1) && !isTileEmpty(fieldLayer[row+1][col]))
//    {
//        count++;
//    }

//    if((row > 0) && !isTileEmpty(fieldLayer[row-1][col]))
//    {
//        count++;
//    }

//    if((col < fieldLayer[0].size()-1) && !isTileEmpty(fieldLayer[row][col+1]))
//    {
//        count++;
//    }

//    if((col > 0) && !isTileEmpty(fieldLayer[row][col-1]))
//    {
//        count++;
//    }
//#else
    {
        int r = row+1;
        int c = col;
        if(r == layerSize)
        {
            auto& outerLayer = getLayer(depth-1);
            count += isTileEmpty(outerLayer[centerRow+1][centerCol]) ? 0 : 1;
        }
        else if(r == centerRow && c == centerCol)
        {
            auto& innerLayer = getLayer(depth+1);
            for(int i = 0; i < layerSize; ++i)
            {
                count += isTileEmpty(innerLayer[0][i]) ? 0 : 1;
            }
        }
        else
        {
            count += isTileEmpty(fieldLayer[r][c]) ? 0 : 1;
        }
    }

    {
        int r = row-1;
        int c = col;
        if(r == -1)
        {
            auto& outerLayer = getLayer(depth-1);
            count += isTileEmpty(outerLayer[centerRow-1][centerCol]) ? 0 : 1;
        }
        else if(r == centerRow && c == centerCol)
        {
            auto& innerLayer = getLayer(depth+1);
            for(int i = 0; i < layerSize; ++i)
            {
                count += isTileEmpty(innerLayer[layerSize-1][i]) ? 0 : 1;
            }
        }
        else
        {
            count += isTileEmpty(fieldLayer[r][c]) ? 0 : 1;
        }
    }

    {
        int r = row;
        int c = col+1;
        if(c == layerSize)
        {
            auto& outerLayer = getLayer(depth-1);
            count += isTileEmpty(outerLayer[centerRow][centerCol+1]) ? 0 : 1;
        }
        else if(r == centerRow && c == centerCol)
        {
            auto& innerLayer = getLayer(depth+1);
            for(int i = 0; i < layerSize; ++i)
            {
                count += isTileEmpty(innerLayer[i][0]) ? 0 : 1;
            }
        }
        else
        {
            count += isTileEmpty(fieldLayer[r][c]) ? 0 : 1;
        }
    }

    {
        int r = row;
        int c = col-1;
        if(c == -1)
        {
            auto& outerLayer = getLayer(depth-1);
            count += isTileEmpty(outerLayer[centerRow][centerCol-1]) ? 0 : 1;
        }
        else if(r == centerRow && c == centerCol)
        {
            auto& innerLayer = getLayer(depth+1);
            for(int i = 0; i < layerSize; ++i)
            {
                count += isTileEmpty(innerLayer[i][layerSize-1]) ? 0 : 1;
            }
        }
        else
        {
            count += isTileEmpty(fieldLayer[r][c]) ? 0 : 1;
        }
    }
//    }
//#endif
    return count;
}

struct Update
{
    Update(int _r, int _c, int _d, char _u):
        row(_r), col(_c), depth(_d), update(_u){}

    int row = 0;
    int col = 0;
    int depth = 0;
    char update = '.';
};

int main()
{
    std::ifstream infile("input.txt");

    std::string row;
    std::vector<std::string> fieldLayer0;
    while(std::getline(infile, row))
    {
        fieldLayer0.push_back(row);
    }
    field.insert({0, fieldLayer0});

#ifdef PART1
    intmax_t biodiversity = 0;
    std::map<std::vector<std::string>, bool> history;
    history.insert({fieldLayer0, true});
#else
    int minutes = 0;
#endif
    while(true)
    {
        std::vector<Update> updates;
        auto fieldCopy = field;
        int minD = minDepth;
        int maxD = maxDepth;
        for(int depth = minD-1; depth <= maxD+1; depth++)
        {
            auto& fieldLayer = getLayer(depth);
            for(int row = 0; row < layerSize; row++)
            {
                for(int col = 0; col < layerSize; col++)
                {
#ifndef PART1
                    if(row == centerRow && col == centerCol)
                    {
                        continue;
                    }
#endif
                    char& tile = fieldLayer[row][col];
                    int adjBugs = adjacentBugsCount(row, col, depth);
                    if(!isTileEmpty(tile) && (adjBugs != 1))
                    {
                            updates.push_back(Update(row, col, depth, '.'));
                    }
                    else if(isTileEmpty(tile) && (adjBugs == 1 || adjBugs == 2))
                    {
                            updates.push_back(Update(row, col, depth, '#'));
                    }
                }
            }
        }
        for(auto& update : updates)
        {
            auto& fieldLayer = field.at(update.depth);
            fieldLayer[update.row][update.col] = update.update;
        }
#ifdef PART1
            if(history.find(field[0]) != history.end())
            {
                for(int row = 0; row < layerSize; ++row)
                {
                    std::cout<<field[0][row]<<std::endl;
                    for(int col = 0; col < layerSize; ++col)
                    {
                        if(!isTileEmpty(field[0][row][col]))
                        {
                            biodiversity += static_cast<intmax_t>(std::pow(2, row*layerSize + col));
                        }
                    }
                }
                std::cout<<std::endl;
                std::cout<<"[Part 1] Biodiversity: "<<biodiversity<<std::endl;
                return 0;
            }
            history.insert({field[0], true});
#else
        minutes++;
//        std::cout<<minutes<<std::endl;
        if(minutes == 200)
        {
//            for(auto& [depth, fieldLayer] : field)
//            {
//                std::cout<<"depth: "<<depth<<std::endl;
//                for(const auto &row : fieldLayer)
//                {
//                    std::cout<<row<<std::endl;
//                }
//                std::cout<<std::endl;
//                std::cout<<std::endl;
//            }
            intmax_t numberOfBugs = 0;
            for(auto& [depth, fieldLayer] : field)
            {
                for(int row = 0; row < layerSize; row++)
                {
                    for(int col = 0; col < layerSize; col++)
                    {
                        if(!isTileEmpty(fieldLayer[row][col]))
                        {
                            numberOfBugs++;
                        }
                    }
                }
            }
            std::cout<<"[Part 2] Number of bugs: "<<numberOfBugs<<std::endl;
            return 0;
        }
#endif
    }

    return 0;
}
