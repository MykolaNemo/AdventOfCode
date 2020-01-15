#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>

class Element
{
public:
  virtual void addInput(Element* el)
  {
    inputs.push_back(el);
    dirty = true;
  }
  void addOutput(Element* el)
  {
    outputs.push_back(el);
  }
  virtual int outputSignal() = 0;
  virtual void setName(const std::string& n)
  {
    name = n;
  }
  std::string getName()
  {
    return name;
  }
  bool isDirty()
  {
    return dirty;
  }
  virtual void setDirty(bool d)
  {
    dirty = d;
  }
  void setSignal(int value)
  {
    signal = value;
  }

protected:
  virtual void update() = 0;

  std::string name;
  int signal = 0;
  std::vector<Element*> inputs;
  std::vector<Element*> outputs;
  bool dirty = true;
};

class RawInput: public Element
{
public:
  RawInput(int s)
  {
    signal = s;
    dirty = false;
  }
  int outputSignal()
  {
    if(!dirty)
    {
      return signal;
    }
    else
    {
      std::cout<<"RawInput can not be dirty!"<<std::endl;
      throw std::exception();
      return 0;
    }
  }
  void addInput(Element* el)
  {
    std::cout<<"RawInput can not have inputs!"<<std::endl;
    throw std::exception();
  }
  void setDirty(bool d)
  {
      //empty
  }
protected:
  void update()
  {
    dirty = false;
  }
};

class Gate: public Element
{
public:
  enum Type
  {
    OR = 0,
    AND,
    NOT,
    RSHIFT,
    LSHIFT
  };

  Gate(Type type):mType(std::move(type))
  {
    dirty = false;
  }
  int outputSignal()
  {
    if(dirty)
    {
      update();
    }
    return signal;
  }
  void addInput(Element* el)
  {
    Element::addInput(el);
    if(inputs.size() > 2)
    {
      std::cout<<"Gate can not have more than 2 inputs!"<<std::endl;
      throw std::exception();
    }
  }

protected:
  void update()
  {
    switch(mType)
    {
    case OR:
      if(inputs.size() == 2)
      {
        signal = inputs[0]->outputSignal() | inputs[1]->outputSignal();
      }
      break;
    case AND:
      if(inputs.size() == 2)
      {
        signal = inputs[0]->outputSignal() & inputs[1]->outputSignal();
      }
      break;
    case NOT:
      if(inputs.size() == 1)
      {
        signal = ~inputs[0]->outputSignal();
      }
      break;
    case RSHIFT:
      if(inputs.size() == 2)
      {
        signal = inputs[0]->outputSignal() >> inputs[1]->outputSignal();
      }
      break;
    case LSHIFT:
      if(inputs.size() == 2)
      {
        signal = inputs[0]->outputSignal() << inputs[1]->outputSignal();
      }
      break;
    }
    dirty = false;
  }

private:
  Gate::Type mType;
};

class Wire: public Element
{
public:
  Wire()
  {
    dirty = false;
  }
  Wire(const std::string& n)
  {
    for(char c : n)
    {
      if(c < 'a' || c > 'z')
      {
        std::cout<<"Wire can not have such name: '"<<n<<'\''<<std::endl;
        throw std::exception();
      }
    }
    name = n;
  }

  int outputSignal()
  {
    if(dirty)
    {
      update();
    }
    return signal;
  }

  void addInput(Element* el)
  {
    Element::addInput(el);
    if(inputs.size() > 1)
    {
      std::cout<<"Wire can not have more than 1 input!"<<std::endl;
      throw std::exception();
    }
  }

protected:
  void update()
  {
    if(inputs.size() == 1)
    {
      signal = inputs[0]->outputSignal();
    }
    dirty = false;
  }
};

std::map<std::string, Gate::Type> gateNames = {{"NOT", Gate::NOT},
                                               {"AND", Gate::AND},
                                               {"OR",  Gate::OR},
                                               {"LSHIFT", Gate::LSHIFT},
                                               {"RSHIFT", Gate::RSHIFT}};
std::vector<Element*> allElements;

bool isWireName(const std::string& n)
{
  for(char c : n)
  {
    if(c < 'a' || c > 'z')
    {
      return false;
    }
  }
  return true;
}

Wire* getWireByName(const std::string& name)
{
  auto it = std::find_if(allElements.begin(), allElements.end(), [&name](Element* el)->bool{
    return (name == el->getName());
  });
  if(it != allElements.end())
  {
    return static_cast<Wire*>(*it);
  }
  else
  {
    Wire* wire = new Wire(name);
    allElements.push_back(wire);
    return wire;
  }
}

void parseInput(const std::vector<std::string>& strings)
{
  for(const std::string& wireString : strings)
  {
    size_t arrowPos = wireString.find("->");
    std::string leftPart = wireString.substr(0, arrowPos-1);
    std::string rightPart = wireString.substr(arrowPos+3, wireString.size()-(arrowPos+3));

    bool isGateInput = true;
    bool isWireInput = true;
    bool isRawInput = true;
    Gate::Type gateType;
    std::string gateInput1Name;
    std::string gateInput2Name;
    std::string inputWireName;
    std::string outputWireName = rightPart;
    int rawSignalValue = 0;

    size_t gatePos = std::string::npos;
    for(auto& [name, type] : gateNames)
    {
      gatePos = leftPart.find(name);
      if(gatePos != std::string::npos)
      {
        gateType = type;
        break;
      }
    }
    if(gatePos != std::string::npos)
    {
      isWireInput = false;
      isRawInput = false;
      if(gateType == Gate::NOT)
      {
        size_t spacePos = leftPart.find(' ');
        gateInput1Name = leftPart.substr(spacePos+1, leftPart.size() - spacePos);
      }
      else
      {
        size_t spacePos1 = leftPart.find_first_of(' ');
        gateInput1Name = leftPart.substr(0, spacePos1);
        size_t spacePos2 = leftPart.find_last_of(' ');
        gateInput2Name = leftPart.substr(spacePos2+1, leftPart.size() - spacePos2);
      }
    }
    else
    {
      isGateInput = false;
      if(!isWireName(leftPart))
      {
        isWireInput = false;
        rawSignalValue = std::stoi(leftPart);
      }
      else
      {
        isRawInput = false;
        inputWireName = leftPart;
      }
    }

    Wire* outputWire = getWireByName(outputWireName);

    if(isGateInput)
    {
      Gate* inputGate = new Gate(gateType);
      allElements.push_back(inputGate);
      inputGate->addOutput(outputWire);
      if(!gateInput1Name.empty())
      {
        if(isWireName(gateInput1Name))
        {
          inputGate->addInput(getWireByName(gateInput1Name));
        }
        else
        {
          RawInput* inputRawValue = new RawInput(std::stoi(gateInput1Name));
          inputRawValue->addOutput(inputGate);
          inputGate->addInput(inputRawValue);
        }
      }
      if(!gateInput2Name.empty())
      {
        if(isWireName(gateInput2Name))
        {
          inputGate->addInput(getWireByName(gateInput2Name));
        }
        else
        {
          RawInput* inputRawValue = new RawInput(std::stoi(gateInput2Name));
          inputRawValue->addOutput(inputGate);
          inputGate->addInput(inputRawValue);
        }
      }
      outputWire->addInput(inputGate);
      allElements.push_back(inputGate);
    }

    if(isWireInput)
    {
      Wire* inputWire = getWireByName(inputWireName);
      inputWire->addOutput(outputWire);
      outputWire->addInput(inputWire);
    }

    if(isRawInput)
    {
      RawInput* inputRawValue = new RawInput(rawSignalValue);
      inputRawValue->addOutput(outputWire);
      outputWire->addInput(inputRawValue);
      allElements.push_back(inputRawValue);
    }
  }
}

int main()
{
  std::vector<std::string> wiresStrings;

  std::string wireString;
  std::fstream infile("input.txt");
  while(std::getline(infile, wireString))
  {
    wiresStrings.push_back(wireString);
  }

  parseInput(wiresStrings);

  int part1WireOutput = getWireByName("a")->outputSignal();
  std::cout<<"[Part 1] "<<part1WireOutput<<std::endl;
  for(auto el : allElements)
  {
    if(el->getName() == "b")
    {
      el->setSignal(part1WireOutput);
    }
    else
    {
      el->setDirty(true);
    }
  }
  int part2WireOutput = getWireByName("a")->outputSignal();
  std::cout<<"[Part 2] "<<part2WireOutput<<std::endl;
  return 0;
}
