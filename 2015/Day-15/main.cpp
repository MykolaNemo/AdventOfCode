#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> list;
    int searchBegin = 0;
    size_t pos = str.find(delim);
    while(true)
    {
        list.push_back(str.substr(searchBegin, pos-searchBegin));

        size_t newPos = str.find(delim, pos+delim.size());
        if(newPos == std::string::npos)
        {
            list.push_back(str.substr(pos+delim.size(), str.size()-(pos+delim.size())));
            break;
        }
        searchBegin = pos+1;
        pos = newPos;
    }
    return list;
}

struct Ingredient
{
  std::string name;
  int capacity = 0;
  int durability = 0;
  int flavor = 0;
  int texture = 0;
  int calories = 0;
};

bool operator<(const Ingredient& i1, const Ingredient& i2)
{
  return i1.name < i2.name;
}

std::vector<std::map<Ingredient, int> > possibleIngridients;

void chooseIngridients(const std::vector<Ingredient>& ingredients,
                       int spoonsLeft,
                       std::map<Ingredient, int> chosenIngridients)
{
  if(ingredients.size() == 1)
  {
    chosenIngridients[ingredients[0]] = spoonsLeft;
    possibleIngridients.push_back(chosenIngridients);
    return;
  }
  for(int spoons = 1; spoons <= spoonsLeft - ingredients.size() + 1; ++spoons)
  {
    chosenIngridients[ingredients[0]] = spoons;
    auto newIngredients = ingredients;
    newIngredients.erase(newIngredients.begin());
    chooseIngridients(newIngredients, spoonsLeft - spoons, chosenIngridients);
  }
}

int main()
{
  std::fstream infile("input.txt");
  std::vector<std::string> lines;
  std::string line;
  while(std::getline(infile, line))
  {
    lines.push_back(line);
  }

  std::vector<Ingredient> ingredients;
  for(auto& line : lines)
  {
    //Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
    std::vector<std::string> words = split(line, " ");
    words[0].erase(words[0].size()-1);
    words[2].erase(words[2].size()-1);
    words[4].erase(words[4].size()-1);
    words[6].erase(words[6].size()-1);
    words[8].erase(words[8].size()-1);

    Ingredient ing;
    ing.name = words[0];
    ing.capacity = std::stoi(words[2]);
    ing.durability = std::stoi(words[4]);
    ing.flavor = std::stoi(words[6]);
    ing.texture = std::stoi(words[8]);
    ing.calories = std::stoi(words[10]);
    ingredients.push_back(ing);
  }

  int spoonsLeft = 100;
  std::map<Ingredient, int> chosenIngridients;
  chooseIngridients(ingredients, spoonsLeft, chosenIngridients);

//  std::cout<<possibleIngridients.size()<<std::endl;

  int32_t maxScore = INT32_MIN;
  int32_t maxScore500 = INT32_MIN;
  for(auto ingMap : possibleIngridients)
  {
    int capacity = 0;
    int durability = 0;
    int flavor = 0;
    int texture = 0;
    int calories = 0;
    for(auto& [ing, spoons] : ingMap)
    {
      capacity += ing.capacity*spoons;
      durability += ing.durability*spoons;
      flavor += ing.flavor*spoons;
      texture += ing.texture*spoons;
      calories += ing.calories*spoons;
    }
    capacity = (capacity < 0) ? 0 : capacity;
    durability = (durability < 0) ? 0 : durability;
    flavor = (flavor < 0) ? 0 : flavor;
    texture = (texture < 0) ? 0 : texture;
    calories = (calories < 0) ? 0 : calories;

    int32_t score = capacity*durability*flavor*texture;
    if(score > maxScore)
    {
      maxScore = score;
    }
    if(calories == 500 && score > maxScore500)
    {
      maxScore500 = score;
    }
  }
  std::cout<<"[Part 1] Best cookie: "<<maxScore<<std::endl;
  std::cout<<"[Part 2] Best 500 calorie cookie: "<<maxScore500<<std::endl;

  return 0;
}
