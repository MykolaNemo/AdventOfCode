#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

/*
 * The first floor contains
 * a strontium generator,
 * a strontium-compatible microchip,
 * a plutonium generator,
 * and a plutonium-compatible microchip.
*/

/*
 * The second floor contains
 * a thulium generator,
 * a ruthenium generator,
 * a ruthenium-compatible microchip,
 * a curium generator,
 * and a curium-compatible microchip.
*/

/*
 * The third floor contains
 * a thulium-compatible microchip.
*/

/*
 * The fourth floor contains
 *  nothing relevant.
*/

enum class Type
{
    S,
    P,
    R,
    C,
    T
};

struct Item
{
    Item(int obj_, Type type_):obj(obj_),type(type_){}

    int obj = 0; //0 - generator, 1 - module
    Type type = Type::S;
};

/*
 * F3:
 * F2: TM
 * F1: RG RM CG CM TG
 * F0: SG SM PG PM
*/

auto lambda = []()->std::vector<std::vector<Item>>
{
    std::vector<std::vector<Item>> floors;
    std::vector<Item> floor1 = {Item(0,Type::S),
                                Item(1,Type::S),
                                Item(0,Type::P),
                                Item(1,Type::P)};

    std::vector<Item> floor2 = {Item(0,Type::R),
                                Item(1,Type::R),
                                Item(0,Type::C),
                                Item(1,Type::C),
                                Item(0,Type::T)};

    std::vector<Item> floor3 = {Item(1,Type::T)};

    std::vector<Item> floor4;

    floors.push_back(floor1);
    floors.push_back(floor2);
    floors.push_back(floor3);
    floors.push_back(floor4);
    return floors;
};
std::vector<std::vector<Item>> floors = lambda();

int main()
{
    return 0;
}
