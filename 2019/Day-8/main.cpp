#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

enum Color
{
  Black = 0,
  White = 1,
  Transparent = 2
};

int main()
{
    std::ifstream infile("input.txt");
    std::string imageMessage;
    std::getline(infile, imageMessage);

    std::vector<int> imagePixels;
    for(char c : imageMessage)
    {
        imagePixels.push_back(c - '0');
    }

    std::vector<std::vector<int> > layers;

    int pos = 0;
//    std::map<int, int> minCount{{0,-1},{1,-1},{2,-1}};
    while(pos < imagePixels.size())
    {
        std::vector<int> layer;
//        std::map<int, int> count;
        for(int i = pos; i < pos+150; ++i)
        {
//            if(imagePixels[i] == 0)
//            {
//                count[0]++;
//            }
//            else if(imagePixels[i] == 1)
//            {
//                count[1]++;
//            }
//            else if(imagePixels[i] == 2)
//            {
//                count[2]++;
//            }
            layer.push_back(imagePixels[i]);
        }
//        if((minCount[0] < 0) || (count[0] < minCount[0]))
//        {
//            minCount = count;
//        }
        layers.push_back(layer);
        pos += 150;
//        std::cout<<layer.size()<<std::endl;
    }
//    std::cout<<minCount[1]*minCount[2]<<std::endl;

    std::vector<int> imageResult;///25x6
    for(int pos = 0; pos < 6*25; ++pos)
    {
        imageResult.push_back((int)Transparent);
    }

    for(int l = 0; l < layers.size(); ++l)
    {
        for(int pos = 0; pos < 6*25; ++pos)
        {
            if(imageResult[pos] == (int)Transparent)
            {
                imageResult[pos] = layers[l][pos];
            }
            std::cout<<layers[l][pos];
        }
        std::cout<<std::endl;
    }
    for(int row = 0; row < 6; ++row)
    {
        for(int col = 0; col < 25; ++col)
        {
            int value = imageResult[row*25 + col];
            switch(value)
            {
            case Black:
                std::cout<<" ";
                break;
            case White:
                std::cout<<".";
                break;
            case Transparent:
                std::cout<<" ";
                break;
            }
        }
        std::cout<<std::endl;
    }
    return 0;
}
