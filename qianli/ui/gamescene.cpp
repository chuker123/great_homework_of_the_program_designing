#include "gamescene.h"
#include "systems/savemanager.h"
#include "systems/choicesystem.h"
#include "models/enemy.h"
#include "ui/backpackdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>

GameScene::GameScene(QWidget *parent) : QWidget(parent) {
    m_levelNames = EnemyFactory::getLevelNames();
    setupUI();
}

void GameScene::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_stack = new QStackedWidget(this);
    mainLayout->addWidget(m_stack);

    // 地图界面
    m_mapWidget = new LevelMapWidget();
    connect(m_mapWidget, &LevelMapWidget::levelSelected, this, &GameScene::onLevelSelected);
    m_stack->addWidget(m_mapWidget);

    // 剧情选择界面
    m_choiceWidget = new ChoiceWidget();
    connect(m_choiceWidget, &ChoiceWidget::choiceMade, this, &GameScene::onChoiceMade);
    m_stack->addWidget(m_choiceWidget);

    // 战斗界面
    m_battleScene = new BattleScene();
    connect(m_battleScene, &BattleScene::battleFinished, this, &GameScene::onBattleFinished);
    m_stack->addWidget(m_battleScene);

    // 商店界面
    m_shopWidget = new ShopWidget();
    connect(m_shopWidget, &ShopWidget::shopClosed, this, &GameScene::onShopClosed);
    connect(m_shopWidget, &ShopWidget::openBackpack, this, &GameScene::onOpenBackpack);
    m_stack->addWidget(m_shopWidget);

    // 胜利界面
    QWidget *victoryWidget = new QWidget();
victoryWidget->setStyleSheet("background-color: #1a1a2a;"); // 备用背景色

// 1. 创建背景标签（不参与布局）
QLabel *victoryBgLabel = new QLabel(victoryWidget);
victoryBgLabel->setPixmap(QPixmap("winbackground.png"));
victoryBgLabel->setScaledContents(true);
victoryBgLabel->setGeometry(victoryWidget->rect());
victoryBgLabel->lower();
m_bgLabels.append(victoryBgLabel);  // 加入背景列表，以便resizeEvent调整

// 2. 创建主布局，其他控件正常添加到布局中
QVBoxLayout *victoryLayout = new QVBoxLayout(victoryWidget);
victoryLayout->setSpacing(20);
victoryLayout->setContentsMargins(50, 50, 50, 50);

QLabel *victoryTitle = new QLabel("恭喜通关！");
victoryTitle->setStyleSheet(
    "font-family: 'zilingyufengti_T'; "
    "font-size: 60px; "
    "font-weight: bold; "
    "color: #f1bc0f; "          // 金色（与失败页面的红色不同）
    "background: transparent;"
);
QGraphicsDropShadowEffect *borderShadow = new QGraphicsDropShadowEffect(this);
borderShadow->setColor(Qt::black);    // 描边颜色
borderShadow->setBlurRadius(0);       // 0 表示完全不模糊，就是硬边描边
borderShadow->setOffset(1, 1);        // 向右下偏移1像素

QGraphicsDropShadowEffect *borderShadow2 = new QGraphicsDropShadowEffect(this);
borderShadow2->setColor(Qt::black);
borderShadow2->setBlurRadius(0);
borderShadow2->setOffset(-1, -1);

victoryTitle->setGraphicsEffect(borderShadow);
victoryTitle->setAlignment(Qt::AlignCenter);
victoryLayout->addWidget(victoryTitle);

QLabel *victoryDesc = new QLabel("关羽过五关斩六将，终于与刘备重逢！\n\n千里走单骑，忠义传千古！");
victoryDesc->setStyleSheet(
    "font-family: 'zihunshuyasong_T'; "
    "font-size: 24px; "
    "color: #f1bc0f; "          // 白色（与失败页面的红色不同）
    "background: transparent;"
);
victoryDesc->setAlignment(Qt::AlignCenter);
victoryDesc->setWordWrap(true);
victoryLayout->addWidget(victoryDesc);

victoryLayout->addStretch(1);

// 返回主菜单按钮（仿照失败页面的按钮样式）
QPushButton *victoryReturnBtn = new QPushButton("返回主菜单");
victoryReturnBtn->setStyleSheet(
    "QPushButton { "
    "   background-color: #3498db; "   // 蓝色（可自定义，与失败页面的灰色/红色不同）
    "   color: white; "
    "   padding: 16px 54px; "
    "   font-size: 25px; "
    "   border-radius: 8px; "
    "} "
    "QPushButton:hover { background-color: #2980b9; }"
);
connect(victoryReturnBtn, &QPushButton::clicked, this, &GameScene::returnToMenu);
victoryLayout->addWidget(victoryReturnBtn, 0, Qt::AlignCenter);

victoryLayout->addStretch(1);

// 3. 将背景标签置于底层
victoryBgLabel->lower();

m_stack->addWidget(victoryWidget);

    // 失败界面
    QWidget *gameOverWidget = new QWidget();
    gameOverWidget->setStyleSheet("background-color: #1a1a2a;"); // 备用背景色

    QLabel *bgLabel = new QLabel(gameOverWidget);
    bgLabel->setPixmap(QPixmap("losebackground.png"));
    bgLabel->setScaledContents(true);   // 拉伸填满
    bgLabel->setGeometry(gameOverWidget->rect());
    bgLabel->lower();          
    m_bgLabels.append(bgLabel);      

    QVBoxLayout *goLayout = new QVBoxLayout(gameOverWidget);
    goLayout->setSpacing(20);
    goLayout->setContentsMargins(50, 50, 50, 50);

    QLabel *goTitle = new QLabel("战斗失败");
    goTitle->setStyleSheet(
        "font-family: 'zilingyufengti_T'; "
        "font-size: 60px; "
        "font-weight: bold; "
        "color: #ed0707; "
        "background: transparent;"
    );
    goTitle->setAlignment(Qt::AlignCenter);
    goLayout->addWidget(goTitle);

    QLabel *goDesc = new QLabel("关羽战死沙场...\n\n但忠义精神永存！");
    goDesc->setStyleSheet(
        "font-family: 'zihunshuyasong_T'; "
        "font-size: 24px; "
        "color: #c83e3e; "
        "background: transparent;"
    );
    goDesc->setAlignment(Qt::AlignCenter);
    goDesc->setWordWrap(true);
    goLayout->addWidget(goDesc);

    goLayout->addStretch(1);

    QPushButton *retryBtn = new QPushButton("重新挑战");
    retryBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #e74c3c; "
        "   color: white; "
        "   padding: 16px 54px; "
        "   font-size: 25px; "
        "   border-radius: 8px; "
        "} "
        "QPushButton:hover { background-color: #c0392b; }"
    );
    connect(retryBtn, &QPushButton::clicked, [this]() {
        // 获取所有存档
        auto saves = SaveManager::instance().listSaves();
        if (saves.isEmpty()) {
            QMessageBox::information(this, "读取存档", "没有可用的存档！");
            return;
        }
        QStringList items;
        for (int i = 0; i < saves.size(); ++i) {
            items.append(QString("存档%1: %2 (HP:%3 PHY:%4 金钱:%5)")
                         .arg(i+1).arg(saves[i].levelName)
                         .arg(saves[i].stats.hp).arg(saves[i].stats.phy).arg(saves[i].stats.money));
        }
        bool ok;
        QString item = QInputDialog::getItem(this, "选择重新开始的关卡", "选择存档:", items, 0, false, &ok);
        if (ok && !item.isEmpty()) {
            int slot = items.indexOf(item); 
            auto save = SaveManager::instance().loadSave(slot);
            if (save.isValid) {
                m_player.stats = save.stats;
                m_inventory = save.inventory;
                m_currentLevelIndex = m_levelNames.indexOf(save.levelName);
                if (m_currentLevelIndex < 0) m_currentLevelIndex = 0;
                m_clearedLevels = m_currentLevelIndex;
                SaveManager::instance().clearSavesFrom(slot);
                goToMap();
            }
        }
    });
    goLayout->addWidget(retryBtn, 0, Qt::AlignCenter);

    QPushButton *goReturnBtn = new QPushButton("返回主菜单");
    goReturnBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #7f8c8d; "
        "   color: white; "
        "   padding: 16px 54px; "
        "   font-size: 25px; "
        "   border-radius: 8px; "
        "} "
        "QPushButton:hover { background-color: #95a5a6; }"
    );
    connect(goReturnBtn, &QPushButton::clicked, this, &GameScene::returnToMenu);
    goLayout->addWidget(goReturnBtn, 0, Qt::AlignCenter);

    goLayout->addStretch(1);

    bgLabel->lower();

    m_stack->addWidget(gameOverWidget);
}

void GameScene::startNewGame() {
    // 清除所有存档（从头开始，覆盖所有存档）
    SaveManager::instance().clearAllSaves();
    m_player = Player();
    m_inventory.clear();
    m_currentLevelIndex = 0;
    m_clearedLevels = 0;
    goToMap();
}

void GameScene::loadGame(int saveSlot) {
    auto save = SaveManager::instance().loadSave(saveSlot);
    if (save.isValid) {
        m_player.stats = save.stats;
        m_inventory = save.inventory;
        m_currentLevelIndex = m_levelNames.indexOf(save.levelName);
        if (m_currentLevelIndex < 0) m_currentLevelIndex = 0;
        // 存档的关卡已通关，所以已通关数为当前索引+1
        m_clearedLevels = m_currentLevelIndex + 1;
        goToMap();
    }
}

void GameScene::goToMap() {
    m_mapWidget->setCurrentLevel(m_currentLevelIndex);
    m_mapWidget->setLevelsCleared(m_clearedLevels);
    m_stack->setCurrentIndex(0);
}

void GameScene::onLevelSelected(int levelIndex) {
    m_currentLevelIndex = levelIndex;
    QString levelName = m_levelNames[levelIndex];

    // 清除临时攻击力加成
    m_player.clearTempBoost();

    // 检查是否有剧情选择
    if (ChoiceSystem::hasChoiceEvent(levelName)) {
        m_hasChoiceEvent = true;
        goToChoice(levelName);
    } else {
        m_hasChoiceEvent = false;
        goToBattle(levelName);
    }
}

void GameScene::goToChoice(const QString &levelName) {
    auto event = ChoiceSystem::getEventForLevel(levelName);
    m_choiceWidget->setChoiceEvent(event);
    m_stack->setCurrentIndex(1);
}

void GameScene::onChoiceMade(bool choseA) {
    QString levelName = m_levelNames[m_currentLevelIndex];
    auto event = ChoiceSystem::getEventForLevel(levelName);

    if (choseA) {
        event.effectA(m_player);
    } else {
        event.effectB(m_player);
    }

    QTimer::singleShot(1500, [this, levelName]() {
        goToBattle(levelName);
    });
}

void GameScene::goToBattle(const QString &levelName) {
    Enemy enemy = EnemyFactory::createEnemy(levelName);
    m_battleScene->setupBattle(&m_player, enemy);
    m_stack->setCurrentIndex(2);
}

void GameScene::onBattleFinished(bool won, int rewardMoney) {
    if (won) {
        m_player.stats.money += rewardMoney;
        m_clearedLevels = m_currentLevelIndex + 1;

        if (m_currentLevelIndex >= m_levelNames.size() - 1) {
            // 通关
            showVictory();
        } else {
            // 进入商店
            autoSave();
            goToShop();
        }
    } else {
        showGameOver();
    }
}

void GameScene::goToShop() {
    m_shopWidget->setupShop(&m_player, &m_inventory);
    m_stack->setCurrentIndex(3);
}

void GameScene::onShopClosed() {
    autoSave();
    m_currentLevelIndex++;
    goToMap();
}

void GameScene::onOpenBackpack() {
    BackpackDialog dialog(&m_player, &m_inventory, this);
    connect(&dialog, &BackpackDialog::itemUsed, this, &GameScene::onBackpackItemUsed);
    dialog.exec();
}

void GameScene::onBackpackItemUsed() {
    m_shopWidget->setupShop(&m_player, &m_inventory);
}

void GameScene::showVictory() {
    m_stack->setCurrentIndex(4);
    for (QLabel *label : m_bgLabels) {
        if (label && label->parentWidget()) {
            label->setGeometry(label->parentWidget()->rect());
        }
    }
    emit gameOver(true);
}

void GameScene::showGameOver() {
    m_stack->setCurrentIndex(5);
    for (QLabel *label : m_bgLabels) {
        if (label && label->parentWidget()) {
            label->setGeometry(label->parentWidget()->rect());
        }
    }
    emit gameOver(false);
}

void GameScene::autoSave() {
    SaveManager::LevelSave save;
    save.levelName = m_levelNames[m_currentLevelIndex];
    save.stats = m_player.stats;
    save.inventory = m_inventory;
    save.isValid = true;
    SaveManager::instance().autoSave(save, m_currentLevelIndex);
}

void GameScene::onSaveGame() {
    autoSave();
    QMessageBox::information(this, "存档", "游戏已自动存档！");
}

void GameScene::resizeEvent(QResizeEvent *event) {
    for (QLabel *label : m_bgLabels) {
        if (label && label->parentWidget()) {
            label->setGeometry(label->parentWidget()->rect());
        }
    }
    QWidget::resizeEvent(event);
}

void GameScene::forceVictory() {
    m_player.stats.hp = m_player.stats.maxHp;
    m_player.stats.phy = m_player.stats.maxPhy;
    m_player.stats.money = 9999;  // 可选
    showVictory();
}