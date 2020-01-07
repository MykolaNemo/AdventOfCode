#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>

class Computer
{
public:

    enum InstructionMode
    {
        Position = 0,
        Immediate = 1,
        Relative = 2
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

    void setProgram(const std::vector<long long>& instructions)
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

    void start(std::vector<long long>& input, std::vector<long long> &output)
    {
        mState = Running;
        while(true)
        {
            long long value = mInstructions[mPointer];

            long long code = value%100L;
            InstructionMode mode1 = (InstructionMode)((value/100)%10);
            InstructionMode mode2 = (InstructionMode)((value/1000)%10);
            InstructionMode mode3 = (InstructionMode)(value/10000);
            if((mode1 < 0 || mode2 < 0 || mode3 < 0) || (mode1 > 2 || mode2 > 2 || mode3 > 2))
            {
                std::cout<<"Incorrect mode!"<<std::endl;
                throw std::exception();
            }
            switch (code)
            {
            case 1:
            {
                long long op1 = parameter(1, mode1);
                long long op2 = parameter(2, mode2);
                long long &op3 = parameter(3, mode3);
                op3 = op1 + op2;
                mPointer += 4;
                break;
            }
            case 2:
            {
              long long op1 = parameter(1, mode1);
              long long op2 = parameter(2, mode2);
              long long &op3 = parameter(3, mode3);
                op3 = op1 * op2;
                mPointer += 4;
                break;
            }
            case 3:
            {
                if(!input.empty())
                {
                    long long &op1 = parameter(1, mode1);
                    op1 = input[0];
                    input.erase(input.begin());
                    mPointer += 2;
                }
                else if(mWaitForInput)
                {
                    mState = Suspended;
                    return;
                }
                break;
            }
            case 4:
            {
              long long op1 = parameter(1, mode1);
              output.push_back(op1);
              mPointer += 2;
              break;
            }
            case 5:
            {
                long long op1 = parameter(1, mode1);
                long long op2 = parameter(2, mode2);
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
                long long op1 = parameter(1, mode1);
                long long op2 = parameter(2, mode2);
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
              long long op1 = parameter(1, mode1);
              long long op2 = parameter(2, mode2);
              long long &op3 = parameter(3, mode3);
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
              long long op1 = parameter(1, mode1);
              long long op2 = parameter(2, mode2);
              long long &op3 = parameter(3, mode3);
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
            case 9:
            {
              long long op1 = parameter(1, mode1);
              mRelativeBase += op1;
              mPointer += 2;
              break;
            }
            case 99:
            {
                mState = Halted;
                return;
            }
            default:
              std::cout<<"Unknown instruction: "<<code<<std::endl;
               throw std::exception();
            }
        }
    }

    void setWaitForInput(bool waitForInput)
    {
      mWaitForInput = waitForInput;
    }

private:
    long long paramPos(const long long& pointer, const InstructionMode& mode)
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
        case Relative:
        {
          return mInstructions[pointer] + mRelativeBase;
        }
        default:
        {
          std::cout<<"paramPos: Incorrect mode!"<<std::endl;
          throw std::exception();
        }
        }
    }

    long long& parameter(const int &paramNumber, const InstructionMode &mode)
    {
      long long pos = paramPos(mPointer+paramNumber, mode);
      int oldSize = mInstructions.size();
      while(pos >= mInstructions.size())
      {
        mInstructions.resize(mInstructions.size()*2);
        for(int i = oldSize; i < mInstructions.size(); ++i)
        {
          mInstructions[i] = 0;
        }
      }
      return mInstructions[pos];
    }

    std::vector<long long> mInstructions;
    long long mPointer = 0;
    int mId;
    State mState;
    long long mRelativeBase = 0;
    bool mWaitForInput = false;
};
#endif // COMPUTER_H
