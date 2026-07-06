#include "card.h"
#include <QRandomGenerator>
#include <algorithm>

QList<Card> CardDeck::getAllAttackCards() {
    return {
        Card("撩云式", CardType::Attack, 15, 1.0, false, 1, "基础攻击招式"),
        Card("转轮式", CardType::Attack, 18, 1.2, false, 1, "旋转攻击，伤害较高"),
        Card("拨风式", CardType::Attack, 12, 0.8, false, 1, "快速出击，消耗较低"),
        Card("翻浪式", CardType::Attack, 20, 1.4, false, 1, "猛烈攻击，消耗较高"),
        Card("力劈华山", CardType::Attack, 25, 1.8, false, 4, "从天而降的强力一击"),
        Card("青龙探爪", CardType::Attack, 22, 1.6, false, 4, "如青龙出海般的迅猛攻击"),
        Card("飞龙在天", CardType::Attack, 30, 2.2, false, 5, "腾龙之势，威力无穷"),
        Card("亢龙有悔", CardType::Attack, 28, 2.0, false, 5, "龙战于野，其血玄黄")
    };
}

QList<Card> CardDeck::getAllDefenseCards() {
    return {
        Card("格挡", CardType::Defense, 10, 0.0, false, 1, "举起大刀格挡，70%完全防御"),
        Card("闪避", CardType::Defense, 8, 0.0, false, 1, "侧身闪避，70%完全防御"),
        Card("拖刀斩", CardType::Defense, 15, 0.5, true, 4, "拖刀反击，完全防御并反弹50%伤害"),
        Card("盘龙吐信", CardType::Defense, 18, 0.8, true, 5, "龙形防守，完全防御并反弹80%伤害")
    };
}

QList<Card> CardDeck::getAvailableAttackCards(int weaponLevel) {
    QList<Card> result;
    for (const auto &card : getAllAttackCards()) {
        if (weaponLevel >= card.unlockWeaponLevel) {
            result.append(card);
        }
    }
    return result;
}

QList<Card> CardDeck::getAvailableDefenseCards(int weaponLevel) {
    QList<Card> result;
    for (const auto &card : getAllDefenseCards()) {
        if (weaponLevel >= card.unlockWeaponLevel) {
            result.append(card);
        }
    }
    return result;
}

QList<Card> CardDeck::drawCards(bool enemyAttacking, int weaponLevel) {
    QList<Card> result;
    if (enemyAttacking) {
        // 敌方进攻：1张攻击 + 2张防御
        auto attacks = getAvailableAttackCards(weaponLevel);
        auto defenses = getAvailableDefenseCards(weaponLevel);
        if (!attacks.isEmpty()) {
            int idx = QRandomGenerator::global()->bounded(attacks.size());
            result.append(attacks[idx]);
        }
        for (int i = 0; i < 2 && !defenses.isEmpty(); ++i) {
            int idx = QRandomGenerator::global()->bounded(defenses.size());
            result.append(defenses[idx]);
        }
    } else {
        // 敌方防御：3张攻击
        auto attacks = getAvailableAttackCards(weaponLevel);
        for (int i = 0; i < 3 && !attacks.isEmpty(); ++i) {
            int idx = QRandomGenerator::global()->bounded(attacks.size());
            result.append(attacks[idx]);
        }
    }
    return result;
}

int CardDeck::getPhyCostWithWeaponLevel(int baseCost, int weaponLevel) {
    int reduction = 0;
    if (weaponLevel >= 3) reduction += 2;
    if (weaponLevel >= 4) reduction += 2;
    if (weaponLevel >= 5) reduction += 2;
    return std::max(1, baseCost - reduction);
}
