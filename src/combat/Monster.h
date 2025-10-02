#ifndef MONSTER_H
#define MONSTER_H

#include "./Weapon.h"
#include "./Poison.h"
#include <string>
#include <memory>

class Character;    //  forward
enum class MonsterType { GOBLIN, SKELETON, SLIME, GHOST, GOLEM, DRAGON };

class Monster {
private:
    std::string name;
    int fullHealth;
    int currentHealth;
    int baseDamage;
    int strength;
    int agility;
    int endurance;
    Weapon rewardWeapon;
    MonsterType monsterType;
    int turnCount;
    std::unique_ptr<Poison> poison;

    void applySpecialEffects(int& damage, DamageType damageType, const Character& attacker);

public:
    Monster(MonsterType type);

    bool attack(Character& target);
    void takeDamage(int damage, DamageType damageType, const Character& attacker);
    void swallowPoison(int damage);
    
    std::string getName() const;
    int getBaseDamage() const;
    int getStrength() const;
    int getAgility() const;
    int getEndurance() const;
    int getFullHealth() const;
    int getCurrentHealth() const;
    Weapon getRewardWeapon() const;
    Poison* getPoison() const;
    // std::unique_ptr<Poison> getPosion() const;    
};

#endif
