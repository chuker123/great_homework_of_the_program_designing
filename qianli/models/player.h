#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDataStream>

struct PlayerStats {
    int hp = 100;           // 血量 [0,100]
    int maxHp = 100;
    int phy = 100;          // 体力 [0,100]
    int maxPhy = 100;
    int dmg = 40;           // 攻击力 [WTS,100]
    int weaponLevel = 1;    // 武器等级 {1,2,3,4,5}
    int money = 0;
    int tempDmgBoost = 0;   // 酒提供的临时攻击力加成（下关生效）

    bool isBloodyBattle() const { return hp > 0 && hp < 50; }
    int getEffectiveDmg() const;
    int getEffectiveMaxHp() const { return maxHp; }
    int getEffectiveMaxPhy() const { return maxPhy; }

    friend QDataStream &operator<<(QDataStream &out, const PlayerStats &stats);
    friend QDataStream &operator>>(QDataStream &in, PlayerStats &stats);
};

class Player {
public:
    PlayerStats stats;

    void takeDamage(int damage);
    void healHp(int amount);
    void consumePhy(int amount);
    void restorePhy(int amount);
    void boostDmg(int amount);
    void upgradeWeapon();
    void applyWineBoost(int boost);
    void clearTempBoost();

    bool isDead() const { return stats.hp <= 0; }
    bool hasEnoughPhy(int cost) const { return stats.phy >= cost; }
};

#endif // PLAYER_H
