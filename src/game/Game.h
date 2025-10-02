#ifndef GAME_H
#define GAME_H

#include "../combat/Character.h"
#include "../combat/Monster.h"
#include "../combat/Weapon.h"
#include <memory>

class Game {
private:
    std::unique_ptr<Character> player;
    int consecutiveWins;
    bool gameWon;

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
