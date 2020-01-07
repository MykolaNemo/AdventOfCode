#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
  std::vector<int> moduleMasses;

  std::ifstream infile("input.txt");
  std::string line;
  while (std::getline(infile, line))
  {
    std::istringstream iss(line);
    int moduleMass = 0;
    if (!(iss >> moduleMass))
    {
      std::cout << "Error!" << std::endl;
      break;
    } // error
    moduleMasses.push_back(moduleMass);
  }
  int totalFuel = 0;
  for(int mass : moduleMasses)
  {
    int fuel = 0;
    int m = mass;
    do
    {
      int f = m/3-2;
      f = (f < 0) ? 0 : f;
      m = f;
      fuel += f;
    }
    while(m > 0);
    totalFuel += fuel;
  }
  std::cout<<totalFuel<<std::endl;
  return 0;
}
