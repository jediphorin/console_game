#include <gtest/gtest.h>
#include "game/Game.h"
#include "utils/EntityUtils.h"

class GameLogicTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
    }

    std::unique_ptr<Game> game;
};

TEST_F(GameLogicTest, EntityUtils) {
    EXPECT_TRUE(EntityUtils::isAlive(5));
    EXPECT_TRUE(EntityUtils::isAlive(1));
    EXPECT_FALSE(EntityUtils::isAlive(0));
    EXPECT_FALSE(EntityUtils::isAlive(-1));
}

TEST_F(GameLogicTest, CharacterAliveCheck) {
    Character character(CharacterClass::WARRIOR, 2, 2, 2);
    EXPECT_TRUE(EntityUtils::isAlive(character.getCurrentHealth()));
    
    // Симулируем смерть
    // character.takeDamage(character.getCurrentHealth() + 10, ...);
    // EXPECT_FALSE(EntityUtils::isAlive(character.getCurrentHealth()));
}

TEST_F(GameLogicTest, WeaponComparison) {
    Weapon dagger("Кинжал", 2, DamageType::PIERCING);
    Weapon sword("Меч", 3, DamageType::SLASHING);
    
    EXPECT_NE(dagger.getName(), sword.getName());
    EXPECT_NE(dagger.getDamage(), sword.getDamage());
    EXPECT_NE(dagger.getType(), sword.getType());
}