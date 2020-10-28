#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

std::vector<std::vector<int>> screen = []()->std::vector<std::vector<int>>{
    std::vector<std::vector<int>> v;
    for(int i = 0; i < 6; ++i)
    {
        std::vector<int> v2;
        for(int i = 0; i < 50; ++i)
        {
            v2.push_back(0);
        }
        v.push_back(v2);
    }
    return v;
}();

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

int main()
{
    std::fstream infile("input.txt");
    std::vector<std::string> operations;
    {
        std::string operation;
        while(std::getline(infile, operation))
        {
            operations.push_back(operation);
        }
    }
    for(auto op : operations)
    {
        std::vector<std::string> words = split(op, " ");
        if(words[0] == "rotate")
        {
            auto pos = words[2].find_first_of('=');
            int A = std::stoi(words[2].substr(pos+1, words[2].size() - pos - 1));
            int B = std::stoi(words[4]);
            if(words[1] == "row")
            {
                std::vector<int> row = screen[A];
                std::vector<int> newRow;
                for(int i = 0; i < row.size(); ++i)
                {
                    int index = i - B;
                    while(index < 0)
                    {
                        index += row.size();
                    }
                    newRow.push_back(row[index]);
                }
                screen[A] = newRow;
            }
            else if(words[1] == "column")
            {
                std::vector<int> newColumn;
                for(int i = 0; i < screen.size(); ++i)
                {
                    int index = i - B;
                    while(index < 0)
                    {
                        index += screen.size();
                    }
                    newColumn.push_back(screen[index][A]);
                }
                for(int i = 0; i < newColumn.size(); ++i)
                {
                    screen[i][A] = newColumn[i];
                }
            }
        }
        else if(words[0] == "rect")
        {
            auto pos = words[1].find_first_of('x');
            int A = std::stoi(words[1].substr(0, pos));
            int B = std::stoi(words[1].substr(pos+1, words[1].size() - pos - 1));
            for(int i = 0; i < A; ++i)
            {
                for(int j = 0; j < B; ++j)
                {
                    screen[j][i] = 1;
                }
            }
        }
    }
    int n = 0;
    for(int i = 0; i < screen.size(); ++i)
    {
        for(int j = 0; j < screen[i].size(); ++j)
        {
            if(screen[i][j])
            {
                n++;
                std::cout<<"#";
            }
            else
            {
                std::cout<<" ";
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<"Answer: "<<n<<std::endl;
    return 0;
}
