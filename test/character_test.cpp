#include <gtest/gtest.h>
#include "combat/Character.h"
#include "combat/Monster.h"

class CharacterTest : public ::testing::Test {
protected:
    void SetUp() override {
        rogue = std::make_unique<Character>(CharacterClass::ROGUE, 2, 3, 1);
        warrior = std::make_unique<Character>(CharacterClass::WARRIOR, 3, 1, 2);
        barbarian = std::make_unique<Character>(CharacterClass::BARBARIAN, 3, 2, 1);
    }

    std::unique_ptr<Character> rogue;
    std::unique_ptr<Character> warrior;
    std::unique_ptr<Character> barbarian;
};

// Тесты создания персонажа
TEST_F(CharacterTest, CharacterCreation) {
    EXPECT_EQ(rogue->getLevel(), 1);
    EXPECT_EQ(rogue->getStrength(), 2);
    EXPECT_EQ(rogue->getAgility(), 3);
    EXPECT_EQ(rogue->getEndurance(), 1);
}

TEST_F(CharacterTest, InitialWeapons) {
    EXPECT_EQ(rogue->getWeapon().getName(), "Кинжал");
    EXPECT_EQ(warrior->getWeapon().getName(), "Меч");
    EXPECT_EQ(barbarian->getWeapon().getName(), "Дубина");
}

TEST_F(CharacterTest, HealthCalculation) {
    // Разбойник: 4 здоровья за уровень + выносливость
    EXPECT_EQ(rogue->getFullHealth(), 5); // 4 + 1
    
    // Воин: 5 здоровья за уровень + выносливость  
    EXPECT_EQ(warrior->getFullHealth(), 7); // 5 + 2
    
    // Варвар: 6 здоровья за уровень + выносливость
    EXPECT_EQ(barbarian->getFullHealth(), 7); // 6 + 1
}

// Тесты способностей 1 уровня
TEST_F(CharacterTest, RogueSneakAttack) {
    Monster lowAgilityMonster(MonsterType::GOLEM); // Ловкость 1
    Monster highAgilityMonster(MonsterType::GHOST); // Ловкость 3
    
    // При атаке низколовкого монстра должен быть бонус
    int damageVsLowAgility = rogue->calculateDamage(lowAgilityMonster.getAgility());
    int damageVsHighAgility = rogue->calculateDamage(highAgilityMonster.getAgility());
    
    EXPECT_GT(damageVsLowAgility, damageVsHighAgility);
}

TEST_F(CharacterTest, WarriorFirstTurnBonus) {
    // Первый ход воина - двойной урон оружием
    warrior->attack(*(std::make_unique<Monster>(MonsterType::GOBLIN)).get());
    // Проверяем через отладочный вывод или модифицируем метод
}

// Тесты прокачки
TEST_F(CharacterTest, LevelUpStats) {
    rogue->levelUp(CharacterClass::ROGUE); // 2 уровень - +1 ловкость
    EXPECT_EQ(rogue->getAgility(), 4);
    
    warrior->levelUp(CharacterClass::WARRIOR);
    warrior->levelUp(CharacterClass::WARRIOR); // 3 уровень - +1 сила
    EXPECT_EQ(warrior->getStrength(), 4);
    
    barbarian->levelUp(CharacterClass::BARBARIAN);
    barbarian->levelUp(CharacterClass::BARBARIAN); // 3 уровень - +1 выносливость
    EXPECT_EQ(barbarian->getEndurance(), 2);
}

TEST_F(CharacterTest, MultiClass) {
    rogue->levelUp(CharacterClass::WARRIOR);
    EXPECT_EQ(rogue->getLevel(), 2);
    EXPECT_TRUE(rogue->getClassName().find("Воин") != std::string::npos);
    
    // Должен получить здоровье от обоих классов
    EXPECT_GE(rogue->getFullHealth(), 9); // 4(разб) + 5(воин) + 1(вынос)
}