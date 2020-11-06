#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

#define PART1

std::string password1 = "abcdefgh";
std::string password2 = "fbgdceah";

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
        searchBegin = pos+1;
        pos = newPos;
    }
    return list;
}

/*

    swap position X with position Y
means that the letters at indexes X and Y (counting from 0) should be swapped.

    swap letter X with letter Y
means that the letters X and Y should be swapped (regardless of where they appear in the string).

    rotate left/right X steps
means that the whole string should be rotated; for example, one right rotation would turn abcd into dabc.

    rotate based on position of letter X
means that the whole string should be rotated to the right based on the index of letter X (counting from 0) as determined before this instruction does any rotations.
Once the index is determined, rotate the string to the right one time, plus a number of times equal to that index, plus one additional time if the index was at least 4.

    reverse positions X through Y
means that the span of letters at indexes X through Y (including the letters at X and Y) should be reversed in order.

    move position X to position Y
means that the letter which is at index X should be removed from the string, then inserted such that it ends up at index Y.

*/

void swapPositions(int x, int y, std::string& str)
{
    char t = str[x];
    str[x] = str[y];
    str[y] = t;
}

void swapLetters(char letterX, char letterY, std::string& str)
{
    for(int i = 0; i < static_cast<int>(str.size()); ++i)
    {
        if(str[i] == letterX)
        {
            str[i] = letterY;
            continue;
        }
        if(str[i] == letterY)
        {
            str[i] = letterX;
            continue;
        }
    }
}

void rotateLeft(int steps, std::string& str)
{
    if(steps > 0)
    {
        auto right = str.substr(steps, str.size() - steps);
        auto left = str.substr(0, steps);
        str = right + left;
    }
}

void rotateRight(int steps, std::string& str)
{
    if(steps > 0)
    {
        auto right = str.substr(str.size() - steps, steps);
        auto left = str.substr(0, str.size() - steps);
        str = right + left;
    }
}

void rotateRightBasedOnLetter(char letter, std::string& str)
{
    int index = -1;
    for(int i = 0; i < static_cast<int>(str.size()); ++i)
    {
        if(str[i] == letter)
        {
            index = i;
            break;
        }
    }
    int steps = (1 + index + (index >= 4 ? 1 : 0)) % str.size();
    rotateRight(steps, str);
}

void reverseRotateRightBasedOnLetter(char letter, std::string& str)
{
    std::vector<std::string> strings;
    {
        std::string temp = str;
        for(int i = 1; i <= static_cast<int>(temp.size()); ++i)
        {
            rotateRight(1, temp);
            strings.push_back(temp);
        }
    }

    for(auto& s : strings)
    {
        std::string save = s;
        rotateRightBasedOnLetter(letter, s);
        if(s == str)
        {
            str = save;
            return;
        }
    }
}

void reverse(int x, int y, std::string& str)
{
    for(int i = x; i <= y; ++i)
    {
        if(i >= x + (y-x+1)/2)
        {
            break;
        }
        char t = str[i];
        str[i] = str[y-(i-x)];
        str[y-(i-x)] = t;
    }
}

void move(int x, int y, std::string& str)
{
    char c = str[x];
    auto it = str.begin();
    std::advance(it, x);
    str.erase(it);
    str.insert(y, 1, c);
}

int main()
{
    std::fstream infile("input.txt");
    std::vector<std::string> instructions;
    {
        std::string instruction;
        while(std::getline(infile, instruction))
        {
            instructions.push_back(instruction);
        }
    }
    for(const auto& instruction : instructions)
    {
        auto words = split(instruction, " ");
        if(words[0] == "swap" && words[1] == "position")
        {
            int x = std::stoi(words[2]);
            int y = std::stoi(words[5]);
            swapPositions(x, y, password1);
        }
        else if(words[0] == "swap" && words[1] == "letter")
        {
            char letterX = words[2][0];
            char letterY = words[5][0];
            swapLetters(letterX, letterY, password1);
        }
        else if(words[0] == "rotate" && words[1] == "left")
        {
            int steps = std::stoi(words[2]) % password1.size();
            rotateLeft(steps, password1);
        }
        else if(words[0] == "rotate" && words[1] == "right")
        {
            int steps = std::stoi(words[2]) % password1.size();
            rotateRight(steps, password1);
        }
        else if(words[0] == "rotate" && words[1] == "based")
        {
            char letter = words[6][0];
            rotateRightBasedOnLetter(letter, password1);
        }
        else if(words[0] == "reverse")
        {
            int x = std::stoi(words[2]);
            int y = std::stoi(words[4]);
            reverse(x, y, password1);
        }
        else if(words[0] == "move")
        {
            int x = std::stoi(words[2]);
            int y = std::stoi(words[5]);
            move(x, y, password1);
        }
    }
    std::cout<<"[Part 1] Answer: "<<password1<<std::endl;

    //reverse
    for(int i = static_cast<int>(instructions.size())-1; i >=0; --i)
    {
        auto words = split(instructions[i], " ");
        if(words[0] == "swap" && words[1] == "position")
        {
            //the same
            int x = std::stoi(words[2]);
            int y = std::stoi(words[5]);
            swapPositions(x, y, password2);
        }
        else if(words[0] == "swap" && words[1] == "letter")
        {
            //the same
            char letterX = words[2][0];
            char letterY = words[5][0];
            swapLetters(letterX, letterY, password2);
        }
        else if(words[0] == "rotate" && words[1] == "left")
        {
            //rotate right instead of left
            int steps = std::stoi(words[2]) % password2.size();
            rotateRight(steps, password2);
        }
        else if(words[0] == "rotate" && words[1] == "right")
        {
            //rotate left instead of right
            int steps = std::stoi(words[2]) % password1.size();
            rotateLeft(steps, password2);
        }
        else if(words[0] == "rotate" && words[1] == "based")
        {
            //call reverse function
            char letter = words[6][0];
            reverseRotateRightBasedOnLetter(letter, password2);
        }
        else if(words[0] == "reverse")
        {
            //the same
            int x = std::stoi(words[2]);
            int y = std::stoi(words[4]);
            reverse(x, y, password2);
        }
        else if(words[0] == "move")
        {
            //move "from y to x" instead of "from x to y"
            int x = std::stoi(words[2]);
            int y = std::stoi(words[5]);
            move(y, x, password2);
        }
    }
    std::cout<<"[Part 2] Answer: "<<password2<<std::endl;
    return 0;
}
