#ifndef GAME_H
#define GAME_H

#include "../combat/Character.h"
#include "../combat/Monster.h"
#include "../combat/Weapon.h"
#include <vector>
#include <memory>

class Game {
private:
    std::unique_ptr<Character> player;
    int consecutiveWins;
    bool gameWon;
    // std::vector<Weapon> availableWeapons;

    void createCharacter();
    void pause(int time);
    void battle();
    void showLevelUpMenu();
    void showClassSelection();
    CharacterClass selectClass();
    Monster generateRandomMonster();

public:
    Game();
    void run();
};

#endif
