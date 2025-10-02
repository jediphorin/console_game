#include "Character.h"
#include "Monster.h"
#include <iostream>
#include "../utils/RandomUtils.h"

Character::Character(CharacterClass charClass, int str, int agi, int end)
    : characterClass(charClass), strength(str), agility(agi), endurance(end), level(1), turnCount(0), weapon("Кулаки", 1, DamageType::CRUSHING) {
    classLevels[charClass] = 1;
    calculateStats();
    currentHealth = fullHealth;
    // start weapon
    switch(charClass) {
        case CharacterClass::ROGUE:
            weapon = Weapon("Кинжал", 2, DamageType::PIERCING);
            break;
        case CharacterClass::WARRIOR:
            weapon = Weapon("Меч", 3, DamageType::SLASHING);
            break;
        case CharacterClass::BARBARIAN:
            weapon = Weapon("Дубина", 3, DamageType::CRUSHING);
            break;        
    }
}

void Character::calculateStats() {
    fullHealth = 0;
    for(const auto& [cls, lvl] : classLevels) {
        switch(cls) {
            case CharacterClass::ROGUE:
                fullHealth += 4 * lvl;
                break;
            case CharacterClass::WARRIOR:
                fullHealth += 5 * lvl;
                break;
            case CharacterClass::BARBARIAN:
                fullHealth += 6 * lvl;
                break;            
        }
    }
    fullHealth += endurance;
}

int Character::calculateDamage(int targetAgility) const {
    int damageResult = weapon.getDamage() + strength;
    for(const auto& [cls, lvl] : classLevels) {
        switch(cls) {
            case CharacterClass::WARRIOR:                
                // воин 1: двойной урон оружием в первый ход
                if (lvl >= 1 && turnCount == 1) {
                    damageResult += weapon.getDamage();
                    std::cout << "Порыв к действию! +" << weapon.getDamage() << " урона." << std::endl;
                }                
                break;
            case CharacterClass::BARBARIAN:
                // варвар 1: +2 урона первые 3 хода, потом -1
                if (turnCount <= 3) {
                    damageResult += 2;
                    std::cout << "Ярость! +2 урона" << std::endl;
                } else {
                    damageResult--;
                    std::cout << "Тёмная сторона ярости. -1 урона." << std::endl;
                }
                break;
            case CharacterClass::ROGUE:
                //  Разбойник 1: +1 урон, если ловкость выше ловкости цели                
                if(targetAgility < agility) {
                    damageResult++;
                    std::cout << "Скрытая атака! +1 урона." << std::endl;
                }
                break;            
        }
    }
    return damageResult;
}

bool Character::attack(Monster& target) {
    turnCount++;
    bool hitResult = RandomUtils::checkHit(agility, target.getAgility());
    if (!hitResult) {
        std::cout << "Промах!" << std::endl;
        if (target.getPoison()) {
            if (getClassLevels().count(CharacterClass::ROGUE) > 0 && getClassLevels().at(CharacterClass::ROGUE) == 3 && target.getPoison()->getPoisonStatus()) {            
                int damage = target.getPoison()->getDamage();
                std::cout << "Но яд всё равно наносит " << target.getPoison()->getDamage() << " урона!" << std::endl;
                target.swallowPoison(damage);
                target.getPoison()->damageProgression();
            }
        }
    }        
    else {
        int damage = calculateDamage(target.getAgility());
        std::cout << "Попадание! Урон = " << damage << std::endl;
        target.takeDamage(damage, weapon.getType(), *this);
        hitResult = true;
    }
    if (target.getPoison()) {
        std::cout << "текущий урон от яда = " << target.getPoison()->getDamage() << "." << std::endl;
    }
    return hitResult;
}

void Character::takeDamage(int damage, const Monster& attacker) {
    int finalDamage = damage;
    if (classLevels.count(CharacterClass::WARRIOR) && classLevels.at(CharacterClass::WARRIOR) >= 2 && strength > attacker.getStrength()) {
        finalDamage -= 3;
        if (finalDamage < 0)
            finalDamage = 0;
        std::cout << "Щит воина блокирует 3 урона!" << std::endl;
    } else if (classLevels.count(CharacterClass::BARBARIAN) && classLevels.at(CharacterClass::BARBARIAN) >= 2) {
        finalDamage -= endurance;
        if (finalDamage < 0)
            finalDamage = 0;
        std::cout << "Каменная кожа снижает урона на " << endurance << "!" << std::endl;
    }
    currentHealth -= finalDamage;
    std::cout << "Получено урона = " << finalDamage << ". Здоровье " << currentHealth << "/" << fullHealth << "." << std::endl;
}

void Character::healFull() {
    currentHealth = fullHealth;
}

void Character::levelUp(CharacterClass newClass) {
    level++;
    classLevels[newClass]++;

    switch(newClass) {
        case CharacterClass::ROGUE:
            if (classLevels[newClass] == 2) {
                agility++;
                std::cout << "Ловкость +1!" << std::endl;
            }
            break;
        case CharacterClass::WARRIOR:
            if (classLevels[newClass] == 3) {
                strength++;
                std::cout << "Сила +1!" << std::endl;
            }
            break;
        case CharacterClass::BARBARIAN:
            if (classLevels[newClass] == 3) {
                endurance++;
                std::cout << "Выносливость +1!" << std::endl;
            }
            break;
    }
    calculateStats();
    healFull();
    std::cout << "Уровень повышен! Теперь вы " << getClassName() << ", ваш уровень - " << level << std::endl;
}

void Character::equip(const Weapon& dropWeapon) {
    weapon = dropWeapon;
    std::cout << "Экипировано новое оружие: " << weapon.getName() << " (урон = " << weapon.getDamage() << ")" << std::endl;
}

std::string Character::getClassName() const {
    std::string classNameResult;
    for(const auto& [cls, lvl] : classLevels) {
        if (!classNameResult.empty())
            classNameResult += "/";
        switch(cls) {            
            case CharacterClass::ROGUE:
                classNameResult += "Разбойник";
                break;
            case CharacterClass::WARRIOR:
                classNameResult += "Воин";
                break;
            case CharacterClass::BARBARIAN:
                classNameResult += "Варвар";
                break;
        }
        classNameResult += " " + std::to_string(lvl);
    }
    return classNameResult;
}

int Character::getStrength() const {
    return strength;
}
int Character::getAgility() const {
    return agility;
}
int Character::getEndurance() const {
    return endurance;
}
int Character::getFullHealth() const {
    return fullHealth;
}
int Character::getCurrentHealth() const {
    return currentHealth;
}
int Character::getLevel() const {
    return level;
}
int Character::getTurnCount() const {
    return turnCount;
}
const Weapon& Character::getWeapon() const {
    return weapon;
}
std::map<CharacterClass, int> Character::getClassLevels() const
{
    return classLevels;
}

void Character::defaultTurnCount() {
    turnCount = 0;
}

void Character::displayStats() {
    std::cout << "\n=== Статистика персонажа ===" << std::endl;
    std::cout << "Класс: " << getClassName() << std::endl;
    std::cout << "Уровень: " << level << std::endl;
    std::cout << "Сила: " << strength << std::endl;
    std::cout << "Ловкость: " << agility << std::endl;
    std::cout << "Выносливость: " << endurance << std::endl;
    std::cout << "Здоровье: " << currentHealth << "/" << fullHealth << "." << std::endl;
    std::cout << "Оружие: " << weapon.getName() << " (урон = " << weapon.getDamage() << ")." << std::endl;
}
