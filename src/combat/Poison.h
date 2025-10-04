#ifndef POISON_H
#define POISON_H

class Poison {
private:
  int damage;
  bool poisonStatus;

public:
  Poison(int dam, bool poiSta) : damage(dam), poisonStatus(poiSta) {}
  void damageProgression() { damage++; }
  void setPoisonStatus(bool status) { poisonStatus = status; }
  int getDamage() const { return damage; }
  bool getPoisonStatus() const { return poisonStatus; }
};

#endif