#include "battlescene.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>

BattleScene::BattleScene(QWidget *parent) : QWidget(parent) 
{
    setupUI();
}

void BattleScene::setupUI() 
{
    setStyleSheet("background-color: #2a2a3a; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 10, 20, 10);

    QHBoxLayout *topLayout = new QHBoxLayout();

    QVBoxLayout *playerLayout = new QVBoxLayout();
    QLabel *playerTitle = new QLabel("关羽");
    playerTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #4CAF50;");
    playerLayout->addWidget(playerTitle);

    m_playerHpBar = new QProgressBar();
    m_playerHpBar->setRange(0, 100);
    m_playerHpBar->setValue(100);
    m_playerHpBar->setStyleSheet("QProgressBar { border: 1px solid #555; border-radius: 5px; text-align: center; }"
                                 "QProgressBar::chunk { background-color: #e74c3c; border-radius: 5px; }");
    m_playerHpLabel = new QLabel("HP: 100/100");
    playerLayout->addWidget(m_playerHpLabel);
    playerLayout->addWidget(m_playerHpBar);

    m_playerPhyBar = new QProgressBar();
    m_playerPhyBar->setRange(0, 100);
    m_playerPhyBar->setValue(100);
    m_playerPhyBar->setStyleSheet("QProgressBar { border: 1px solid #555; border-radius: 5px; text-align: center; }"
                                  "QProgressBar::chunk { background-color: #3498db; border-radius: 5px; }");
    m_playerPhyLabel = new QLabel("PHY: 100/100");
    playerLayout->addWidget(m_playerPhyLabel);
    playerLayout->addWidget(m_playerPhyBar);

    m_playerDmgLabel = new QLabel("攻击力: 50");
    m_playerWlLabel = new QLabel("武器等级: 1");
    playerLayout->addWidget(m_playerDmgLabel);
    playerLayout->addWidget(m_playerWlLabel);

    topLayout->addLayout(playerLayout);
    topLayout->addStretch();

    QVBoxLayout *centerLayout = new QVBoxLayout();
    m_turnLabel = new QLabel("第 1 回合");
    m_turnLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #f39c12;");
    m_turnLabel->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(m_turnLabel);

    m_actionLabel = new QLabel("敌方正在进攻！");
    m_actionLabel->setStyleSheet("font-size: 16px; color: #e74c3c;");
    m_actionLabel->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(m_actionLabel);

    topLayout->addLayout(centerLayout);
    topLayout->addStretch();

    QVBoxLayout *enemyLayout = new QVBoxLayout();
    QLabel *enemyTitle = new QLabel("敌方");
    enemyTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #e74c3c;");
    enemyLayout->addWidget(enemyTitle);

    m_enemyHpBar = new QProgressBar();
    m_enemyHpBar->setRange(0, 100);
    m_enemyHpBar->setValue(100);
    m_enemyHpBar->setStyleSheet("QProgressBar { border: 1px solid #555; border-radius: 5px; text-align: center; }"
                                "QProgressBar::chunk { background-color: #c0392b; border-radius: 5px; }");
    m_enemyHpLabel = new QLabel("HP: 100/100");
    enemyLayout->addWidget(m_enemyHpLabel);
    enemyLayout->addWidget(m_enemyHpBar);

    m_enemyDmgLabel = new QLabel("攻击力: 25");
    m_enemyDefLabel = new QLabel("防御率: 80%");
    enemyLayout->addWidget(m_enemyDmgLabel);
    enemyLayout->addWidget(m_enemyDefLabel);

    topLayout->addLayout(enemyLayout);
    mainLayout->addLayout(topLayout);

    m_battleArea = new QWidget();
    m_battleArea->setFixedHeight(250);
    m_battleArea->setMinimumWidth(960);
    m_battleArea->setStyleSheet("background-color: #1a1a2a; border-radius: 10px;");

    m_battleBgLabel = new QLabel(m_battleArea);
    m_battleBgLabel->setGeometry(0, 0, 960, 250);
    m_battleBgLabel->setScaledContents(true);
    m_battleBgLabel->setStyleSheet("background: transparent; border-radius: 10px;");
    m_battleBgLabel->lower();  

    m_playerSprite = new QLabel(m_battleArea);
    m_playerSprite->setAlignment(Qt::AlignCenter);
    m_playerSprite->setStyleSheet("background: transparent;");
    m_playerSprite->setGeometry(20, 10, 180, 230);

    QLabel* vsLabel = new QLabel("⚔️", m_battleArea);
    vsLabel->setStyleSheet("font-size: 36px; color: #f39c12; background: transparent;");
    vsLabel->setAlignment(Qt::AlignCenter);
    vsLabel->setGeometry(420, 90, 60, 60);

    m_enemySprite = new QLabel(m_battleArea);
    m_enemySprite->setAlignment(Qt::AlignCenter);
    m_enemySprite->setStyleSheet("background: transparent;");
    m_enemySprite->setGeometry(760, 10, 180, 230);

    mainLayout->addWidget(m_battleArea);
    m_timerBar = new QProgressBar();
    m_timerBar->setRange(0, 10000);
    m_timerBar->setValue(10000);
    m_timerBar->setStyleSheet("QProgressBar { border: 1px solid #555; border-radius: 5px; height: 20px; }"
                              "QProgressBar::chunk { background-color: #f39c12; border-radius: 5px; }");
    mainLayout->addWidget(m_timerBar);

    m_battleLog = new QLabel("战斗开始！");
    m_battleLog->setStyleSheet("font-size: 14px; color: #ecf0f1; background-color: #1a1a2a; padding: 10px; border-radius: 5px;");
    m_battleLog->setWordWrap(true);
    m_battleLog->setMinimumHeight(60);
    m_battleLog->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_battleLog);

    m_cardsContainer = new QWidget();
    QHBoxLayout *cardsLayout = new QHBoxLayout(m_cardsContainer);
    cardsLayout->setSpacing(20);
    cardsLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_cardsContainer);

    m_skipButton = new QPushButton("放弃出牌");
    m_skipButton->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; padding: 10px 30px; "
                                "font-size: 14px; border-radius: 5px; }"
                                "QPushButton:hover { background-color: #95a5a6; }");
    connect(m_skipButton, &QPushButton::clicked, this, &BattleScene::onSkipTurn);
    mainLayout->addWidget(m_skipButton, 0, Qt::AlignCenter);
}

void BattleScene::setupBattle(Player* player, const Enemy& enemy) 
{

    m_battleLog->setText("战斗开始！");

    if (m_battleManager) 
        delete m_battleManager;
    m_battleManager = new BattleManager(player, enemy, this);

    connect(m_battleManager, &BattleManager::cardsDealt, this, &BattleScene::onCardsDealt);
    connect(m_battleManager, &BattleManager::playerDamaged, this, &BattleScene::onPlayerDamaged);
    connect(m_battleManager, &BattleManager::enemyAttacked, this, &BattleScene::onEnemyAttacked);
    connect(m_battleManager, &BattleManager::damageBlocked, this, &BattleScene::onDamageBlocked);
    connect(m_battleManager, &BattleManager::damageRebounded, this, &BattleScene::onDamageRebounded);
    connect(m_battleManager, &BattleManager::timerUpdated, this, &BattleScene::onTimerUpdated);
    connect(m_battleManager, &BattleManager::timerTimeout, this, &BattleScene::onTimerTimeout);
    connect(m_battleManager, &BattleManager::battleWon, this, &BattleScene::onBattleWon);
    connect(m_battleManager, &BattleManager::battleLost, this, &BattleScene::onBattleLost);
    connect(m_battleManager, &BattleManager::turnChanged, this, &BattleScene::onTurnChanged);
    connect(m_battleManager, &BattleManager::notEnoughPhy, this, &BattleScene::onNotEnoughPhy);

    // 根据关卡设置背景图和敌方立绘
    QString bgPath;
    QString enemyImage;

    if (enemy.name == "孔秀") 
    {
        bgPath = ":/resources/backgrounds/battle_bg_1.png";
        enemyImage = ":/resources/characters/kongxiu.png";
    }
    else if (enemy.name == "韩福") 
    {
        bgPath = ":/resources/backgrounds/battle_bg_2.png";
        enemyImage = ":/resources/characters/hanfu.png";
    }
    else if (enemy.name == "卞喜") 
    {
        bgPath = ":/resources/backgrounds/battle_bg_3.png";
        enemyImage = ":/resources/characters/bianxi.png";
    }
    else if (enemy.name == "王植") 
    {
        bgPath = ":/resources/backgrounds/battle_bg_4.png";
        enemyImage = ":/resources/characters/wangzhi.png";
    }
    else if (enemy.name == "秦琪") 
    {
        bgPath = ":/resources/backgrounds/battle_bg_5.png";
        enemyImage = ":/resources/characters/qinqi.png";
    }

    QPixmap bg(bgPath);
    if (!bg.isNull() && m_battleBgLabel) 
    {
        QPixmap scaled = bg.scaled(960, 250, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        int x = (scaled.width() - 960) / 2;
        QPixmap cropped = scaled.copy(x, 0, 960, 250);
        m_battleBgLabel->setPixmap(cropped);
    }

    QPixmap guanyuPixmap(":/resources/characters/guanyu.png");
    if (!guanyuPixmap.isNull()) 
    {
        guanyuPixmap = guanyuPixmap.scaled(180, 230, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_playerSprite->setPixmap(guanyuPixmap);
    }

    QPixmap enemyPixmap(enemyImage);
    if (!enemyPixmap.isNull()) 
    {
        enemyPixmap = enemyPixmap.scaled(180, 230, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_enemySprite->setPixmap(enemyPixmap);
    }

    updatePlayerStats();
    updateEnemyStats();
    m_battleManager->startBattle();
}

void BattleScene::updatePlayerStats() 
{
    if (!m_battleManager) return;
    Player *p = m_battleManager->getPlayer();
    m_playerHpBar->setRange(0, p->stats.maxHp);
    m_playerHpBar->setValue(p->stats.hp);
    m_playerHpLabel->setText(QString("HP: %1/%2").arg(p->stats.hp).arg(p->stats.maxHp));

    m_playerPhyBar->setRange(0, p->stats.maxPhy);
    m_playerPhyBar->setValue(p->stats.phy);
    m_playerPhyLabel->setText(QString("PHY: %1/%2").arg(p->stats.phy).arg(p->stats.maxPhy));

    m_playerDmgLabel->setText(QString("攻击力: %1").arg(p->stats.getEffectiveDmg()));
    m_playerWlLabel->setText(QString("武器等级: %1").arg(p->stats.weaponLevel));
}

void BattleScene::updateEnemyStats() 
{
    if (!m_battleManager) return;
    const Enemy &e = m_battleManager->getEnemy();
    m_enemyHpBar->setRange(0, e.maxHp);
    m_enemyHpBar->setValue(e.hp);
    m_enemyHpLabel->setText(QString("HP: %1/%2").arg(e.hp).arg(e.maxHp));
    m_enemyDmgLabel->setText(QString("攻击力: %1").arg(e.dmg));
    m_enemyDefLabel->setText(QString("防御率: %1%").arg(int(e.defenseRate * 100)));
}

void BattleScene::onCardsDealt(const QList<Card> &cards) 
{
    clearCards();
    m_skipButton->setEnabled(true);

    QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(m_cardsContainer->layout());
    for (int i = 0; i < cards.size(); ++i) 
    {
        CardWidget *cw = new CardWidget(cards[i], m_battleManager->getPlayer()->stats.weaponLevel, m_cardsContainer);
        connect(cw, &CardWidget::cardClicked, [this, i]() { onCardClicked(i); });
        layout->addWidget(cw);
        m_cardWidgets.append(cw);
    }

    updatePlayerStats();
}

void BattleScene::clearCards() 
{
    for (auto *cw : m_cardWidgets) 
        cw->deleteLater();

    m_cardWidgets.clear();
}

void BattleScene::onCardClicked(int index) 
{
    m_skipButton->setEnabled(false);
    for (auto *cw : m_cardWidgets) 
        cw->setEnabled(false);

    m_battleManager->playerPlayCard(index);
}

void BattleScene::onSkipTurn() 
{
    m_skipButton->setEnabled(false);
    for (auto *cw : m_cardWidgets) 
        cw->setEnabled(false);
    
    addLog("已放弃出牌！");
    m_battleManager->skipTurn();
}

void BattleScene::onTurnChanged(int turn, bool enemyAttacking) 
{
    m_turnLabel->setText(QString("第 %1 回合").arg(turn));
    m_actionLabel->setText(enemyAttacking ? "敌方正在进攻！请防守！" : "敌方正在防御！请进攻！");
    m_actionLabel->setStyleSheet(enemyAttacking
                                     ? "font-size: 16px; color: #e74c3c; font-weight: bold;"
                                     : "font-size: 16px; color: #3498db; font-weight: bold;");
}

void BattleScene::onPlayerDamaged(int damage) 
{
    addLog(QString("受到 %1 点伤害！").arg(damage));
    updatePlayerStats();
}

void BattleScene::onEnemyAttacked(int damage) 
{
    addLog(QString("对敌方造成 %1 点伤害！").arg(damage));
    updateEnemyStats();
}

void BattleScene::onDamageBlocked(int blockRate, bool fullBlock) 
{
    if (fullBlock) 
        addLog("完全防御！未受到伤害！");
    else 
        addLog(QString("防御成功！抵消 %1% 伤害！").arg(blockRate));
}

void BattleScene::onDamageRebounded(int damage) 
{
    addLog(QString("反弹 %1 点伤害！").arg(damage));
}

void BattleScene::onTimerUpdated(int remainingMs) 
{
    m_timerBar->setValue(remainingMs);
}

void BattleScene::onTimerTimeout() 
{
    addLog("时间到！放弃出牌！");
}

void BattleScene::onNotEnoughPhy() 
{
    addLog("体力不足！无法打出该卡牌！");
    m_skipButton->setEnabled(true);
    for (auto *cw : m_cardWidgets) 
        cw->setEnabled(true);
}

void BattleScene::onBattleWon(int rewardMoney) 
{
    clearCards();
    m_skipButton->setEnabled(false);
    m_timerBar->setValue(0);
    addLog(QString("战斗胜利！获得 %1 金钱！").arg(rewardMoney));

    QTimer::singleShot(2000, [this, rewardMoney]() 
    {
        emit battleFinished(true, rewardMoney);
    });
}

void BattleScene::onBattleLost() 
{
    clearCards();
    m_skipButton->setEnabled(false);
    m_timerBar->setValue(0);
    addLog("战斗失败...关羽战死沙场...");

    QTimer::singleShot(2000, [this]() 
    {
        emit battleFinished(false, 0);
    });
}

void BattleScene::addLog(const QString &text) 
{
    m_battleLog->setText(text);
}