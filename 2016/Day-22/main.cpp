#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <iomanip>

struct Node
{
    int size = 0;
    int used = 0;
    int avail = 0;
};

using Grid = std::vector<std::vector<Node>>;

int rows = 25;
int cols = 35;
const int dataX = cols-1;
const int dataY = 0;
int finalX = 0;
int finalY = 0;

void part2(Grid& grid)
{
    int minSize = 1000;
    int maxSize = 0;
    int minUsed = 1000;
    int maxUsed = 0;
    for(int y = 0; y < rows; ++y)
    {
        for(int x = 0; x < cols; ++x)
        {
            char c;
            minSize = std::min(minSize, grid[y][x].size);
            if(grid[y][x].size <= 100)
            {
                c = '.';
                maxSize = std::max(maxSize, grid[y][x].size);
            }
            else
            {
                c = '#';
            }

            if(grid[y][x].used != 0)
            {
                minUsed = std::min(minUsed, grid[y][x].used);
            }
            else
            {
                c = '0';
            }

            if(grid[y][x].used <= 100)
            {
                maxUsed = std::max(maxUsed, grid[y][x].used);
            }
            if(x == dataX && y == dataY)
            {
                c = 'G';
            }
            if(x == finalX && y == finalY)
            {
                c = 'F';
            }
            std::cout<<c;
        }
        std::cout<<std::endl;
    }
    std::cout<<"min size: "<<minSize<<std::endl
               <<"max size: "<<maxSize<<std::endl
                 <<"min used: "<<minUsed<<std::endl
                   <<"max used: "<<maxUsed<<std::endl;
}

void part1(const Grid& nodes)
{
    int n = 0;
    for(int y1 = 0; y1 < rows; ++y1)
    {
        for(int x1 = 0; x1 < cols; ++x1)
        {
            for(int y2 = y1; y2 < rows; ++y2)
            {
                for(int x2 = 0; x2 < cols; ++x2)
                {
                    if(x1 >= x2 && y1 == y2) continue;

                    if(nodes[y1][x1].used <= nodes[y2][x2].avail)
                    {
                        n++;
                    }
                }
            }
        }
    }
    std::cout<<"[Part 1] Answer: "<<n<<std::endl;
}

int main()
{
    std::fstream infile("input.txt");
    Grid nodes;
    for(int y = 0; y < rows; ++y)
    {
        std::vector<Node> row;
        for(int x = 0; x < cols; ++x)
        {
            row.push_back(Node());
        }
        nodes.push_back(row);
    }
    {
        std::string nodeString;
        while(std::getline(infile, nodeString))
        {
//            Filesystem              Size  Used  Avail  Use%
//            /dev/grid/node-x0-y0     94T   65T    29T   69%
            nodeString.erase(0, 16);

            int x = 0;
            int y = 0;
            {
                auto pos = nodeString.find_first_of('-');
                x = std::stoi(nodeString.substr(0, pos));
                nodeString = nodeString.substr(pos + 2, nodeString.size() - pos - 2);
            }

            {
                auto pos = nodeString.find_first_of(' ');
                y = std::stoi(nodeString.substr(0, pos));
                nodeString = nodeString.substr(pos, nodeString.size() - pos);
            }

            Node& node = nodes[y][x];
            {
                auto pos1 = nodeString.find_first_not_of(' ');
                auto pos2 = nodeString.find_first_of('T');
                node.size = std::stoi(nodeString.substr(pos1, pos2 - pos1));
                nodeString = nodeString.substr(pos2+1, nodeString.size() - pos2 - 1);
            }

            {
                auto pos1 = nodeString.find_first_not_of(' ');
                auto pos2 = nodeString.find_first_of('T');
                node.used = std::stoi(nodeString.substr(pos1, pos2 - pos1));
                nodeString = nodeString.substr(pos2+1, nodeString.size() - pos2 - 1);
            }

            {
                auto pos1 = nodeString.find_first_not_of(' ');
                auto pos2 = nodeString.find_first_of('T');
                node.avail = std::stoi(nodeString.substr(pos1, pos2 - pos1));
                nodeString = nodeString.substr(pos2+1, nodeString.size() - pos2 - 1);
            }
        }
    }

    part1(nodes);
    part2(nodes);
    std::cout<<"[Part 2] Answer: "<<35+5*33<<std::endl;
//    [Part 2] Answer: 35+5*33 = 200
//    F.................................G
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    .....................##############
//    ...................................
//    ...................................
//    ...........................0.......
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
//    ...................................
    return 0;
}