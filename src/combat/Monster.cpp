#include "Monster.h"
#include "Character.h"
#include <iostream>
#include <random>
#include "../utils/RandomUtils.h"

Monster::Monster(MonsterType type)
    : monsterType(type), turnCount(0), rewardWeapon("Нет", 0, DamageType::SLASHING) {
    switch(type) {
        case MonsterType::GOBLIN:
            name = "Гоблин";
            fullHealth = 5;
            baseDamage = 2;
            strength = 1;
            agility = 1;
            endurance = 1;
            rewardWeapon = Weapon("Кинжал", 2, DamageType::PIERCING);
            break;
        case MonsterType::SKELETON:
            name = "Скелет";
            fullHealth = 10;
            baseDamage = 2;
            strength = 2;
            agility = 2;
            endurance = 1;
            rewardWeapon = Weapon("Дубина", 3, DamageType::CRUSHING);
            break;
        case MonsterType::SLIME:
            name = "Слайм";
            fullHealth = 8;
            baseDamage = 1;
            strength = 3;
            agility = 1;
            endurance = 2;
            rewardWeapon = Weapon("Копьё", 3, DamageType::PIERCING);
            break;
        case MonsterType::GHOST:
            name = "Призрак";
            fullHealth = 6;
            baseDamage = 3;
            strength = 1;
            agility = 3;
            endurance = 1;
            rewardWeapon = Weapon("Меч", 3, DamageType::SLASHING);
            break;
        case MonsterType::GOLEM:
            name = "Голем";
            fullHealth = 10;
            baseDamage = 1;
            strength = 3;
            agility = 1;
            endurance = 3;
            rewardWeapon = Weapon("Топор", 4, DamageType::SLASHING);
            break;
        case MonsterType::DRAGON:
            name = "Дракон";
            fullHealth = 20;
            baseDamage = 4;
            strength = 3;
            agility = 3;
            endurance = 3;
            rewardWeapon = Weapon("Легендарный меч", 5, DamageType::SLASHING);
            break;
    }
    currentHealth = fullHealth;
}

bool Monster::attack(Character& target) {
    turnCount++;
    bool hitResult = RandomUtils::checkHit(agility, target.getAgility());
    if (!hitResult)
        std::cout << name << " промахнулся!" << std::endl;
    else {
        // int damage = baseDamage + strength;
        int damage = baseDamage;
        if (monsterType == MonsterType::DRAGON && turnCount % 3 == 0) {
            damage += 3;
            std::cout << "Дракон дышит огнём! +3 урона." << std::endl;
        } else if (monsterType == MonsterType::GHOST && agility > target.getAgility()) {
            damage += 1;
            std::cout << "Скрытая атака призрака! +1 урона." << std::endl;
        }
        std::cout << name << " атакует! Урон = " << damage << std::endl;
        target.takeDamage(damage, *this);
        hitResult = true;
    }             
    return hitResult;
}

void Monster::applySpecialEffects(int& damage, DamageType damageType, const Character& attacker) {
    switch(monsterType) {
        case MonsterType::SKELETON:
            if (damageType == DamageType::CRUSHING) {
                damage += attacker.getWeapon().getDamage();
                std::cout << "Дробящее оружие удваивает урон по скелету!" << std::endl;
            }
            break;
        case MonsterType::GOLEM:
            damage -= endurance;
            if (damage < 0)
                damage = 0;
            std::cout << "Каменная кожа снижает урон на " << endurance << "!" << std::endl;
            break;
        case MonsterType::SLIME:
            if (damageType == DamageType::SLASHING) {                
                damage -= attacker.getWeapon().getDamage();
                std::cout << "Урон рубящего оружия обнуляется." << std::endl;
            }
            break;
        default:
            break;
    }
    if (attacker.getClassLevels().count(CharacterClass::ROGUE) > 0 && attacker.getClassLevels().at(CharacterClass::ROGUE) == 3) {
        if (!poison) {
            poison = std::make_unique<Poison>(0, false);
            std::cout << "ЯД СОЗДАН 115" << std::endl;
        }
    }
}

void Monster::takeDamage(int damage, DamageType damageType, const Character& attacker) {
    int finalDamage = damage;
    applySpecialEffects(finalDamage, damageType, attacker);
    if (poison) {
        std::cout << "ЯД СУЩЕСТВУЕТ 123" << std::endl;
        if (poison->getPoisonStatus()) {
            std::cout << "Яд забрал " << poison->getDamage() << " единиц здоровья." << std::endl;
            finalDamage += poison->getDamage();
            std::cout << "УРОН ОТ ЯДА ДОБАВЛЕН ВО ВЕСЬ УРОН 127" << std::endl;
        } else {
            std::cout << "Яд будет отравлять монстра со следующего хода." << std::endl;
            poison->setPoisonStatus(true);
            std::cout << "ДЕЙСТВИЕ ЯДА ВКЛЮЧЕНО 131" << std::endl;
        }
        poison->damageProgression();
    }
    std::cout << "ЯДОВИТЫЙ БЛОК ЗАКОНЧЕН 135" << std::endl;
    if (finalDamage > 0) {
        currentHealth -= finalDamage;
        std::cout << name << " получает урон = " << finalDamage << ". Здоровье " << currentHealth << "/" << fullHealth << "." << std::endl;
    } else
        std::cout << name << " не получает урона!" << std::endl;    
}

void Monster::swallowPoison(int damage) {
    currentHealth -= damage;
}

std::string Monster::getName() const {
    return name;
}
int Monster::getBaseDamage() const {
    return baseDamage;
}
int Monster::getStrength() const {
    return strength;
}
int Monster::getAgility() const {
    return agility;
}
int Monster::getEndurance() const {
    return endurance;
}
int Monster::getFullHealth() const {
    return fullHealth;
}
int Monster::getCurrentHealth() const {
    return currentHealth;
}
Weapon Monster::getRewardWeapon() const {
    return rewardWeapon;
}

Poison* Monster::getPoison() const { 
    return poison.get(); 
}