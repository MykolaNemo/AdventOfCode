#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>


enum Mode
{
    Position = 0,
    Immediate = 1
};
class Computer
{
public:

    enum InstructionMode
    {
        Position = 0,
        Immediate = 1
    };

    enum State
    {
        NotRunning = 0,
        Running = 1,
        Suspended = 2,
        Halted = 3
    };
    Computer(int id):
        mId(id)
    {
        mState = NotRunning;
    }

    void setProgram(const std::vector<int>& instructions)
    {
        mInstructions = instructions;
        mState = NotRunning;
    }

    void clearPointer()
    {
        mPointer = 0;
    }

    State state() const
    {
        return mState;
    }

    void start(std::vector<int>& input, int &output)
    {
        mState = Running;
        while(true)
        {
            int value = mInstructions[mPointer];

            int code = value%100;
            InstructionMode mode1 = (InstructionMode)((value/100)%10);
            InstructionMode mode2 = (InstructionMode)((value/1000)%10);
            InstructionMode mode3 = (InstructionMode)(value/10000);
            if((mode1 < 0 || mode2 < 0 || mode3 < 0) || (mode1 > 1 || mode2 > 1 || mode3 > 1))
            {
                std::cout<<"Incorrect mode!"<<std::endl;
                throw std::exception();
            }
            switch (code)
            {
            case 1:
            {
                int op1 = mInstructions[paramPos(mPointer+1, mode1)];
                int op2 = mInstructions[paramPos(mPointer+2, mode2)];
                int &op3 = mInstructions[paramPos(mPointer+3, mode3)];
                op3 = op1 + op2;
                mPointer += 4;
                break;
            }
            case 2:
            {
                int op1 = mInstructions[paramPos(mPointer+1, mode1)];
                int op2 = mInstructions[paramPos(mPointer+2, mode2)];
                int &op3 = mInstructions[paramPos(mPointer+3, mode3)];
                op3 = op1 * op2;
                mPointer += 4;
                break;
            }
            case 3:
            {
                if(!input.empty())
                {
                    int &op1 = mInstructions[paramPos(mPointer+1, mode3)];
                    op1 = input[0];
                    input.erase(input.begin());
                    mPointer += 2;
                }
                else
                {
                    mState = Suspended;
                    return;
                }
                break;
            }
            case 4:
            {
                int op1 = mInstructions[paramPos(mPointer+1, mode1)];
                output = op1;
                mPointer += 2;
                mState = Suspended;
                return;
            }
            case 5:
            {
                int op1 = mInstructions[paramPos(mPointer+1, mode1)];
                int op2 = mInstructions[paramPos(mPointer+2, mode2)];
                if(op1 != 0)
                {
                    mPointer = op2;
                }
                else
                {
                    mPointer += 3;
                }
                break;
            }
            case 6:
            {
                int op1 = mInstructions[paramPos(mPointer+1, mode1)];
                int op2 = mInstructions[paramPos(mPointer+2, mode2)];
                if(op1 == 0)
                {
                    mPointer = op2;
                }
                else
                {
                    mPointer += 3;
                }
                break;
            }
            case 7:
            {
                int op1 = mInstructions[paramPos(mPointer+1, mode1)];
                int op2 = mInstructions[paramPos(mPointer+2, mode2)];
                int &op3 = mInstructions[paramPos(mPointer+3, mode3)];
                if(op1 < op2)
                {
                    op3 = 1;
                }
                else
                {
                    op3 = 0;
                }
                mPointer += 4;
                break;
            }
            case 8:
            {
                int op1 = mInstructions[paramPos(mPointer+1, mode1)];
                int op2 = mInstructions[paramPos(mPointer+2, mode2)];
                int &op3 = mInstructions[paramPos(mPointer+3, mode3)];
                if(op1 == op2)
                {
                    op3 = 1;
                }
                else
                {
                    op3 = 0;
                }
                mPointer += 4;
                break;
            }
            case 99:
            {
                mState = Halted;
                return;
            }
            default:
               throw std::exception();
            }
        }
    }

private:
    int paramPos(int pointer, const InstructionMode& mode)
    {
        switch(mode)
        {
        case Position:
        {
            return mInstructions[pointer];
        }
        case Immediate:
        {
            return pointer;
        }
        }
        std::cout<<"paramPos: Incorrect mode!"<<std::endl;
        throw std::exception();
    }

    std::vector<int> mInstructions;
    int mPointer = 0;
    int mId;
    State mState;
};

int main()
{
    std::ifstream infile("input.txt");
    std::vector<int> program;

    std::string code;
    while(std::getline(infile, code, ','))
    {
      program.push_back(std::stoi(code));
    }

    std::vector<Computer> machines;
    for(int i = 0; i < 5; ++i)
    {
        Computer m(i);
        m.setProgram(program);
        machines.push_back(m);
    }

    std::vector<int> phases;
    int maxOutput = 0;
    for(int i1 = 5; i1 <= 9; ++i1)
    {
        for(int i2 = 5; i2 <= 9; ++i2)
        {
            if(i2 == i1) continue;
            for(int i3 = 5; i3 <= 9; ++i3)
            {
                if(i3 == i2 || i3 == i1) continue;
                for(int i4 = 5; i4 <= 9; ++i4)
                {
                    if(i4 == i3 || i4 == i2 || i4 == i1) continue;
                    for(int i5 = 5; i5 <= 9; ++i5)
                    {
                        if(i5 == i4 || i5 == i3 || i5 == i2 || i5 == i1) continue;

                        phases = {i1,i2,i3,i4,i5};

                        int output = 0;
                        int input = 0;
                        int amplifierNo = 0;
                        bool firstLoop = true;
                        for(int i = 0; i < machines.size(); ++i)
                        {
                            machines[i].setProgram(program);
                            machines[i].clearPointer();
                        }

                        while(machines[4].state() != Computer::Halted)
                        {
                            std::vector<int> inputVector;
                            if(firstLoop)
                            {
                                inputVector.push_back(phases[amplifierNo]);
                            }
                            inputVector.push_back(input);

                            machines[amplifierNo].start(inputVector, output);
                            input = output;

                            amplifierNo++;
                            if(amplifierNo > 4)
                            {
                                amplifierNo = 0;
                                firstLoop = false;
                            }
                        }
                        if(output > maxOutput)
                        {
                            maxOutput = output;
                        }
                    }
                }
            }
        }
    }
    std::cout<<"Output: "<<maxOutput<<std::endl;

    return 0;
}
