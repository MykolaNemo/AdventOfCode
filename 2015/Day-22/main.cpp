#include <iostream>
#include <vector>


// Magic Missile costs 53 mana. It instantly does 4 damage.
// Drain costs 73 mana. It instantly does 2 damage and heals you for 2 hit points.
// Shield costs 113 mana. It starts an effect that lasts for 6 turns. While it is active, your armor is increased by 7.
// Poison costs 173 mana. It starts an effect that lasts for 6 turns. At the start of each turn while it is active, it deals the boss 3 damage.
// Recharge costs 229 mana. It starts an effect that lasts for 5 turns. At the start of each turn while it is active, it gives you 101 new mana.


//Boss Hit Points: 51
//Boss Damage: 9
//Boss Armor: 0 (all ignored)

//#define PART1

struct Spell
{
    int cost = 0;
    int damage = 0;
    int heal = 0;
    bool poison = false;
    bool shield = false;
    bool recharge = false;
};

struct State
{
    int shieldDuration = 0;
    int poisonDuration = 0;
    int rechargeDuration = 0;
    int playerHitPoints = 0;
    int playerMana = 0;
    int playerArmor = 0;
    int bossHitPoints = 0;
    int manaSpent = 0;
};

bool operator==(const State& s1, const State& s2)
{
    bool equal = true;
    equal &= (s1.shieldDuration == s2.shieldDuration);
    equal &= (s1.poisonDuration == s2.poisonDuration);
    equal &= (s1.rechargeDuration == s2.rechargeDuration);
    equal &= (s1.playerHitPoints == s2.playerHitPoints);
    equal &= (s1.playerMana == s2.playerMana);
    return equal;
}

bool operator!=(const State& s1, const State& s2)
{
    return !(s1==s2);
}

std::vector<State> allStates;
std::vector<Spell> allSpells;
const int KShieldStrength = 7;
const int KPoisonStrength = 3;
const int KRechargeStrength = 101;
const int KShieldDuration = 6;
const int KPoisonDuration = 6;
const int KRechargeDuration = 5;
const int KPlayerHitPoints = 50;
const int KPlayerMana = 500;
const int KBossHitPoints = 51;
const int KBossDamage = 9;


Spell MagicMissile = []()->Spell{
        Spell spell;
        spell.cost = 53;
        spell.damage = 4;
        return spell;
    }();
Spell Drain = []()->Spell{
        Spell spell;
        spell.cost = 73;
        spell.damage = 2;
        spell.heal = 2;
        return spell;
    }();
Spell Shield = []()->Spell{
        Spell spell;
        spell.cost = 113;
        spell.shield = true;
        return spell;
    }();
Spell Poison = []()->Spell{
        Spell spell;
        spell.cost = 173;
        spell.poison = true;
        return spell;
    }();
Spell Recharge = []()->Spell{
        Spell spell;
        spell.cost = 229;
        spell.recharge = true;
        return spell;
    }();

void applyEffects(State& state, bool hard)
{
    if(hard)
    {
        state.playerHitPoints -= 1;
    }
    if(state.shieldDuration > 0)
    {
        state.playerArmor = KShieldStrength;
        state.shieldDuration -= 1;
    }
    else
    {
        state.playerArmor = 0;
    }

    if(state.poisonDuration > 0)
    {
        state.bossHitPoints -= KPoisonStrength;
        state.poisonDuration -= 1;
    }
    if(state.rechargeDuration > 0)
    {
        state.playerMana += KRechargeStrength;
        state.rechargeDuration -= 1;
    }
}

bool isBossAlive(State& state)
{
    return (state.bossHitPoints > 0);
}

bool isPlayerAlive(State& state)
{
    return (state.playerHitPoints > 0);
}

void battle()
{
    int minManaSpent = 0;
    auto recordManaSpent = [&minManaSpent](int manaSpent){
        if(minManaSpent == 0)
        {
            minManaSpent = manaSpent;
        }
        else if(manaSpent < minManaSpent)
        {
            minManaSpent = manaSpent;
        }
    };

    {
        State state;
        state.playerHitPoints = KPlayerHitPoints;
        state.playerMana = KPlayerMana;
        state.bossHitPoints = KBossHitPoints;

        allStates.insert(allStates.begin(), state);
    }

    int n = 0;
    while(!allStates.empty())
    {
        n++;
        State oldState = allStates.back();
        allStates.pop_back();

        for(int i = 0; i < allSpells.size(); ++i)
        {
            const Spell& spell = allSpells[i];
            State state = oldState;

            //Player turn
#ifdef PART1
            applyEffects(state, false);
#else
            applyEffects(state, true);
            if(!isPlayerAlive(state)) continue;
#endif
            if(!isBossAlive(state))
            {
                recordManaSpent(state.manaSpent);
                continue;
            }
            //Player cast spell
            if(state.playerMana < spell.cost) continue;
            if(state.shieldDuration > 0 && spell.shield)     continue;
            if(state.poisonDuration > 0 && spell.poison)     continue;
            if(state.rechargeDuration > 0 && spell.recharge) continue;
            state.playerMana -= spell.cost;
            state.manaSpent += spell.cost;
            state.bossHitPoints -= spell.damage;
            if(state.shieldDuration == 0)
            {
                state.shieldDuration = KShieldDuration * (spell.shield ? 1 : 0);
            }
            if(state.poisonDuration == 0)
            {
                state.poisonDuration = KPoisonDuration * (spell.poison ? 1 : 0);
            }
            if(state.rechargeDuration == 0)
            {
                state.rechargeDuration = KRechargeDuration * (spell.recharge ? 1 : 0);
            }
            state.playerHitPoints += spell.heal;
            if(!isBossAlive(state))
            {
                recordManaSpent(state.manaSpent);
                continue;
            }

            //Boss turn
            applyEffects(state, false);
#ifndef PART1
            if(!isPlayerAlive(state)) continue;
#endif
            if(!isBossAlive(state))
            {
                recordManaSpent(state.manaSpent);
                continue;
            }
            //Boss attacks
            int bossDamage = KBossDamage - state.playerArmor;
            bossDamage = (bossDamage <=0 ) ? 1 : bossDamage;
            state.playerHitPoints -= bossDamage;
            if(!isPlayerAlive(state)) continue;

            bool stateDuplicates = false;
            for(State& s : allStates)
            {
                if(state == s)
                {
                    stateDuplicates = true;
                    if(state.manaSpent < s.manaSpent)
                    {
                        s.manaSpent = state.manaSpent;
                    }
                    break;
                }
            }
            if(!stateDuplicates)
            {
                allStates.insert(allStates.begin(), state);
            }
            std::cout<<allStates.size()<<std::endl;
        }
    }
    std::cout<<"[Part 1] "<<minManaSpent<<" mana"<<std::endl;
}

int main()
{
    allSpells.push_back(MagicMissile);
    allSpells.push_back(Drain);
    allSpells.push_back(Shield);
    allSpells.push_back(Poison);
    allSpells.push_back(Recharge);

    battle();

    return 0;
}
