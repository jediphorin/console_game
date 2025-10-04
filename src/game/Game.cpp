#include "Game.h"
#include "../utils/EntityUtils.h"
#include "../utils/RandomUtils.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <thread>

Game::Game() : consecutiveWins(0), gameWon(false) {}

void Game::createCharacter() {
  std::cout << "\n=== СОЗДАНИЕ ПЕРСОНАЖА ===" << std::endl;
  int str = RandomUtils::randomInt(1, 3);
  int agi = RandomUtils::randomInt(1, 3);
  int end = RandomUtils::randomInt(1, 3);
  std::cout << "Случайные характеристики: " << std::endl;
  std::cout << " сила = " << str << ";" << std::endl;
  std::cout << " ловкость = " << agi << ";" << std::endl;
  std::cout << " выносливость = " << end << "." << std::endl;
  CharacterClass selectedClass = selectClass();
  player = std::make_unique<Character>(selectedClass, str, agi, end);
  consecutiveWins = 0;
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
  while (EntityUtils::isAlive(player->getCurrentHealth()) &&
         EntityUtils::isAlive(monster.getCurrentHealth())) {
    if (playerFirst) {
      std::cout << "\n--- Ход игрока.---" << std::endl;
      player->attack(monster);
      if (!(EntityUtils::isAlive(monster.getCurrentHealth())))
        break;
      pause(3);
      std::cout << "\n--- Ход монстра.---" << std::endl;
      monster.attack(*player);
      pause(3);
    } else {
      std::cout << "\n--- Ход монстра.---" << std::endl;
      monster.attack(*player);
      if (!(EntityUtils::isAlive(player->getCurrentHealth())))
        break;
      pause(3);
      std::cout << "\n--- Ход игрока.---" << std::endl;
      player->attack(monster);
    }
  }
  if (EntityUtils::isAlive(player->getCurrentHealth())) {
    player->healFull();
    player->defaultTurnCount();
    std::cout << "УРА! Вы победили " << monster.getName() << "!" << std::endl;
    if (monster.getRewardWeapon().getName() != player->getWeapon().getName()) {
      std::cout << "В жопе у монстра ты находишь "
                << monster.getRewardWeapon().weaponInfo() << std::endl;
      std::cout << "Твоё текущее " << player->getWeapon().weaponInfo()
                << std::endl;
      std::cout << "Чтобы заменить оружие, введи 'y' или 'Y'." << std::endl;
      char choice;
      std::cin >> choice;
      if (choice == 'y' || choice == 'Y') {
        player->equip(monster.getRewardWeapon());
        std::cout << "Оружие заменено!" << std::endl;
      } else
        std::cout << "Оставил оружие в жопе у монстра." << std::endl;
    }
    player->displayStats();
  } else
    std::cout << "\n=== ПОРАЖЕННИЕ! Ваш персонаж погиб. ===" << std::endl;
}

void Game::run() {
  std::cout << "=== АВТОБАТТЛЕР ===" << std::endl;
  try {
    while (!gameWon) {
      showClassSelection();
      createCharacter();
      while (EntityUtils::isAlive(player->getCurrentHealth()) && !gameWon) {
        battle();
        if (EntityUtils::isAlive(player->getCurrentHealth())) {
          consecutiveWins++;
          if (consecutiveWins == 3) {
            std::cout << "ПОБЕДА! Победа над 3 монстрами подряд." << std::endl;
            gameWon = true;
            break;
          }
          showLevelUpMenu();
        }
      }
      if (!gameWon) {
        std::cout
            << "\nВаш персонаж убит. Чтобы создать нового, введи 'y' или 'Y': ";
        char choice;
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y')
          break;
      }
    }
  } catch (const std::exception &e) {
    std::cout << "\nИгра завершена: " << e.what() << std::endl;
  }
}

void Game::showClassSelection() {
  std::cout << "Разбойник 1 уровня. +4 здоровья, Скрытая атака: +1 к урону, "
               "если ловкость персонажа выше ловкости цели."
            << std::endl;
  std::cout << "Разбойник 2 уровня. +4 здоровья, Ловкость +1." << std::endl;
  std::cout << "Разбойник 3 уровня. +4 здоровья, Яд: наносит дополнительные +1 "
               "урона на 2 ходу, +2 на 3 и т.д.."
            << std::endl;
  std::cout << "Воин 1 уровня. +5 здоровья, Порыв к действию: в 1 ход наносит "
               "двойной урон оружием."
            << std::endl;
  std::cout << "Воин 2 уровня. +5 здоровья, Щит: -3 к получаемому урону, если "
               "сила персонажа выше силы атакующего."
            << std::endl;
  std::cout << "Воин 3 уровня. +5 здоровья, Сила +1." << std::endl;
  std::cout << "Варвар 1 уровня. +6 здоровья, Ярость: +2 к урону первые 3 "
               "хода, потом -1 к урону."
            << std::endl;
  std::cout << "Варвар 2 уровня. +6 здоровья, Каменная кожа: получаемый урон "
               "снижается на значение выносливости."
            << std::endl;
  std::cout << "Варвар 3 уровня. +6 здоровья, Выносливость +1." << std::endl;
}

CharacterClass Game::selectClass() {
  int choice;
  while (true) {
    std::cout << "Выбирай класс: 1 - разбойник, 2 - воин, 3 - варвар (1-3): ";
    if (std::cin.eof()) {
      std::cout << "\nЗавершение игры." << std::endl;
      throw std::runtime_error("Игрунок ввёл EOF.");
    }
    // Проверяем, является ли ввод числом
    if (!(std::cin >> choice)) {
      // Очищаем флаги ошибок
      std::cin.clear();
      // Очищаем буфер от некорректных данных
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Ошибка: введите целое число!" << std::endl;
      continue;
    }
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
  int monsterType = RandomUtils::randomInt(0, 5);
  return Monster(static_cast<MonsterType>(monsterType));
}