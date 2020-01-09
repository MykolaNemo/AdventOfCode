#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>

using namespace std;

enum Color
{
  Black = 0,
  White = 1,
  Transparent = 2
};

int imageWidth = 25;
int imageHeight = 6;
const int pixelsNumber = imageWidth*imageHeight;

std::vector<int> image;
std::vector<std::vector<int> > imageLayers;

void part1()
{
    int pos = 0;
    std::map<int, int> minColorsCount{{Black,-1}, {White,-1}, {Transparent,-1}};
    while(pos < image.size())
    {
        std::map<int, int> colorsCount;
        std::vector<int> layer;
        for(int i = pos; i < pos+pixelsNumber; ++i)
        {
            colorsCount[image[i]]++;
        }
        if((minColorsCount[Black] < 0) || (colorsCount[Black] < minColorsCount[Black]))
        {
            minColorsCount = colorsCount;
        }
        pos += pixelsNumber;
    }
    std::cout<<"[Part 1] Answer: "<<minColorsCount[1] * minColorsCount[2]<<std::endl;
}

void part2()
{
    std::vector<int> displayImage;///25x6
    for(int pos = 0; pos < pixelsNumber; ++pos)
    {
        displayImage.push_back(Transparent);
    }

    // form layers
    int pos = 0;
    while(pos < image.size())
    {
        std::vector<int> layer;
        for(int i = pos; i < pos+pixelsNumber; ++i)
        {
            layer.push_back(image[i]);
        }
        imageLayers.push_back(layer);
        pos += pixelsNumber;
    }

    //convert from space to display format
    for(int l = 0; l < imageLayers.size(); ++l)
    {
        for(int pos = 0; pos < pixelsNumber; ++pos)
        {
            if(displayImage[pos] == Transparent)
            {
                displayImage[pos] = imageLayers[l][pos];
            }
        }
    }

    std::cout<<"[Part 2] Image\n";
    //display image
    for(int row = 0; row < imageHeight; ++row)
    {
        for(int col = 0; col < imageWidth; ++col)
        {
            int value = displayImage[row*imageWidth + col];
            switch(value)
            {
            case White:
                std::cout<<"*";
                break;
            case Black:
            case Transparent:
                std::cout<<" ";
                break;
            }
        }
        std::cout<<std::endl;
    }
}

int main()
{
    std::ifstream infile("input.txt");
    std::string imageMessage;
    std::getline(infile, imageMessage);

    for(char c : imageMessage)
    {
        image.push_back(c - '0');
    }

    part1();
    part2();
    return 0;
}
