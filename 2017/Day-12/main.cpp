#include <iostream>
#include "../../utils.h"
#include <map>

struct Node
{
    Node(){}
    Node(int _id):id(_id){}

    int id = 0;
    bool visited = false;
};

std::vector<Node*> allNodes;
std::map<Node*, std::vector<Node*>> pipes;

Node* nodeById(int id)
{
    auto it = std::find_if(allNodes.begin(), allNodes.end(), [&id](Node* n)->bool{return n->id == id;});
    if(it != allNodes.end())
    {
        return *it;
    }
    else
    {
        Node* n = new Node(id);
        allNodes.push_back(n);
        return n;
    }
}

int main()
{
    auto content = readFile("input.txt");
    for(const auto& pipeString : content)
    {
        StringVector words = split(pipeString, " ");

        Node* leftNode = nodeById(std::stoi(words[0]));
        std::vector<Node*> rightNodes;
        for(int i = 2; i < static_cast<int>(words.size()); ++i)
        {
            rightNodes.push_back(nodeById(std::stoi(words[i])));
        }

        pipes[leftNode] = rightNodes;
    }

    {
        int visited = 0;
        std::vector<Node*> nodesToVisit = {nodeById(0)};
        while(!nodesToVisit.empty())
        {
            Node* currentNode = nodesToVisit.front();
            erase<Node*>(nodesToVisit, currentNode);
            currentNode->visited = true;
            visited++;

            for(auto otherNode : pipes[currentNode])
            {
                if(!otherNode->visited)
                {
                    nodesToVisit.push_back(otherNode);
                }
            }
        }
        std::cout<<"[Part 1] Answer: "<<visited<<std::endl;
    }

    {
        int groups = 0;
        auto nodes = allNodes;
        while(!nodes.empty())
        {
            std::vector<Node*> nodesToVisit = {nodes.front()};
            while(!nodesToVisit.empty())
            {
                Node* currentNode = nodesToVisit.front();
                erase<Node*>(nodesToVisit, currentNode);
                currentNode->visited = true;

                for(auto otherNode : pipes[currentNode])
                {
                    if(!otherNode->visited)
                    {
                        nodesToVisit.push_back(otherNode);
                    }
                }
            }
            groups++;
            nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [](Node* n)->bool{return n->visited;}), nodes.end());
        }
        std::cout<<"[Part 2] Answer: "<<groups<<std::endl;
    }
    return 0;
}
