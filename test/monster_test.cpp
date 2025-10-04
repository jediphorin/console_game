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