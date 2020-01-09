#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
struct Dimension
{
    Dimension(int _l, int _w, int _h):l(_l),w(_w),h(_h){}
    int l = 0;
    int w = 0;
    int h = 0;
};

int main()
{
    std::vector<std::string> inputList;

    std::fstream infile("input.txt");
    std::string dimension;
    while(std::getline(infile, dimension))
    {
        inputList.push_back(dimension);
    }

    std::vector<Dimension> dimensions;
    for(std::string dimStr : inputList)
    {
        size_t pos = dimStr.find('x');
        int l = std::stoi(dimStr.substr(0, pos));

        dimStr = dimStr.substr(pos + 1, dimStr.size() - pos - 1);
        pos = dimStr.find_first_of('x');
        int w = std::stoi(dimStr.substr(0, pos));

        dimStr = dimStr.substr(pos + 1, dimStr.size() - pos - 1);
        pos = dimStr.find_first_of('x');
        int h = std::stoi(dimStr.substr(0, pos));

        dimensions.push_back(Dimension(l, w, h));
    }

    int paper = 0;
    int ribbon = 0;
    for(auto& d : dimensions)
    {
        int side1 = d.l * d.w;
        int side2 = d.w * d.h;
        int side3 = d.h * d.l;
        paper += 2*side1 + 2*side2 + 2*side3 + std::min(std::min(side1, side2), side3);

        int dist1 = 2*(d.h + d.w);
        int dist2 = 2*(d.l + d.w);
        int dist3 = 2*(d.l + d.h);
        int bow = d.l*d.w*d.h;
        ribbon += bow + std::min(std::min(dist1, dist2), dist3);
    }
    std::cout<<"[Part 1] Wrap paper needed: "<<paper<<std::endl;
    std::cout<<"[Part 2] Ribbon needed: "<<ribbon<<std::endl;
    return 0;
}
