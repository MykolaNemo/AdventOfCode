#include <iostream>
#include "../../utils.h"

struct Node
{
    std::string name;
    int weight = 0;
    std::vector<Node*> children;
    Node* parent = nullptr;
};

std::vector<Node*> allNodes;

Node* findNodeByName(const std::string& name)
{
    auto it = std::find_if(allNodes.begin(), allNodes.end(), [name](Node* node)->bool{
            return (node->name == name);
    });
    if(it != allNodes.end())
    {
        return *it;
    }
    return nullptr;
}

Node* findParentNode(Node* childNode)
{
    auto it = std::find_if(allNodes.begin(), allNodes.end(), [childNode](Node* parentNode)->bool{
        for(Node* child : parentNode->children)
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

Node* newNode(const std::string& name)
{
    Node* node = new Node();
    node->name = name;
    allNodes.push_back(node);
    return node;
}

//kpjxln (44)
//kpjxln (44) -> dzzbvkv, gzdxgvj, wsocb, jidxg
Node* parseNode(std::string stringNode)
{
    StringVector words = split(stringNode, " ");

    std::string name = words[0];
    Node* currentNode = findNodeByName(name);
    if(!currentNode)
    {
        currentNode = newNode(name);
    }
    currentNode->weight = std::stoi(words[1].substr(1, words[1].size()-2));

    auto parentNode = findParentNode(currentNode);
    if(parentNode)
    {
        parentNode->children.push_back(currentNode);
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
            Node* childNode = findNodeByName(childName);
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

int main()
{
    StringVector content = readFile("input.txt");
    for(const auto& row : content)
    {
        parseNode(row);
    }

    auto it = std::find_if(allNodes.begin(), allNodes.end(), [](Node* node)->bool{
        return (node->parent == nullptr);
    });
    if(it != allNodes.end())
    {
        std::cout<<"[Part 1] Answer: "<<(*it)->name<<std::endl;
    }
    return 0;
}
