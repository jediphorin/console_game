#include <gtest/gtest.h>
#include "combat/Character.h"
#include "combat/Monster.h"
#include "utils/RandomUtils.h"

class PoisonTest : public ::testing::Test {
protected:
    void SetUp() override {
        rogue = std::make_unique<Character>(CharacterClass::ROGUE, 1, 100, 3);
        enemy = std::make_unique<Monster>(MonsterType::SLIME);
    }
    std::unique_ptr<Character> rogue;
    std::unique_ptr<Monster> enemy;
};

//  тест падает, если первая атака мажет, поэтому ловкость у Разбойника = 100, чтобы почти гарантировать успех
TEST_F(PoisonTest, PoisonMechanics) {
    rogue->levelUp(CharacterClass::ROGUE);
    rogue->levelUp(CharacterClass::ROGUE); // 3 уровень - яд
    
    rogue->attack(*enemy);
    EXPECT_NE(enemy->getPoison(), nullptr);
    
    // Вторая атака - яд активируется
    rogue->attack(*enemy);
    EXPECT_TRUE(enemy->getPoison()->getPoisonStatus());
    
    // Проверяем прогрессию
    int initialDamage = enemy->getPoison()->getDamage();
    enemy->getPoison()->damageProgression();
    EXPECT_EQ(enemy->getPoison()->getDamage(), initialDamage + 1);
}