#ifndef ENEMY_H
#define ENEMY_H

#include <QString>

struct Enemy {
    QString name;
    int hp;
    int maxHp;
    int dmg;            // 敌方攻击力
    double defenseRate; // 敌方防御率 EDR
    int rewardMoney;    // 击败后获得金钱
    QString description;

    Enemy() = default;
    Enemy(QString n, int h, int d, double dr, int money, QString desc)
        : name(n), hp(h), maxHp(h), dmg(d), defenseRate(dr), rewardMoney(money), description(desc) {}

    bool isDead() const { return hp <= 0; }
    void takeDamage(int damage) { hp = (hp > damage) ? hp - damage : 0; }
};

class EnemyFactory {
public:
    static Enemy createEnemy(const QString &levelName);
    static QStringList getLevelNames();
};

#endif // ENEMY_H
