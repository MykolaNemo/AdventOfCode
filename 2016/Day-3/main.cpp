#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//#define PART1

struct Triangle
{
    int a = 0;
    int b = 0;
    int c = 0;
    bool isPossible() const
    {
        bool possible = true;
        possible &= (a+b)>c;
        possible &= (a+c)>b;
        possible &= (b+c)>a;
        return possible;
    }
};

int main()
{
    std::vector<Triangle> triangles;
    std::fstream infile("input.txt");
#ifdef PART1
    std::string triangleString;
    while(std::getline(infile, triangleString))
    {
        std::vector<std::string> sides;
        {
            std::string side;
            for(int i = 0; i < triangleString.size(); ++i)
            {
                if(triangleString[i] != ' ')
                {
                    side += triangleString[i];
                }
                else if(!side.empty())
                {
                    sides.push_back(side);
                    side.clear();
                }
            }
            if(!side.empty())
            {
                sides.push_back(side);
                side.clear();
            }
        }

        Triangle t;
        t.a = std::stoi(sides[0]);
        t.b = std::stoi(sides[1]);
        t.c = std::stoi(sides[2]);
        triangles.push_back(t);

        sides.clear();
    }
#else
    std::vector<std::vector<int>> sidesArray;
    std::string row;
    while(std::getline(infile, row))
    {
        std::vector<int> numbers;
        {
            std::string number;
            for(int i = 0; i < row.size(); ++i)
            {
                if(row[i] != ' ')
                {
                    number += row[i];
                }
                else if(!number.empty())
                {
                    numbers.push_back(std::stoi(number));
                    number.clear();
                }
            }
            if(!number.empty())
            {
                numbers.push_back(std::stoi(number));
                number.clear();
            }
        }
        sidesArray.push_back(numbers);
        numbers.clear();
    }
    for(int i = 0; i < sidesArray.size(); i+=3)
    {
        Triangle t1;
        t1.a = sidesArray[i][0];
        t1.b = sidesArray[i+1][0];
        t1.c = sidesArray[i+2][0];

        Triangle t2;
        t2.a = sidesArray[i][1];
        t2.b = sidesArray[i+1][1];
        t2.c = sidesArray[i+2][1];

        Triangle t3;
        t3.a = sidesArray[i][2];
        t3.b = sidesArray[i+1][2];
        t3.c = sidesArray[i+2][2];

        triangles.push_back(t1);
        triangles.push_back(t2);
        triangles.push_back(t3);
    }
#endif
    int numberOfPossible = 0;
    for(const auto& t : triangles)
    {
        if(t.isPossible())
        {
            numberOfPossible++;
        }
    }
    std::cout<<"Answer: "<<numberOfPossible<<std::endl;
    return 0;
}
