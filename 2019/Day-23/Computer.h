#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>
#include <iostream>
#include <map>

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

      long long code = value%100LL;
      InstructionMode mode1 = (InstructionMode)((value/100LL)%10LL);
      InstructionMode mode2 = (InstructionMode)((value/1000LL)%10LL);
      InstructionMode mode3 = (InstructionMode)(value/10000LL);
      if((mode1 < 0 || mode2 < 0 || mode3 < 0) || (mode1 > 2 || mode2 > 2 || mode3 > 2))
      {
        std::cout<<"Incorrect mode!"<<std::endl;
        throw std::exception();
      }
      switch (code)
      {
      case 1:
      {
        auto op1 = parameter(1, mode1);
        auto op2 = parameter(2, mode2);
        auto &op3 = parameter(3, mode3);
        op3 = op1 + op2;
        mPointer += 4;
        break;
      }
      case 2:
      {
        auto op1 = parameter(1, mode1);
        auto op2 = parameter(2, mode2);
        auto &op3 = parameter(3, mode3);
        op3 = op1 * op2;
        mPointer += 4;
        break;
      }
      case 3:
      {
        if(!input.empty())
        {
          auto &op1 = parameter(1, mode1);
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
        auto op1 = parameter(1, mode1);
        output.push_back(op1);
        mPointer += 2;

        if(mWaitAfterOutput)
        {
          if(output.size() == mWaitAfterOutputCounter)
          {
            mState = Suspended;
            return;
          }
        }
        break;
      }
      case 5:
      {
        auto op1 = parameter(1, mode1);
        auto op2 = parameter(2, mode2);
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
        auto op1 = parameter(1, mode1);
        auto op2 = parameter(2, mode2);
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
        auto op1 = parameter(1, mode1);
        auto op2 = parameter(2, mode2);
        auto &op3 = parameter(3, mode3);
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
        auto op1 = parameter(1, mode1);
        auto op2 = parameter(2, mode2);
        auto &op3 = parameter(3, mode3);
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
        auto op1 = parameter(1, mode1);
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
      if(mMultiThreading)
      {
          mState = Suspended;
          return;
      }
    }
  }

  void setWaitForInput(bool waitForInput)
  {
    mWaitForInput = waitForInput;
  }

  void setWaitAfterOutput(bool waitAfterOutput, int number)
  {
    mWaitAfterOutput = waitAfterOutput;
    mWaitAfterOutputCounter = number;
  }

  void setMultiThreading(bool multiThreading)
  {
    mMultiThreading = multiThreading;
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

  long long& parameter(const long long &paramNumber, const InstructionMode &mode)
  {
    long long pos = paramPos(mPointer+paramNumber, mode);
    if(pos >= mInstructions.size())
    {
        auto it = mExtendedInstructions.find(pos);
        if(it == mExtendedInstructions.end())
        {
            mExtendedInstructions[pos] = 0;
            return mExtendedInstructions[pos];
        }
        else
        {
            return mExtendedInstructions[pos];
        }
    }
    return mInstructions[pos];
  }

  std::vector<long long> mInstructions;
  std::map<long long, long long> mExtendedInstructions;
  long long mPointer = 0;
  int mId;
  State mState;
  long long mRelativeBase = 0;
  bool mWaitForInput = false;
  bool mWaitAfterOutput = false;
  int mWaitAfterOutputCounter = 0;
  bool mMultiThreading = false;
};
#endif // COMPUTER_H
