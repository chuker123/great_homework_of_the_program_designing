#include "player.h"
#include <algorithm>

int PlayerStats::getEffectiveDmg() const 
{
    int base = dmg + tempDmgBoost;
    if (weaponLevel >= 2) base += 5;
    if (weaponLevel >= 3) base += 5;
    if (weaponLevel >= 4) base += 5;
    if (weaponLevel == 5) base += 5;

    if (hp > 0 && hp < 50) 
        base = static_cast<int>(base * 1.3);
    return std::min(base, 200);
}

QDataStream &operator<<(QDataStream &out, const PlayerStats &stats) 
{
    out << stats.hp << stats.maxHp << stats.phy << stats.maxPhy 
        << stats.dmg << stats.weaponLevel << stats.money << stats.tempDmgBoost;
    return out;
}

QDataStream &operator>>(QDataStream &in, PlayerStats &stats) 
{
    in >> stats.hp >> stats.maxHp >> stats.phy >> stats.maxPhy 
       >> stats.dmg >> stats.weaponLevel >> stats.money >> stats.tempDmgBoost;
    return in;
}

void Player::takeDamage(int damage) 
{
    stats.hp = std::max(0, stats.hp - damage);
}

void Player::healHp(int amount) 
{
    stats.hp = std::min(stats.maxHp, stats.hp + amount);
}

void Player::consumePhy(int amount) 
{
    stats.phy = std::max(0, stats.phy - amount);
}

void Player::restorePhy(int amount) 
{
    stats.phy = std::min(stats.maxPhy, stats.phy + amount);
}

void Player::boostDmg(int amount) 
{
    stats.dmg = std::min(100, stats.dmg + amount);
}

void Player::upgradeWeapon() 
{
    if (stats.weaponLevel < 5) 
        stats.weaponLevel++;
}

void Player::applyWineBoost(int boost) 
{
    stats.tempDmgBoost = boost;
}

void Player::clearTempBoost() 
{
    stats.tempDmgBoost = 0;
}
