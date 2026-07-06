#include "battlemanager.h"
#include <QRandomGenerator>
#include <QtMath>
#include <QTimer>

BattleManager::BattleManager(Player *player, Enemy enemy, QObject *parent)
    : QObject(parent), m_player(player), m_enemy(enemy) {
    m_turnTimer = new QTimer(this);
    m_turnTimer->setInterval(TIMER_INTERVAL);
    connect(m_turnTimer, &QTimer::timeout, this, &BattleManager::onTurnTimerTick);
}

void BattleManager::startBattle() {
    m_turn = 1;
    emit battleStarted();
    dealCards();
}

bool BattleManager::shouldEnemyAttackThisTurn() const {
    return (m_turn % 2 == 1);
}

void BattleManager::dealCards() {
    m_enemyIsAttacking = shouldEnemyAttackThisTurn();
    m_currentCards = CardDeck::drawCards(m_enemyIsAttacking, m_player->stats.weaponLevel);
    m_playerTurn = true;
    m_remainingTime = TURN_TIME;

    emit turnChanged(m_turn, m_enemyIsAttacking);
    emit cardsDealt(m_currentCards);

    m_turnTimer->start();
}

void BattleManager::onTurnTimerTick() {
    m_remainingTime -= TIMER_INTERVAL;
    emit timerUpdated(m_remainingTime);
    if (m_remainingTime <= 0) {
        m_turnTimer->stop();
        emit timerTimeout();
        skipTurn();
    }
}

void BattleManager::playerPlayCard(int cardIndex) {
    if (!m_playerTurn || cardIndex < 0 || cardIndex >= m_currentCards.size()) return;

    const Card &card = m_currentCards[cardIndex];
    int actualCost = CardDeck::getPhyCostWithWeaponLevel(card.costPhy, m_player->stats.weaponLevel);

    if (!m_player->hasEnoughPhy(actualCost)) {
        emit notEnoughPhy();
        return;
    }

    m_turnTimer->stop();
    m_player->consumePhy(actualCost);

    if (m_enemyIsAttacking) {
        // 敌方进攻，玩家后手出牌（防御）
        processDefense(card);
    } else {
        // 敌方防御，玩家先手出牌（攻击）
        processAttack(card);
    }

    endPlayerTurn();
}

void BattleManager::processDefense(const Card &card) {
    int enemyDmg = calculateEnemyDamage();

    if (card.type == CardType::Attack) {
        int dmg = calculatePlayerDamage(card);
        m_enemy.takeDamage(dmg);
        emit enemyAttacked(dmg);
        m_player->takeDamage(enemyDmg);
        emit playerDamaged(enemyDmg);
        return; 
    }

    if (card.isSpecial) {
        // 特殊防御卡：完全防御 + 反弹
        int reboundDmg = static_cast<int>(enemyDmg * card.multiplier * m_enemy.defenseRate * 0.5);
        emit damageBlocked(100, true);
        if (reboundDmg > 0) {
            m_enemy.takeDamage(reboundDmg);
            emit damageRebounded(reboundDmg);
            emit enemyAttacked(reboundDmg);
        }
    } else {
        // 普通防御卡：70%完全防御，30%抵消75%
        int roll = QRandomGenerator::global()->bounded(100);
        if (roll < 70) {
            emit damageBlocked(100, true);
        } else {
            int actualDmg = static_cast<int>(enemyDmg * 0.25);
            m_player->takeDamage(actualDmg);
            emit damageBlocked(75, false);
            emit playerDamaged(actualDmg);
        }
    }
}

void BattleManager::processAttack(const Card &card) {
    int dmg = calculatePlayerDamage(card);
    m_enemy.takeDamage(dmg);
    emit enemyAttacked(dmg);
}

int BattleManager::calculatePlayerDamage(const Card &card) const {
    int dmg = m_player->stats.getEffectiveDmg();
    return static_cast<int>(dmg * card.multiplier * m_enemy.defenseRate);
}

int BattleManager::calculateEnemyDamage() const {
    return m_enemy.dmg;
}

void BattleManager::skipTurn() {
    if (!m_playerTurn) return;
    m_turnTimer->stop();

    if (m_enemyIsAttacking) {
        // 放弃出牌，受到全额伤害
        int dmg = calculateEnemyDamage();
        m_player->takeDamage(dmg);
        emit playerDamaged(dmg);
    }

    endPlayerTurn();
}

void BattleManager::endPlayerTurn() {
    m_playerTurn = false;
    emit turnEnded();

    // 检查战斗结果
    if (m_player->isDead()) {
        emit battleLost();
        return;
    }
    if (m_enemy.isDead()) {
        emit battleWon(m_enemy.rewardMoney);
        return;
    }

    // 下一回合
    m_turn++;
    QTimer::singleShot(800, this, &BattleManager::dealCards);
}
