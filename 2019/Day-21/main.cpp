#include <iostream>
#include <fstream>
#include <string>
#include "Computer.h"

using namespace std;

bool TRegister = false;
bool JRegister = false;
//A1
//B2
//C3
//D4
//E5
//F6
//G7
//H8
//I9


//AND X Y sets Y to true if both X and Y are true; otherwise, it sets Y to false.
//OR X Y sets Y to true if at least one of X or Y is true; otherwise, it sets Y to false.
//NOT X Y sets Y to true if X is false; otherwise, it sets Y to false.

//In all three instructions, the second argument (Y) needs to be a writable register (either T or J).
//The first argument (X) can be any register (including A, B, C, or D).

//#define PART1;
#ifdef PART1
std::string inputString = "NOT C J\n"//hole3
                          "AND D J\n"//hole3&ground4
                          "NOT A T\n"//hole1
                          "AND D T\n"//hole1&ground4
                          "OR T J\n"//hole3&ground4 || hole1&ground4
                          "WALK\n";
#else
//std::string inputString = "NOT E T\n"//hole5
//                          "OR H J\n"//ground8
//                          "AND J T\n"//hole5&ground8
//                          "AND D T\n"//ground4&hole5&ground8
//                          "AND F T\n"//ground4&hole5&ground6&ground8

//                          "NOT C J\n"//hole3
//                          "AND J T\n"//hole3&ground4&hole5&ground6&ground8
//                          "NOT G J\n"//hole7
//                          "AND T J\n"//hole3&ground4&hole5&ground6&hole7&ground8

//                          "NOT C T\n"//hole3
//                          "AND D T\n"//ground4&ground8
//                          "AND G T\n"//ground4&ground8&ground9
//                          "OR T J\n"

//                          "NOT A T\n"//hole1
//                          "AND D T\n"//hole1&ground4
//                          "OR T J\n"
//                          "RUN\n";
// @
// #####.#.#.
//(3 * 11 * 2)
std::string inputString =
        "NOT A J\n"
        "NOT B T\n"
        "OR T J\n"
        "NOT C T\n"
        "OR T J\n"
        "AND D J\n"

        "NOT E T\n"
        "AND H T\n"
        "OR E T\n"

        "AND T J\n"
        "RUN\n";
//                           "OR D J\n"
//                           "RUN\n";
#endif

///       @
///    #####.###..#.####
///    #####.###########
///     #####.#..########
///     #####.#.#########
///   #####.#.##...####

int main()
{
    std::ifstream infile("input.txt");
    std::vector<long long> program;

    std::string code;
    while(std::getline(infile, code, ','))
    {
      program.push_back(std::stol(code));
    }

    std::vector<long long> input;
    std::vector<long long> output;
    Computer springDroid(0);
    springDroid.setProgram(program);
    springDroid.setWaitForInput(true);
    while(springDroid.state() != Computer::State::Halted)
    {
        springDroid.start(input, output);
        std::string str;
        std::vector<long long> number;
        for(auto& o : output)
        {
            str += (char)o;
            number.push_back((long long)o);
        }
        output.clear();

        std::cout<<"<"<<str<<">\n";
        for(auto& n: number)
        {
            std::cout<<"<-"<<n<<"->";
        }
        std::cout.flush();

        for(char c : inputString)
        {
            input.push_back((long long)c);
        }
        inputString.clear();
    }
    return 0;
}
