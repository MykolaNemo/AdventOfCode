#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

void runCode(std::vector<int>& opcodes)
{
  int pos = 0;
  while(pos < opcodes.size())
  {
    int code = opcodes[pos];
    switch (code)
    {
    case 1:
      opcodes[opcodes[pos+3]] = opcodes[opcodes[pos+1]] + opcodes[opcodes[pos+2]];
      pos += 4;
      break;
    case 2:
      opcodes[opcodes[pos+3]] = opcodes[opcodes[pos+1]] * opcodes[opcodes[pos+2]];
      pos += 4;
      break;
    case 99:
      return;
    default:
      std::cout<<"Error!"<<std::endl;
      return;
    }
  }
}

void part1(std::vector<int> program)
{
  program[1] = 12;
  program[2] = 2;
  runCode(program);
  std::cout<<"[Part1] Answer: "<<program[0]<<std::endl;
}

void part2(std::vector<int> program)
{
  for(int verb = 0; verb <= 99; ++verb)
  {
    for(int noun = 0; noun <= 99; ++noun)
    {
      std::vector<int> programCopy = program;
      programCopy[1] = verb;
      programCopy[2] = noun;
      runCode(programCopy);
      if(programCopy[0] == 19690720)
      {
        std::cout<<"[Part2] Answer: "<<100*verb+noun<<std::endl;
      }
    }
  }
}

int main()
{
  std::ifstream infile("input.txt");
  std::vector<int> program;
  std::string code;

  while(std::getline(infile, code, ',') )
  {
    program.push_back(std::stoi(code));
  }

  part1(program);
  part2(program);

  return 0;
}


//Opcodes (like 1, 2, or 99) mark the beginning of an instruction. The values used immediately after an opcode, if any, are called the instruction's parameters. For example, in the instruction 1,2,3,4, 1 is the opcode; 2, 3, and 4 are the parameters. The instruction 99 contains only an opcode and has no parameters.

//The address of the current instruction is called the instruction pointer; it starts at 0. After an instruction finishes, the instruction pointer increases by the number of values in the instruction; until you add more instructions to the computer, this is always 4 (1 opcode + 3 parameters) for the add and multiply instructions. (The halt instruction would increase the instruction pointer by 1, but it halts the program instead.)

//"With terminology out of the way, we're ready to proceed.
//To complete the gravity assist, you need to determine what pair of inputs produces the output 19690720."

//The inputs should still be provided to the program by replacing the values at addresses 1 and 2, just like before.
// In this program, the value placed in address 1 is called the noun, and the value placed in address 2 is called the verb. Each of the two input values will be between 0 and 99, inclusive.

//Once the program has halted, its output is available at address 0, also just like before. Each time you try a pair of inputs, make sure you first reset the computer's memory to the values in the program (your puzzle input) - in other words, don't reuse memory from a previous attempt.

//Find the input noun and verb that cause the program to produce the output 19690720. What is 100 * noun + verb? (For example, if noun=12 and verb=2, the answer would be 1202.)

//Your puzzle answer was 7733.