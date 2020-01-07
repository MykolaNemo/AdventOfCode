#include <iostream>
#include "Computer.h"
#include <fstream>
#include <string>
#include <thread>

using namespace std;

int main()
{
  std::ifstream infile("input.txt");
  std::vector<long long> program;

  std::string code;
  while(std::getline(infile, code, ','))
  {
    program.push_back(std::stol(code));
  }

  Computer camera(0);
  camera.setProgram(program);
//  camera.setWaitAfterOutput(true, 3);

  std::vector<long long> input = {'A',',','B',',','B',',','A',',','C',',','A',',','A',',','C',',','B',',','C',
                                  '\n',
                                  'R',',','8',',','L',',','1','2',',','R',',','8',/*function body A*/
                                  '\n',
                                  'R',',','1','2',',','L',',','8',',','R',',','1','0',/*function body B*/
                                  '\n',
                                  'R',',','8',',','L',',','8',',','L',',','8',',','R',',','8',',','R',',','1','0',/*function body C*/
                                  '\n',
                                  'n',/*continuous video feed*/
                                  '\n'};
  std::vector<long long> output;
  std::vector<std::vector<char>> image;

  while(camera.state() != Computer::Halted)
  {
    camera.start(input, output);

    std::vector<char> temp;
    for(long long &c : output)
    {
      temp.push_back((char)c);
      if((char)c == '\n')
      {
        image.push_back(temp);
        temp.clear();
      }
    }
    for(auto row : image)
    {
      for(auto col : row)
      {
        std::cout<<col;
      }
    }
    std::cout<<"Result: "<<output[output.size()-1]<<std::endl;
  }

//  int sum = 0;
//  for(int row = 1; row < image.size()-1;++row)
//  {
//    for(int col = 1; col < image[row].size()-1;++col)
//    {
//      if(image[row][col] == '#')
//      {
//        if((image[row-1][col] == '#') && (image[row+1][col] == '#') &&
//           (image[row][col-1] == '#') && (image[row][col+1] == '#'))
//        {
//          image[row][col] = 'O';
//          sum += row*col;
//        }
//      }
//    }
//  }
//  for(auto row : image)
//  {
//    for(auto col : row)
//    {
//      std::cout<<col;
//    }
//  }

//  std::cout<<sum<<std::endl;
  return 0;
}
