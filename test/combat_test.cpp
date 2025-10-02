#include <gtest/gtest.h>
#include "combat/Character.h"
#include "combat/Monster.h"
#include "combat/Weapon.h"

class RogueCombatTest : public ::testing::Test {
protected:
    void SetUp() override {
        rogue = std::make_unique<Character>(CharacterClass::ROGUE, 1, 2, 3);
        // Прокачиваем до 3 уровня разбойника
        rogue->levelUp(CharacterClass::ROGUE);
        rogue->levelUp(CharacterClass::ROGUE);
        
        // Экипируем меч
        sword = std::make_unique<Weapon>("Меч", 3, DamageType::SLASHING);
        rogue->equip(*sword);
        
        slime = std::make_unique<Monster>(MonsterType::SLIME);
    }

    void TearDown() override {
        rogue.reset();
        slime.reset();
        sword.reset();
    }

    std::unique_ptr<Character> rogue;
    std::unique_ptr<Monster> slime;
    std::unique_ptr<Weapon> sword;
};

TEST_F(RogueCombatTest, InitialStats) {
    EXPECT_EQ(rogue->getLevel(), 3);
    EXPECT_EQ(rogue->getStrength(), 1);
    EXPECT_EQ(rogue->getAgility(), 3); // +1 на 2 уровне разбойника
    EXPECT_EQ(rogue->getEndurance(), 3);
    EXPECT_EQ(rogue->getWeapon().getName(), "Меч");
    EXPECT_EQ(rogue->getWeapon().getDamage(), 3);
    EXPECT_EQ(rogue->getWeapon().getType(), DamageType::SLASHING);
}

TEST_F(RogueCombatTest, SlimeInitialStats) {
    EXPECT_EQ(slime->getName(), "Слайм");
    EXPECT_EQ(slime->getFullHealth(), 8);
    EXPECT_EQ(slime->getBaseDamage(), 1);
    EXPECT_EQ(slime->getStrength(), 3);
    EXPECT_EQ(slime->getAgility(), 1);
    EXPECT_EQ(slime->getEndurance(), 2);
}

TEST_F(RogueCombatTest, PoisonCreation) {
    // Первая атака должна создать яд
    rogue->attack(*slime);
    
    Poison* poison = slime->getPoison();
    ASSERT_NE(poison, nullptr);
    EXPECT_FALSE(poison->getPoisonStatus()); // Яд активируется со след хода
    EXPECT_EQ(poison->getDamage(), 0); // Начальный урон яда
}

TEST_F(RogueCombatTest, PoisonActivationAndProgression) {
    // Первая атака - создаем яд
    rogue->attack(*slime);
    
    Poison* poison = slime->getPoison();
    ASSERT_NE(poison, nullptr);
    
    // Вторая атака - яд должен активироваться
    rogue->attack(*slime);
    EXPECT_TRUE(poison->getPoisonStatus());
    
    // Проверяем прогрессию урона
    int initialDamage = poison->getDamage();
    poison->damageProgression();
    EXPECT_EQ(poison->getDamage(), initialDamage + 1);
}

TEST_F(RogueCombatTest, SlimeSlashingResistance) {
    // Урон от рубящего оружия должен быть уменьшен для слайма
    int initialHealth = slime->getCurrentHealth();
    rogue->attack(*slime);
    
    // Проверяем, что урон меньше ожидаемого из-за сопротивления рубящему
    int damageDealt = initialHealth - slime->getCurrentHealth();
    // Базовый урон: 3(меч) + 1(сила) = 4, но слайм игнорирует урон от рубящего оружия
    // Должен остаться только урон от силы и возможных бонусов
    EXPECT_LT(damageDealt, 4);
}

TEST_F(RogueCombatTest, RogueSneakAttack) {
    // У разбойника ловкость 3, у слайма 1 - должна сработать скрытая атака
    Monster highAgilityMonster(MonsterType::GHOST); // Ловкость 3
    
    // Атакуем слайма (ловкость 1 < 3) - должна быть скрытая атака
    rogue->attack(*slime);
    
    // Атакуем призрака (ловкость 3 = 3) - не должно быть скрытой атаки
    // Для этого нужно модифицировать метод attack чтобы он возвращал информацию о бонусах
}

TEST_F(RogueCombatTest, CombatSimulation) {
    int initialRogueHealth = rogue->getCurrentHealth();
    int initialSlimeHealth = slime->getCurrentHealth();
    
    // Симулируем несколько раундов боя
    for (int i = 0; i < 3; i++) {
        rogue->attack(*slime);
        if (slime->getCurrentHealth() <= 0) break;
        
        slime->attack(*rogue);
        if (rogue->getCurrentHealth() <= 0) break;
    }
    
    // Проверяем, что здоровье изменилось
    EXPECT_TRUE(rogue->getCurrentHealth() < initialRogueHealth || 
                slime->getCurrentHealth() < initialSlimeHealth);
    
    // Проверяем, что яд создан и прогрессирует
    if (slime->getPoison()) {
        EXPECT_GT(slime->getPoison()->getDamage(), 0);
    }
}