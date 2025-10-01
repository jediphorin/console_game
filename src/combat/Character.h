#ifndef CHARACTER_H
#define CHARACTER_H

#include "Weapon.h"
#include <string>
#include <map>

class Monster;  //  forward declarations
enum class CharacterClass { ROGUE, WARRIOR, BARBARIAN };

class Character {
private:
    CharacterClass characterClass;
    int strength;
    int agility;
    int endurance;
    int fullHealth;
    int currentHealth;
    int level;
    int turnCount;
    Weapon weapon;
    std::map<CharacterClass, int> classLevels;

    void calculateStats();
    int calculateDamage(int targetAgility = 0) const;

public:
    Character(CharacterClass charClass, int str, int agi, int end);

    bool attack(Monster& target);
    void takeDamage(int damage, const Monster& attacker);

    void levelUp(CharacterClass newClass);
    void equip(const Weapon& newWeapon);
    void healFull();

    // bool isAlive() const;
    std::string getClassName() const;
    void displayStats();
    void defaultTurnCount();

    int getStrength() const;
    int getAgility() const;
    int getEndurance() const;
    int getFullHealth() const;
    int getCurrentHealth() const;
    int getLevel() const;
    int getTurnCount() const;
    const Weapon& getWeapon() const;    
};

#endif
