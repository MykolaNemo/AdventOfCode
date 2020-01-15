#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

std::string stripEscapeCharacters(std::string str)
{
  str.erase(str.begin());
  str.erase(str.size()-1);
  size_t escapePos = str.find('\\', 0);
  while(escapePos != std::string::npos)
  {
    switch(str[escapePos+1])
    {
    case '\\':
    {
      str.erase(escapePos, 1);
      escapePos = str.find('\\', escapePos+1);
      break;
    }
    case '\"':
    {
      str.erase(escapePos, 1);
      escapePos = str.find('\\', escapePos);
      break;
    }
    case 'x':
    {
      std::string hexString;
      hexString += str[escapePos+2];
      hexString += str[escapePos+3];

      char newChar = 0;
      std::stringstream stream;
      stream << hexString;
      stream >> std::hex >> newChar;

      str[escapePos] = newChar;
      str.erase(escapePos+1, 1);
      str.erase(escapePos+1, 1);
      str.erase(escapePos+1, 1);

      escapePos = str.find('\\', escapePos);
      break;
    }
    default:
      escapePos = str.find('\\', escapePos+1);
      break;
    }
  }
  return str;
}

std::string addEscapeCharacters(std::string str)
{
    size_t pos = str.find('\\', 0);
    while(pos != std::string::npos)
    {
        str.insert(pos, "\\");
        pos = str.find('\\', pos+2);
    }
    pos = str.find('"', 0);
    while(pos != std::string::npos)
    {
        str.insert(pos, "\\");
        pos = str.find('"', pos+2);
    }
    str.push_back('"');
    str.insert(0, "\"");
    return str;
}

int main()
{
  std::vector<std::string> stringList;

  std::string str;
  std::fstream infile("input.txt");
  while(std::getline(infile, str))
  {
    stringList.push_back(str);
  }

  size_t codeCount = 0;
  size_t stringCount = 0;
  size_t extendedCodeCount = 0;
  for(auto& line : stringList)
  {
    codeCount += line.size();
    stringCount += stripEscapeCharacters(line).size();
    extendedCodeCount += addEscapeCharacters(line).size();
  }
  std::cout<<"[Part 1] "<<codeCount - stringCount<<std::endl;
  std::cout<<"[Part 2] "<<extendedCodeCount - codeCount<<std::endl;
  return 0;
}
