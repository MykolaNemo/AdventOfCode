#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "md5.h"

//#define PART1

int main()
{
    std::string input = "uqwqemis";
    int n = 0;
#ifdef PART1
    std::string password;
#else
    std::string password = "________";
#endif
    while(true)
    {
        MD5 md5(input + std::to_string(n));
        md5.finalize();
        std::string md5String = md5.hexdigest();
#ifdef PART1
        if(md5String.substr(0,5) == "00000")
        {
            password += md5String[5];
        }
        if(password.size() == 8)
        {
            break;
        }

#else
        if(md5String.substr(0,5) == "00000")
        {
            if((md5String[5] >= '0') && (md5String[5] <= '7') && (password[md5String[5] - '0'] == '_'))
            {
                password[md5String[5] - '0'] = md5String[6];
                std::cout<<n<<" "<<password<<std::endl;
            }
        }
        bool ready = true;
        for(const auto& c : password)
        {
            if(c == '_')
            {
                ready = false;
                break;
            }
        }
        if(ready) break;
#endif
        n++;
//        if(n % 10000 == 0)
//        {
//            std::cout<<n<<" "<<password<<std::endl;
//        }
    }
    std::cout<<"Answer: "<<password<<std::endl;
    return 0;
}
