#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

//Santa needs help figuring out which strings in his text file are naughty or nice.

//A nice string is one with all of the following properties:

//    It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
//    It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
//    It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.

//For example:

//    ugknbfddgicrmopn is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of the disallowed substrings.
//    aaa is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
//    jchzalrnumimnmhp is naughty because it has no double letter.
//    haegwjzuvuyypxyu is naughty because it contains the string xy.
//    dvszwmarrgswjxmb is naughty because it contains only one vowel.


namespace Part1
{
std::vector<std::string> forbiddenList = {"ab", "cd", "pq", "xy"};
std::string vowels = {"aeiou"};
}

bool checkRules1(const std::string& text)
{
  int numberOfVowels = 0;
  bool twiceInRow = false;
  bool inTheForbidenList = false;
  for(int i = 0; i < text.size(); ++i)
  {
    if(i < text.size() - 1)
    {
      std::string twoLetters = std::string{text[i]} + std::string{text[i+1]};
      if(std::find(Part1::forbiddenList.begin(), Part1::forbiddenList.end(), twoLetters) != Part1::forbiddenList.end())
      {
        inTheForbidenList = true;
        break;
      }
      if(text[i] == text[i+1])
      {
        twiceInRow = true;
      }
    }
    if(std::find(Part1::vowels.begin(), Part1::vowels.end(), text[i]) != Part1::vowels.end())
    {
      numberOfVowels++;
    }
  }
  if(inTheForbidenList)
  {
    return false;
  }
  if(!twiceInRow)
  {
    return false;
  }
  if(numberOfVowels < 3)
  {
    return false;
  }
  return true;
}

//Now, a nice string is one with all of the following properties:

//    It contains a pair of any two letters that appears at least twice in the string without overlapping,
//                   like xyxy (xy) or aabcdefgaa (aa), but not like aaa (aa, but it overlaps).
//    It contains at least one letter which repeats with exactly one letter between them,
//                   like xyx, abcdefeghi (efe), or even aaa.

bool checkRules2(const std::string& text)
{
  bool twoPairs = false;
  bool letterRepeatsAfterAnotherLetter = false;
  for(int i = 0; i < text.size(); ++i)
  {
    if(!twoPairs && i < text.size()-3)
    {
      for(int j = i+2; j < text.size()-1; ++j)
      {
        if(text[j] == text[i] && text[j+1] == text[i+1])
        {
          twoPairs = true;
          break;
        }
      }
    }
    if(!letterRepeatsAfterAnotherLetter && i < text.size()-2)
    {
      if(text[i] == text[i+2])
      {
        letterRepeatsAfterAnotherLetter = true;
      }
    }
  }
  return letterRepeatsAfterAnotherLetter && twoPairs;
}


int main()
{
  std::vector<std::string> textList;

  std::string text;
  std::fstream infile("input.txt");
  while(std::getline(infile, text))
  {
    textList.push_back(text);
  }

  int count1 = 0;
  int count2 = 0;
  for(auto& text : textList)
  {
    if(checkRules1(text))
    {
      count1++;
    }
    if(checkRules2(text))
    {
      count2++;
    }
  }

  std::cout<<"[Part 1] Number of nice lines: "<<count1<<std::endl;
  std::cout<<"[Part 2] Number of nice lines: "<<count2<<std::endl;
  return 0;
}
