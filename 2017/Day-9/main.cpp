#include <iostream>
#include <algorithm>
#include <functional>
#include "../../utils.h"

struct Group: public Tree
{
    int score = 1;
};

int treeScore = 0;
void sumScore(Tree* node)
{
    treeScore += ((Group*)node)->score;
}

int main()
{
    std::string input = readFile("input.txt")[0];//one line

    Group* rootGroup = new Group;
    Group* currentGroup = rootGroup;
    size_t pos = 1;
    bool trash = false;
    int trashCount = 0;
    while(pos < input.size() && currentGroup)
    {
        switch (input[pos]) {
        case '{':
        {
            if(trash)
            {
                trashCount++;
                break;
            }
            Group* newGroup = new Group;
            newGroup->score = currentGroup->score+1;
            newGroup->parent = currentGroup;
            currentGroup->children.push_back(newGroup);
            currentGroup = newGroup;
            break;
        }
        case '}':
        {
            if(trash)
            {
                trashCount++;
                break;
            }
            currentGroup = (Group*)(currentGroup->parent);
            break;
        }
        case '<':
        {
            if(trash)
            {
                trashCount++;
                break;
            }
            trash = true;
            break;
        }
        case '>':
        {
            trash = false;
            break;
        }
        case '!':
        {
            ++pos;
            break;
        }
        default:
        {
            if(trash)
            {
                trashCount++;
            }
            break;
        }
        }
        ++pos;
    }
    traverseTree(rootGroup, sumScore);
    std::cout<<"[Part 1] Answer: "<<treeScore<<std::endl;
    std::cout<<"[Part 2] Answer: "<<trashCount<<std::endl;
    return 0;
}
