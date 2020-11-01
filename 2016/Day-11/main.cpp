#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

//#define EXAMPLE
//#define PART1

#ifdef EXAMPLE
enum class Type
{
    H,
    L
};
const std::vector<Type> types = {Type::H, Type::L};
#else
#ifdef PART1
enum class Type
{
    S,
    P,
    R,
    C,
    T
};
const std::vector<Type> types = {Type::S, Type::P, Type::R, Type::C, Type::T};
#else
enum class Type
{
    S,
    P,
    R,
    C,
    T,
    E,
    D
};
const std::vector<Type> types = {Type::S, Type::P, Type::R, Type::C, Type::T, Type::E,  Type::D};
#endif
#endif

enum class Object
{
    Generator,
    Module
};

struct Item
{
    Item(Object obj_, Type type_):obj(obj_),type(type_){}
    bool isGenerator() const
    {
        return obj == Object::Generator;
    }
    bool isModule() const
    {
        return obj == Object::Module;
    }
    friend bool operator==(const Item& rhs, const Item& lhs)
    {
        return (rhs.obj == lhs.obj) && (rhs.type == lhs.type);
    }
    Object obj = Object::Generator;
    Type type = static_cast<Type>(0);
};

using Items = std::vector<Item>;
using Building = std::map<int, Items>;
std::multiset<std::pair<int, int>> buildingImage(const Building& building)
{
    std::multiset<std::pair<int, int>> image;
    for(const Type& type : types)
    {
        std::pair<int, int> pair = {-1, -1};
        for(int i = 0; i < static_cast<int>(building.size()); ++i)
        {
            for(const Item& item : building.at(i))
            {
                if(item.isGenerator() && item.type == type)
                {
                    pair.first = i;
                }
                else if(item.isModule() && item.type == type)
                {
                    pair.second = i;
                }
                if(pair.first != -1 && pair.second != -1)
                {
                    break;
                }
            }
            if(pair.first != -1 && pair.second != -1) break;
        }
        if(!(pair.first == -1 && pair.second == -1))
        {
            image.insert(pair);
        }
    }
    return image;
}

struct State
{
    Building building;
    int currentFloor = 0;

    //for storage
    int steps = 0;
    std::vector<Item> loadedItems;

    bool operator==(const State& other) const
    {
        return ((this->currentFloor == other.currentFloor) &&
                (buildingImage(this->building) == buildingImage(other.building)));
    }
    bool isFried()
    {
        bool hasGenerators = false;
        for(const Item& item1 : building[currentFloor])
        {
            if(item1.isModule())
            {
                bool isShielded = false;
                for(const Item& item2 : building[currentFloor])
                {
                    if(item2 == item1) continue;
                    if(item2.isGenerator())
                    {
                        hasGenerators = true;
                        if(item2.type == item1.type)
                        {
                            isShielded = true;
                            break;
                        }
                    }
                }
                if(!isShielded && hasGenerators) return true;
            }
        }
        return false;
    }
};

#ifdef EXAMPLE
int NumberOfItems = 4;
auto startBuilding = []()->auto
{
    Items floor1 = {Item(Object::Module, Type::H),
                    Item(Object::Module, Type::L)};

    Items floor2 = {Item(Object::Generator, Type::H)};

    Items floor3 = {Item(Object::Generator, Type::L)};

    Items floor4 = {};

    Building building;
    building[0] = floor1;
    building[1] = floor2;
    building[2] = floor3;
    building[3] = floor4;
    return building;
}();

#else
#ifdef PART1
int NumberOfItems = 10;
auto startBuilding = []()->auto
{
    Items floor1 = {Item(Object::Generator, Type::S),
                    Item(Object::Module, Type::S),
                    Item(Object::Generator, Type::P),
                    Item(Object::Module, Type::P)};

    Items floor2 = {Item(Object::Generator, Type::R),
                    Item(Object::Module, Type::R),
                    Item(Object::Generator, Type::C),
                    Item(Object::Module, Type::C),
                    Item(Object::Generator, Type::T)};

    Items floor3 = {Item(Object::Module, Type::T)};

    Items floor4;

    Building building;
    building[0] = floor1;
    building[1] = floor2;
    building[2] = floor3;
    building[3] = floor4;
    return building;
}();
#else
int NumberOfItems = 14;
auto startBuilding = []()->auto
{
    Items floor1 = {Item(Object::Generator, Type::S),
                    Item(Object::Module, Type::S),
                    Item(Object::Generator, Type::P),
                    Item(Object::Module, Type::P),
                    Item(Object::Generator, Type::E),
                    Item(Object::Module, Type::E),
                    Item(Object::Generator, Type::D),
                    Item(Object::Module, Type::D)};

    Items floor2 = {Item(Object::Generator, Type::R),
                    Item(Object::Module, Type::R),
                    Item(Object::Generator, Type::C),
                    Item(Object::Module, Type::C),
                    Item(Object::Generator, Type::T)};

    Items floor3 = {Item(Object::Module, Type::T)};

    Items floor4;

    Building building;
    building[0] = floor1;
    building[1] = floor2;
    building[2] = floor3;
    building[3] = floor4;
    return building;
}();
#endif
#endif

template<class T>
bool contains(const std::vector<T>& v, const T& item)
{
    return std::find(v.begin(), v.end(), item) != v.end();
}

std::vector<State> load(const State& state)
{
    std::vector<State> loadStates;
    for(const Item& item1 : state.building.at(state.currentFloor))
    {
        State s1 = state;
        auto& floor1 = s1.building.at(s1.currentFloor);
        floor1.erase(std::remove(floor1.begin(), floor1.end(), item1), floor1.end());
        s1.loadedItems.push_back(item1);
        if(!s1.isFried() && !contains<State>(loadStates, s1))
        {
            loadStates.push_back(s1);
        }

        for(const Item& item2 : floor1)
        {
            State s2 = s1;
            auto& floor2 = s2.building.at(s2.currentFloor);
            floor2.erase(std::remove(floor2.begin(), floor2.end(), item2), floor2.end());
            s2.loadedItems.push_back(item2);
            if(!s2.isFried() && !contains<State>(loadStates, s2))
            {
                loadStates.push_back(s2);
            }
        }
    }

    {
        std::map<std::pair<int, int>, bool> pairs = {{{1, 0}, false},
                                                     {{0, 1}, false},
                                                     {{1, 1}, false}};
        for(State& loadState : loadStates)
        {
            std::pair<int, int> pair = {0,0};
            for(Item& item : loadState.loadedItems)
            {
                if(item.isGenerator())
                {
                    pair.first = 1;
                }
                else if(item.isModule())
                {
                    pair.second = 1;
                }
            }
            if(!pairs[pair])
            {
                pairs[pair] = true;
            }
            else
            {
                loadStates.erase(std::remove(loadStates.begin(), loadStates.end(), loadState), loadStates.end());
            }
        }
    }
    return loadStates;
}

std::vector<State> goUpOrDown(const State& state)
{
    std::vector<State> states;
    if(state.currentFloor < 3)
    {
        State stateUp = state;
        stateUp.currentFloor++;
        stateUp.steps++;
        states.push_back(stateUp);
    }
    if(state.currentFloor > 0)
    {
        bool dontGoDown = true;
        for(int i = 0; i < state.currentFloor; ++i)
        {
            if(!state.building.at(i).empty())
            {
                dontGoDown = false;
                break;
            }
        }
        if(!dontGoDown)
        {
            State stateDown = state;
            stateDown.currentFloor--;
            stateDown.steps++;
            states.push_back(stateDown);
        }
    }
    return states;
}

bool unload(State& currentState)
{
    Items& floor = currentState.building[currentState.currentFloor];
    for(const Item& item : currentState.loadedItems)
    {
        floor.push_back(item);
    }
    currentState.loadedItems.clear();
    if(!currentState.isFried())
    {
        return true;
    }
    return false;
}

int main()
{
    std::map<int, std::vector<State>> allStates;

    std::vector<State> states;
    {
        State startState;
        startState.building = startBuilding;
        states.push_back(startState);
    }
    while(!states.empty())
    {
        State oldState = states[0];
        states.erase(std::remove(states.begin(), states.end(), oldState), states.end());
        for(const auto& loadState : load(oldState))
        {
            for(auto& state : goUpOrDown(loadState))
            {
                if(unload(state))
                {
                    if(static_cast<int>(state.building.at(3).size()) == NumberOfItems)
                    {
                        std::cout<<"Answer: "<<state.steps<<std::endl;
                        return 0;
                    }
                    int currentFloor = state.currentFloor;
                    auto allIt = std::find(allStates[currentFloor].begin(), allStates[currentFloor].end(), state);
                    if(allIt == allStates[currentFloor].end())
                    {
                        allStates[currentFloor].push_back(state);
                        states.push_back(state);
                    }
                }
            }
        }
    }
    return 0;
}