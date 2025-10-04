#ifndef WEAPON_H
#define WEAPON_H

#include <string>

enum class DamageType { SLASHING, CRUSHING, PIERCING };

class Weapon {
private:
  std::string name;
  int damage;
  DamageType type;

public:
  Weapon(const std::string &nam, int dam, DamageType typ)
      : name(nam), damage(dam), type(typ) {}

  std::string getName() const { return name; }
  int getDamage() const { return damage; }
  DamageType getType() const { return type; }
  
  std::string weaponInfo() const {
    std::string infoResult = "Оружие - ";
    infoResult += name;
    infoResult += ", урон = ";
    infoResult += std::to_string(damage);
    infoResult += ", тип урона - ";
    switch (type) {
    case DamageType::SLASHING:
      infoResult += "рубящий.";
      break;
    case DamageType::CRUSHING:
      infoResult += "дробящий.";
      break;
    case DamageType::PIERCING:
      infoResult += "колющий.";
      break;
    default:
      infoResult += "гладящий.";
    }
    return infoResult;
  }
};
#endif
