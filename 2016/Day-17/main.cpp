#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "md5.h"

//#define PART1

std::string passcode = "veumntbg";

struct Path
{
    Path(std::string _path, int _x, int _y):path(_path), x(_x), y(_y){}
    std::string path;
    int x = 0;
    int y = 0;
    friend bool operator==(const Path& rhs, const Path& lhs)
    {
        return (rhs.path == lhs.path) && (rhs.x == lhs.x) && (rhs.y == lhs.y);
    }
};

int main()
{
    int maxLength = 0;
    int finalX = 3;
    int finalY = 3;
    Path path(std::string(), 0, 0);
    std::vector<Path> paths;
    paths.push_back(path);

    while(!paths.empty())
    {
        Path path = paths[0];
        paths.erase(std::remove(paths.begin(), paths.end(), path));

        MD5 md5(passcode + path.path);
        md5.finalize();
        std::string md5String = md5.hexdigest();
        bool u = (md5String[0] >= 'b') && (md5String[0] <= 'f');
        bool d = (md5String[1] >= 'b') && (md5String[1] <= 'f');
        bool l = (md5String[2] >= 'b') && (md5String[2] <= 'f');
        bool r = (md5String[3] >= 'b') && (md5String[3] <= 'f');

        if(u && path.y > 0)
        {
            paths.push_back(Path(path.path + "U", path.x, path.y-1));
        }

        if(d && path.y < 3)
        {
            Path newPath(path.path + "D", path.x, path.y+1);
            if(newPath.x == finalX && newPath.y == finalY)
            {
                maxLength = std::max(static_cast<int>(newPath.path.size()), maxLength);
#ifdef PART1
                std::cout<<"[Part 1] Answer: "<<newPath.path<<std::endl;
                return 0;
#endif
            }
            else
            {
                paths.push_back(newPath);
            }
        }

        if(l && path.x > 0)
        {
            paths.push_back(Path(path.path + "L", path.x-1, path.y));
        }

        if(r && path.x < 3)
        {
            Path newPath(path.path + "R", path.x+1, path.y);
            if(newPath.x == finalX && newPath.y == finalY)
            {
                maxLength = std::max(static_cast<int>(newPath.path.size()), maxLength);
#ifdef PART1
                std::cout<<"[Part 1] Answer: "<<newPath.path<<std::endl;
                return 0;
#endif
            }
            else
            {
                paths.push_back(newPath);
            }
        }
    }
    std::cout<<"[Part 2] Answer: "<<maxLength<<std::endl;
    return 0;
}
