#include <iostream>
#include <vector>

struct Item
{
    int cost = 0;
    int damage = 0;
    int armor = 0;
};

const std::vector<Item> weapons = {{8,4,0},{10,5,0},{25,6,0},{40,7,0},{74,8,0}};
const std::vector<Item> armors = {{0,0,0}, {13,0,1},{31,0,2},{53,0,3},{75,0,4},{102,0,5}};
const std::vector<Item> rings = {{0,0,0},{0,0,0},{25,1,0},{50,2,0},{100,3,0},{20,0,1},{40,0,2},{80,0,3}};

//Weapons:    Cost  Damage  Armor
//Dagger        8     4       0
//Shortsword   10     5       0
//Warhammer    25     6       0
//Longsword    40     7       0
//Greataxe     74     8       0

//Armor:      Cost  Damage  Armor
//Leather      13     0       1
//Chainmail    31     0       2
//Splintmail   53     0       3
//Bandedmail   75     0       4
//Platemail   102     0       5

//Rings:      Cost  Damage  Armor
//Damage +1    25     1       0
//Damage +2    50     2       0
//Damage +3   100     3       0
//Defense +1   20     0       1
//Defense +2   40     0       2
//Defense +3   80     0       3

//Boss Hit Points: 104
//Boss Damage: 8
//Boss Armor: 1

bool battle(Item character)
{
    int hitPoints = 100;
    int bossHitPoints = 104;
    int bossDamage = 8;
    int bossArmor = 1;
    while(true)
    {
        int charHit = character.damage - bossArmor;
        bossHitPoints -= (charHit >= 1) ? charHit : 1;
        if(bossHitPoints <= 0)
        {
            return true;
        }

        int bossHit = bossDamage - character.armor;
        hitPoints -= (bossHit >= 1) ? bossHit : 1;
        if(hitPoints <= 0)
        {
            return false;
        }
    }
    return false;
}

int main()
{
    int minCost = 400;
    int maxCost = 0;
    int minW = 0;
    int minA = 0;
    int minR1 = 0;
    int minR2 = 0;
    Item allItems;
    for(int w = 0; w < weapons.size(); ++w)
    {
        for(int a = 0; a < armors.size(); ++a)
        {
            for(int r1 = 0; r1 < rings.size(); ++r1)
            {
                for(int r2 = 0; r2 < rings.size()-1; ++r2)
                {
                    if(r2 == r1) continue;
                    allItems.armor = weapons[w].armor + armors[a].armor + rings[r1].armor + rings[r2].armor;
                    allItems.cost = weapons[w].cost + armors[a].cost + rings[r1].cost + rings[r2].cost;
                    allItems.damage = weapons[w].damage + armors[a].damage + rings[r1].damage + rings[r2].damage;
                    if (battle(allItems))
                    {
                        minCost = (allItems.cost < minCost) ? allItems.cost : minCost;
                        minW = w;
                        minA = a;
                        minR1 = r1;
                        minR2 = r2;
                    }
                    else
                    {
                        maxCost = (allItems.cost > maxCost) ? allItems.cost : maxCost;
                    }
                }
            }
        }
    }

    std::cout<<"[Part 1] "<<minCost<<" coins"<<std::endl;
    std::cout<<minW<<" "<<minA<<" "<<minR1<<" "<<minR2<<std::endl;
    std::cout<<"[Part 2] "<<maxCost<<" coins"<<std::endl;

    return 0;
}
