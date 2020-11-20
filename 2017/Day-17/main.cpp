#include <iostream>
#include <vector>

int main()
{
    int steps = 369; //input
    {
        //Part 1
        std::vector<int> v = {0};
        int pos = 0;
        for(int i = 1; i <= 2017; ++i)
        {
            pos = (pos + steps) % v.size();
            auto it = v.begin();
            std::advance(it, pos+1);
            v.insert(it, i);
            pos++;
        }
        std::cout<<"[Part 1] Answer: "<<v[pos+1]<<std::endl;
    }
    {
        //Part 2
        int pos = 0;
        int size = 1;
        int answer = 0;
        for(int i = 0; i < 50000000; ++i)
        {
            pos = ((pos + steps) % size)+1;
            size++;
            if(pos == 1)
            {
                answer = i+1;
            }
        }
        std::cout<<"[Part 2] Answer: "<<answer<<std::endl;
    }
    return 0;
}
