#include <gtest/gtest.h>
#include "combat/Character.h"
#include "combat/Monster.h"
#include "utils/RandomUtils.h"

class CombatSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        rogue = std::make_unique<Character>(CharacterClass::ROGUE, 2, 3, 1);
        warrior = std::make_unique<Character>(CharacterClass::WARRIOR, 3, 2, 2);
        skeleton = std::make_unique<Monster>(MonsterType::SKELETON);
        slime = std::make_unique<Monster>(MonsterType::SLIME);
    }

    std::unique_ptr<Character> rogue, warrior;
    std::unique_ptr<Monster> skeleton, slime;
};

TEST_F(CombatSystemTest, HitCalculation) {
    // Высокая ловкость = больше шансов попасть
    int hits = 0;
    int attempts = 1000;
    
    for (int i = 0; i < attempts; i++) {
        if (RandomUtils::checkHit(rogue->getAgility(), skeleton->getAgility())) {
            hits++;
        }
    }
    
    // Статистически должен попадать чаще чем 50%
    EXPECT_GT(hits, attempts * 0.4);
    EXPECT_LT(hits, attempts * 0.9);
}

TEST_F(CombatSystemTest, DamageTypes) {
    Weapon crushing("Дубина", 3, DamageType::CRUSHING);
    
    warrior->equip(crushing);
    int vsSkeleton = warrior->calculateDamage(skeleton->getAgility());
    EXPECT_EQ(vsSkeleton, 6);
}