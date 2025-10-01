#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <random>

namespace RandomUtils {
    inline bool checkHit(int attackerAgility, int targetAgility) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, (attackerAgility + targetAgility));
        return dis(gen) > targetAgility;
    }

    inline int randomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }
}

#endif
