#include <iostream>
#include <vector>
#include "../../utils.h"

struct Layer
{
    int sensorPos = 0;
    bool sensorGoDown = true;
    int depth = 0;
    int range = 0;
};

std::vector<Layer> layers;

void moveSensors()
{
    for(auto& layer : layers)
    {
        if(layer.sensorGoDown)
        {
            if(layer.sensorPos < layer.range-1)
            {
                layer.sensorPos++;
            }
            else
            {
                layer.sensorPos--;
                layer.sensorGoDown = false;
            }
        }
        else
        {
            if(layer.sensorPos > 0)
            {
                layer.sensorPos--;
            }
            else
            {
                layer.sensorPos++;
                layer.sensorGoDown = true;
            }
        }
    }
}

int main()
{
    auto content = readFile("input.txt");
    int maxDepth = 0;
    for(auto str : content)
    {
        auto words = split(str, ": ");
        Layer layer;
        layer.depth = std::stoi(words[0]);
        layer.range = std::stoi(words[1]);
        layers.push_back(layer);
        maxDepth = std::max(maxDepth, layer.depth);
    }
    {
        int severity = 0;
        for(int depth = 0; depth <= maxDepth; ++depth)//98: max depth
        {
            auto it = std::find_if(layers.begin(), layers.end(), [&depth](const Layer& l)->bool {
                return l.depth == depth;
            });
            if(it != layers.end() && (it->sensorPos == 0))
            {
                severity += it->depth*it->range;
            }
            moveSensors();
        }
        std::cout<<"[Part 1] Answer: "<<severity<<std::endl;
    }

    {
        int delay = 0;
        bool caught = false;
        do
        {
            caught = false;
            for(const auto& layer : layers)
            {
                int pos = (layer.depth+delay) % ((layer.range-1)*2);
                if(pos >= layer.range)
                {
                    pos = 2*(layer.range - 1) - pos;
                }
                if(pos == 0)
                {
                    caught = true;
                    delay++;
                    break;
                }
            }
        }
        while (caught);
        std::cout<<"[Part 2] Answer: "<<delay<<std::endl;
    }
    return 0;
}
