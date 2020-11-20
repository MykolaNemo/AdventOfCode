#include <iostream>
#include "../../utils.h"

struct Tree
{
    std::string name;
    int weight = 0;
    std::vector<Tree*> children;
    Tree* parent = nullptr;
};

std::vector<Tree*> allNodes;

Tree* findNodeByName(const std::string& name)
{
    auto it = std::find_if(allNodes.begin(), allNodes.end(), [name](Tree* node)->bool{
            return (node->name == name);
    });
    if(it != allNodes.end())
    {
        return *it;
    }
    return nullptr;
}

Tree* findParentNode(Tree* childNode)
{
    auto it = std::find_if(allNodes.begin(), allNodes.end(), [childNode](Tree* parentNode)->bool{
        for(Tree* child : parentNode->children)
        {
            if(child == childNode)
            {
                return true;
            }
        }
        return false;
    });
    if(it != allNodes.end())
    {
        return (*it);
    }
    return nullptr;
}

Tree* newNode(const std::string& name)
{
    Tree* node = new Tree();
    node->name = name;
    allNodes.push_back(node);
    return node;
}

//kpjxln (44)
//kpjxln (44) -> dzzbvkv, gzdxgvj, wsocb, jidxg
Tree* parseNode(std::string stringNode)
{
    StringVector words = split(stringNode, " ");

    std::string name = words[0];
    Tree* currentNode = findNodeByName(name);
    if(!currentNode)
    {
        currentNode = newNode(name);
    }
    currentNode->weight = std::stoi(words[1].substr(1, words[1].size()-2));

    auto parentNode = findParentNode(currentNode);
    if(parentNode)
    {
        currentNode->parent = parentNode;
    }

    if(words.size() > 3)
    {
        for(int i = 3; i < static_cast<int>(words.size()); ++i)
        {
            std::string childName = words[i];
            if(childName.back() == ',')
            {
                childName.erase(childName.size()-1, 1);
            }
            Tree* childNode = findNodeByName(childName);
            if(!childNode)
            {
                childNode = newNode(childName);
            }
            currentNode->children.push_back(childNode);
            childNode->parent = currentNode;
        }
    }
    return currentNode;
}

int traverseTree(Tree* node)
{
    int weight = node->weight;
    std::vector<Tree*> childrenNodes = node->children;
    while(!childrenNodes.empty())
    {
        Tree* child = childrenNodes.front();
        erase<Tree*>(childrenNodes, child);

        weight += child->weight;
        for(Tree* grandChild : child->children)
        {
            childrenNodes.push_back(grandChild);
        }
    }
    return weight;
}

int findUnbalancedItem(std::vector<int> weights)
{
    for(int i = 0; i < static_cast<int>(weights.size())-1; ++i)
    {
        int weight = weights[i];
        auto it = std::find(weights.begin()+i+1, weights.end(), weight);
        if(it == weights.end())
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    StringVector content = readFile("input.txt");
    for(const auto& row : content)
    {
        parseNode(row);
    }

    Tree* rootNode = nullptr;
    auto it = std::find_if(allNodes.begin(), allNodes.end(), [](Tree* node)->bool{
        return (node->parent == nullptr);
    });
    if(it != allNodes.end())
    {
        rootNode = *it;
        std::cout<<"[Part 1] Answer: "<<rootNode->name<<std::endl;
    }

    Tree* currentRootNode = rootNode;
    int diff = -1;
    while(currentRootNode)
    {
        std::vector<int> subTowersWeight;
        for(Tree* subTower : currentRootNode->children)
        {
            subTowersWeight.push_back(traverseTree(subTower));
        }
        int index = findUnbalancedItem(subTowersWeight);
        if(index != -1)
        {
            if(index == 0)
            {
                diff = (diff == -1) ? (subTowersWeight[index] - subTowersWeight[1]) : diff;
            }
            else
            {
                diff = (diff == -1) ? (subTowersWeight[index] - subTowersWeight[0]) : diff;
            }
            currentRootNode = currentRootNode->children[index];
        }
        else
        {
            std::cout<<"[Part 2] Answer: "<<currentRootNode->weight - diff<<std::endl;
            return 0;
        }
    }

    return 0;
}
