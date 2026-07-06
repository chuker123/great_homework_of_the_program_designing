#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <QObject>
#include <QTimer>
#include "models/player.h"
#include "models/enemy.h"
#include "models/card.h"

class BattleManager : public QObject 
{
    Q_OBJECT
public:
    explicit BattleManager(Player *player, Enemy enemy, QObject *parent = nullptr);

    void startBattle();
    void playerPlayCard(int cardIndex);
    void skipTurn();

    bool isPlayerTurn() const { return m_playerTurn; }
    int getCurrentTurn() const { return m_turn; }
    int getRemainingTime() const { return m_remainingTime; }
    const QList<Card> &getCurrentCards() const { return m_currentCards; }
    const Enemy &getEnemy() const { return m_enemy; }
    Player *getPlayer() const { return m_player; }

signals:
    void battleStarted();
    void turnChanged(int turn, bool enemyAttacking);
    void cardsDealt(const QList<Card> &cards);
    void playerAttacked(int damage, bool isRebound);
    void enemyAttacked(int damage);
    void playerDamaged(int damage);
    void damageBlocked(int blockRate, bool fullBlock);
    void damageRebounded(int damage);
    void timerUpdated(int remainingMs);
    void timerTimeout();
    void battleWon(int rewardMoney);
    void battleLost();
    void turnEnded();
    void notEnoughPhy();

private slots:
    void onTurnTimerTick();
    void endPlayerTurn();

private:
    Player *m_player;
    Enemy m_enemy;
    int m_turn = 1;
    bool m_playerTurn = false;
    bool m_enemyIsAttacking = true;
    QList<Card> m_currentCards;

    QTimer *m_turnTimer;
    int m_remainingTime = 10000;  
    static constexpr int TURN_TIME = 10000;
    static constexpr int TIMER_INTERVAL = 100;

    bool shouldEnemyAttackThisTurn() const;
    void dealCards();
    int calculatePlayerDamage(const Card &card) const;
    int calculateEnemyDamage() const;
    void processDefense(const Card &card);
    void processAttack(const Card &card);
};

#endif 
