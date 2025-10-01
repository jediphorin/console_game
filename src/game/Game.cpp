#include "Game.h"
#include <iostream>
#include <random>
#include <memory>
#include <thread>
#include <chrono>

Game::Game() : consecutiveWins(0), gameWon(false) {}

void Game::createCharacter() {
    std::cout << "\n=== СОЗДАНИЕ ПЕРСОНАЖА ===" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 3);
    int str = dis(gen);
    int agi = dis(gen);
    int end = dis(gen);
    std::cout << "Случайные характеристики: " << std::endl;
    std::cout << " сила = " << str << ";" << std::endl;
    std::cout << " ловкость = " << agi << ";" << std::endl;
    std::cout << " выносливость = " << end << "." << std::endl;
    CharacterClass selectedClass = selectClass();
    player = std::make_unique<Character>(selectedClass, str, agi, end);
    consecutiveWins = 0;    //  дубль?
    player->displayStats();
}

void Game::pause(int time) {
    std::this_thread::sleep_for(std::chrono::seconds(time));
}

void Game::battle() {
    Monster monster = generateRandomMonster();
    std::cout << "\n=== НАЧАЛО БОЯ ===" << std::endl;
    std::cout << "Противник: " << monster.getName() << std::endl;
    std::cout << "Здоровье монстра: " << monster.getCurrentHealth() << std::endl;
    pause(3);
    bool playerFirst = player->getAgility() >= monster.getAgility();
    while(player->isAlive() && monster.isAlive()) {
        if (playerFirst) {
            std::cout << "\n--- Ход игрока.---" << std::endl;
            player->attack(monster);
            if (!monster.isAlive())
                break;
            pause(3);
            std::cout << "\n--- Ход монстра.---" << std::endl;
            monster.attack(*player);
            pause(3);
        } else {            
            std::cout << "\n--- Ход монстра.---" << std::endl;
            monster.attack(*player);
            if(!player->isAlive())
                break;
                pause(3);
            std::cout << "\n--- Ход игрока.---" << std::endl;
            player->attack(monster);
        }
    }
    if (player->isAlive()) {
        player->healFull();
        player->defaultTurnCount();
        std::cout << "УРА! Вы победили " << monster.getName() << "!" << std::endl;
        std::cout << "В жопе у монстра ты находишь " << monster.getRewardWeapon().weaponInfo() << std::endl;
        std::cout << "Твоё текущее " << player->getWeapon().weaponInfo() << std::endl;
        std::cout << "Чтобы заменить оружие, введи 'y' или 'Y'." << std::endl;
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            player->equip(monster.getRewardWeapon());
            std::cout << "Оружие заменено!" << std::endl;
        } else
            std::cout << "Оставил оружие в жопе у монстра." << std::endl;            
    } else {
        std::cout << "\n=== ПОРАЖЕННИЕ! Ваш персонаж погиб. ===" << std::endl;
    }
}

void Game::run() {
    std::cout << "=== АВТОБАТТЛЕР ===" << std::endl;
    while (!gameWon) {
        createCharacter();
        while (player->isAlive() && !gameWon) {
            battle();
            if (player->isAlive()) {
                consecutiveWins++;
                if (consecutiveWins == 3) {
                    std::cout << "ПОБЕДА! Победа над 3 монстрами подряд." << std::endl;
                    gameWon = true;
                    break;
                }
                showLevelUpMenu();
            }
        }
        if(!gameWon) {
            std::cout << "\nВаш персонаж убит. Чтобы создать нового, введи 'y' или 'Y': ";
            char choice;
            std::cin >> choice;
            if (choice != 'y' && choice != 'Y')
                break;
        }
    }
}

void Game::showClassSelection() {    
    std::cout << "Разбойник 1 уровня. +4 здоровья, Скрытая атака: +1 к урону, если ловкость персонажа выше ловкости цели." << std::endl;
    std::cout << "Разбойник 2 уровня. +4 здоровья, Ловкость +1." << std::endl;
    std::cout << "Разбойник 3 уровня. +4 здоровья, Яд: наносит дополнительные +1 урона на 2 ходу, +2 на 3 и т.д.." << std::endl;
    std::cout << "Воин 1 уровня. +5 здоровья, Порыв к действию: в 1 ход наносит двойной урон оружием." << std::endl;
    std::cout << "Воин 2 уровня. +5 здоровья, Щит: -3 к получаемому урону, если сила персонажа выше силы атакующего." << std::endl;
    std::cout << "Воин 3 уровня. +5 здоровья, Сила +1." << std::endl;
    std::cout << "Варвар 1 уровня. +6 здоровья, Ярость: +2 к урону первые 3 хода, потом -1 к урону." << std::endl;
    std::cout << "Варвар 2 уровня. +6 здоровья, Каменная кожа: получаемый урон снижается на значение выносливости." << std::endl;
    std::cout << "Варвар 3 уровня. +6 здоровья, Выносливость +1." << std::endl;
}

CharacterClass Game::selectClass() {
    int choice;
    while (true) {
        std::cout << "Выбирай класс: 1 - разбойник, 2 - воин, 3 - варвар (1-3): ";
        // Проверяем, является ли ввод числом
        if (!(std::cin >> choice)) {
            // Очищаем флаги ошибок
            std::cin.clear();
            // Очищаем буфер от некорректных данных
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите целое число!" << std::endl;
            continue;
        }
        //
        if (choice >= 1 && choice <= 3)
            return static_cast<CharacterClass>(choice - 1);
        std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
    }
}

void Game::showLevelUpMenu() {
    std::cout << "\n=== ПОВЫШЕНИЕ УРОВНЯ ===" << std::endl;
    std::cout << "Выбирете класс для повышения:" << std::endl;
    showClassSelection();
    CharacterClass newClass = selectClass();
    player->levelUp(newClass);
    player->displayStats();
}

Monster Game::generateRandomMonster() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 5);
    int monsterType = dis(gen);
    return Monster(static_cast<MonsterType>(monsterType));
    /*switch(monsterType) {
        case 0:
            return Monster("Гоблин", 5, 2, 1, 1, 1, Weapon("Кинжал", 2, DamageType::PIERCING));
        case 1:
            return Monster("Скелет", 10, 2, 2, 2, Weapon("Дубина", 3, DamageType::CRUSHING));
        case 2:
            return Monster("Слайм", 8, 1, 3, 1, Weapon("Копьё", 3, DamageType::PIERCING));
        case 3:
            return Monster("Призрак", 6, 3, 1, 3, Weapon("Меч", 3, DamageType::SLASHING));
        case 4:
            return Monster("Голем", 10, 1, 3, 1, Weapon("Топор", 4, DamageType::SLASHING));
        case 5:
            return Monster("Дракон", 20, 4, 3, 3, Weapon("Легендарный меч", 5, DamageType::SLASHING));
    }*/
}
