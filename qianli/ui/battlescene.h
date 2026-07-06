#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include "systems/battlemanager.h"
#include "ui/cardwidget.h"

class BattleScene : public QWidget {
    Q_OBJECT
public:
    explicit BattleScene(QWidget *parent = nullptr);
    void setupBattle(Player *player, const Enemy &enemy);

signals:
    void battleFinished(bool won, int rewardMoney);
    void returnToMap();

private slots:
    void onCardsDealt(const QList<Card> &cards);
    void onPlayerDamaged(int damage);
    void onEnemyAttacked(int damage);
    void onDamageBlocked(int blockRate, bool fullBlock);
    void onDamageRebounded(int damage);
    void onTimerUpdated(int remainingMs);
    void onTimerTimeout();
    void onBattleWon(int rewardMoney);
    void onBattleLost();
    void onTurnChanged(int turn, bool enemyAttacking);
    void onNotEnoughPhy();
    void onCardClicked(int index);
    void onSkipTurn();

private:
    BattleManager *m_battleManager = nullptr;
    QList<CardWidget*> m_cardWidgets;

    // UI元素
    QProgressBar *m_playerHpBar;
    QProgressBar *m_playerPhyBar;
    QLabel *m_playerHpLabel;
    QLabel *m_playerPhyLabel;
    QLabel *m_playerDmgLabel;
    QLabel *m_playerWlLabel;
    QProgressBar *m_enemyHpBar;
    QLabel *m_enemyHpLabel;
    QLabel *m_enemyDmgLabel;
    QLabel *m_enemyDefLabel;
    QProgressBar *m_timerBar;
    QLabel *m_turnLabel;
    QLabel *m_actionLabel;
    QLabel *m_battleLog;
    QWidget *m_cardsContainer;
    QPushButton *m_skipButton;
    // 新增：人物立绘
    QLabel *m_playerSprite;
    QLabel *m_enemySprite;

    // 新增：战斗区域背景
    QWidget *m_battleArea;
    QLabel *m_battleBgLabel;  // ← 新增：背景图标签

    void setupUI();
    void updatePlayerStats();
    void updateEnemyStats();
    void addLog(const QString &text);
    void clearCards();
};

#endif // BATTLESCENE_H
