#include <gtest/gtest.h>
#include <iostream>
#include "combat/Character.h"
#include "combat/Monster.h"

class BarbarianRageTest : public ::testing::Test {
protected:
    void SetUp() override {
        rogue = std::make_unique<Character>(CharacterClass::ROGUE, 2, 3, 1);
        warrior = std::make_unique<Character>(CharacterClass::WARRIOR, 3, 1, 2);
        barbarian = std::make_unique<Character>(CharacterClass::BARBARIAN, 1, 3, 3);
        enemy = std::make_unique<Monster>(MonsterType::GOLEM);
    }

    std::unique_ptr<Character> rogue;
    std::unique_ptr<Character> warrior;
    std::unique_ptr<Character> barbarian;
    std::unique_ptr<Monster> enemy;
};

//  меч здесь кастомный - с уроном в 1 единицу, чтобы долго колотить врага
TEST_F(BarbarianRageTest, BarbarianRage) {
    // Первые 3 хода варвара +2 урона, потом -1
    std::cout << "=== START BarbarianRage TEST ===" << std::endl;
    
    barbarian->defaultTurnCount();
    barbarian->equip(Weapon("Меч", 1, DamageType::SLASHING));
    
    std::cout << "=== AFTER EQUIP ===" << std::endl;
    
    for (int i = 0; i <= 5; i++) {
        if (enemy->getCurrentHealth() <= 0) {
            std::cout << "Враг побеждён на ходу " << i << std::endl;
            break;
        }
        int damage = barbarian->calculateDamage(1);
        if (i <= 3) {
            // Должен быть бонус ярости
            EXPECT_EQ(damage, barbarian->getWeapon().getDamage() + barbarian->getStrength() + 2);
        } else {
            // Должен быть штраф
            EXPECT_EQ(damage, barbarian->getWeapon().getDamage() + barbarian->getStrength() - 1);
        }
        barbarian->attack(*enemy);
    }
}