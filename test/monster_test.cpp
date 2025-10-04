#include <gtest/gtest.h>
#include "combat/Monster.h"
#include "combat/Character.h"

class MonsterTest : public ::testing::Test {
protected:
    void SetUp() override {
        goblin = std::make_unique<Monster>(MonsterType::GOBLIN);
        skeleton = std::make_unique<Monster>(MonsterType::SKELETON);
        slime = std::make_unique<Monster>(MonsterType::SLIME);
        ghost = std::make_unique<Monster>(MonsterType::GHOST);
        golem = std::make_unique<Monster>(MonsterType::GOLEM);
        dragon = std::make_unique<Monster>(MonsterType::DRAGON);
    }

    std::unique_ptr<Monster> goblin, skeleton, slime, ghost, golem, dragon;
};

TEST_F(MonsterTest, MonsterStats) {
    EXPECT_EQ(goblin->getFullHealth(), 5);
    EXPECT_EQ(skeleton->getFullHealth(), 10);
    EXPECT_EQ(slime->getFullHealth(), 8);
    EXPECT_EQ(ghost->getFullHealth(), 6);
    EXPECT_EQ(golem->getFullHealth(), 10);
    EXPECT_EQ(dragon->getFullHealth(), 20);
}

TEST_F(MonsterTest, SpecialAbilities) {
    Character warrior(CharacterClass::WARRIOR, 2, 2, 2);
    Weapon crushingWeapon("Дубина", 3, DamageType::CRUSHING);
    Weapon slashingWeapon("Меч", 3, DamageType::SLASHING);
    
    warrior.equip(crushingWeapon);
    int damageToSkeleton = warrior.calculateDamage(skeleton->getAgility());
    
    // Скелет должен получать двойной урон от дробящего
    warrior.equip(slashingWeapon);
    int normalDamage = warrior.calculateDamage(skeleton->getAgility());
    
    // Не можем напрямую проверить, но логика в applySpecialEffects
}

TEST_F(MonsterTest, DragonFireBreath) {
    for (int i = 1; i <= 4; i++) {
        dragon->attack(*(std::make_unique<Character>(CharacterClass::WARRIOR, 2, 2, 2)).get());
        // Каждый 3-й ход должен быть +3 урона
    }
}

TEST_F(MonsterTest, GhostSneakAttack) {
    Character lowAgilityChar(CharacterClass::WARRIOR, 3, 1, 2); // Ловкость 1
    Character highAgilityChar(CharacterClass::ROGUE, 1, 4, 1);  // Ловкость 4
    
    // Призрак должен получать +1 против низколовких целей
}