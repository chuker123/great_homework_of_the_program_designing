#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QList>

enum class CardType { Attack, Defense };

struct Card {
    QString name;
    CardType type;
    int costPhy;            // 消耗体力
    double multiplier;      // 伤害倍率 或 弹反倍率
    bool isSpecial;         // 是否为特殊防御卡
    int unlockWeaponLevel;  // 解锁所需武器等级
    QString description;

    Card() = default;
    Card(QString n, CardType t, int cost, double mult, bool special, int unlock, QString desc)
        : name(n), type(t), costPhy(cost), multiplier(mult), isSpecial(special), 
          unlockWeaponLevel(unlock), description(desc) {}
};

class CardDeck {
public:
    static QList<Card> getAllAttackCards();
    static QList<Card> getAllDefenseCards();
    static QList<Card> getAvailableAttackCards(int weaponLevel);
    static QList<Card> getAvailableDefenseCards(int weaponLevel);
    static QList<Card> drawCards(bool enemyAttacking, int weaponLevel);
    static int getPhyCostWithWeaponLevel(int baseCost, int weaponLevel);
};

#endif // CARD_H
