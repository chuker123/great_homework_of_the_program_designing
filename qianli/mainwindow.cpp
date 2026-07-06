#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>
#include <QShortcut>
#include "systems/savemanager.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("千里走单骑");
    setMinimumSize(1000, 700);

    QWidget *central = new QWidget();
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QStackedWidget *stack = new QStackedWidget();
    mainLayout->addWidget(stack);

    // 主菜单
    m_menuWidget = new QWidget();
    m_menuWidget->setStyleSheet("background-color: #1a1a2a;");

    // 创建布局前先设置背景
    QVBoxLayout* menuLayout = new QVBoxLayout(m_menuWidget);
    menuLayout->setSpacing(20);
    menuLayout->setContentsMargins(50, 50, 50, 50);

    // 背景图标签（放在最底层）
    m_menuBgLabel = new QLabel(m_menuWidget);
    m_menuBgLabel->setGeometry(0, 0, 1000, 700);  // 和窗口最小尺寸一致
    m_menuBgLabel->setScaledContents(true);
    m_menuBgLabel->setStyleSheet("background: transparent;");
    m_menuBgLabel->lower();  // 放到最底层

    // 加载背景图
    QPixmap menuBg(":/resources/backgrounds/menu_bg.png");
    if (!menuBg.isNull()) {
        m_menuBgLabel->setPixmap(menuBg);
    }

    // 标题
    QLabel *title = new QLabel("千里走单骑");
    QFont titleFont("楷体", 60, QFont::Bold);
    title->setFont(titleFont);
    title->setStyleSheet("color: #f39c12; background: transparent;");
    title->setAlignment(Qt::AlignCenter);

    QGraphicsDropShadowEffect *titleShadow = new QGraphicsDropShadowEffect(this);
    titleShadow->setColor(Qt::black);
    titleShadow->setBlurRadius(4);
    titleShadow->setOffset(2, 2);
    title->setGraphicsEffect(titleShadow);
    menuLayout->addWidget(title);

    QLabel *subtitle = new QLabel("—— 过五关斩六将 ——");
    QFont font("黑体", 20);
    subtitle->setFont(font);
    subtitle->setStyleSheet("color: #ffffff; background: transparent;");
    subtitle->setAlignment(Qt::AlignRight);

    QGraphicsDropShadowEffect *subShadow = new QGraphicsDropShadowEffect(this);
    subShadow->setColor(Qt::black);
    subShadow->setBlurRadius(3);
    subShadow->setOffset(1, 1);
    subtitle->setGraphicsEffect(subShadow);

    menuLayout->addWidget(subtitle);

    menuLayout->addSpacing(50);

    // 按钮
    auto createMenuBtn = [](const QString &text, const QString &color) -> QPushButton* {
        QPushButton *btn = new QPushButton(text);
        btn->setFixedSize(250, 55);
        QFont btnFont("楷体", 24, QFont::Bold);
        btn->setFont(btnFont);
        btn->setStyleSheet(QString("QPushButton { background-color: %1; color: white; font-size: 24px; "
                                   "font-weight: bold; border-radius: 8px; }"
                                   "QPushButton:hover { background-color: %2; }").arg(color).arg(color));
        return btn;
    };

    QPushButton *startBtn = createMenuBtn("开始游戏", "#e74c3c");
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartGame);
    menuLayout->addWidget(startBtn, 0, Qt::AlignRight);

    QPushButton *loadBtn = createMenuBtn("读取存档", "#3498db");
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadGame);
    menuLayout->addWidget(loadBtn, 0, Qt::AlignRight);

    QPushButton *helpBtn = createMenuBtn("游戏说明", "#9b59b6");
    connect(helpBtn, &QPushButton::clicked, this, &MainWindow::onShowHelp);
    menuLayout->addWidget(helpBtn, 0, Qt::AlignRight);

    QPushButton *creditBtn = createMenuBtn("制作人员", "#27ae60");
    connect(creditBtn, &QPushButton::clicked, this, &MainWindow::onShowCredits);
    menuLayout->addWidget(creditBtn, 0, Qt::AlignRight);

    QPushButton *quitBtn = createMenuBtn("退出游戏", "#7f8c8d");
    connect(quitBtn, &QPushButton::clicked, this, &QMainWindow::close);
    menuLayout->addWidget(quitBtn, 0, Qt::AlignRight);

    menuLayout->addStretch();

    stack->addWidget(m_menuWidget);

    // 游戏场景
    m_gameScene = new GameScene();
    connect(m_gameScene, &GameScene::gameOver, this, &MainWindow::onGameOver);
    connect(m_gameScene, &GameScene::returnToMenu, this, &MainWindow::onReturnToMenu);
    stack->addWidget(m_gameScene);

    QShortcut *victoryShortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);
    connect(victoryShortcut, &QShortcut::activated, this, [this]() {
        if (m_gameScene) {
            m_gameScene->forceVictory();
        }
    });

    showMenu();
}

MainWindow::~MainWindow() {}

void MainWindow::showMenu() {
    QStackedWidget *stack = qobject_cast<QStackedWidget*>(centralWidget()->layout()->itemAt(0)->widget());
    stack->setCurrentIndex(0);
}

void MainWindow::onStartGame() {
    QStackedWidget *stack = qobject_cast<QStackedWidget*>(centralWidget()->layout()->itemAt(0)->widget());
    m_gameScene->startNewGame();
    stack->setCurrentIndex(1);
}

void MainWindow::onLoadGame() {
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
    QString item = QInputDialog::getItem(this, "读取存档", "选择存档:", items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        int index = items.indexOf(item);
        QStackedWidget *stack = qobject_cast<QStackedWidget*>(centralWidget()->layout()->itemAt(0)->widget());
        m_gameScene->loadGame(index);
        stack->setCurrentIndex(1);
    }
}

void MainWindow::onShowHelp() {
    QString help = R"(
<b>《千里走单骑》游戏说明</b><br><br>
<b>背景：</b>
汉末烽烟起，桃园义难忘。<br>
困曹营而心向汉，受厚禄而志不移。<br>
一封印信，两乘车驾，青龙偃月伴赤兔，独闯千里赴兄约。<br>
五关险隘，六将横刀，皆挡不住这一腔忠义，一路风尘向古城。<br><br>
<b>核心玩法：</b><br>
1. <b>关卡推进</b>：依次挑战东岭关、洛阳、汜水关、荥阳、滑州五关<br>
2. <b>剧情选择</b>：汜水关和荥阳有关键剧情分支选择<br>
3. <b>卡牌战斗</b>：回合制卡牌战斗，10秒内选择卡牌<br>
4. <b>商店系统</b>：战斗胜利后进入商店购买物品<br>
5. <b>存档机制</b>：离开商店时自动存档<br><br>
<b>角色属性：</b><br>
• 血量(HP)：低于50进入"浴血奋战"状态，攻击力提升<br>
• 体力(PHY)：打出卡牌消耗体力<br>
• 攻击力(DMG)：影响敌方行动频率和伤害<br>
• 武器等级(WL)：解锁更强卡牌<br><br>
<b>战斗规则：</b><br>
• 敌方进攻时：派发1攻+2防卡牌，玩家后手出牌<br>
• 敌方防御时：派发3张攻击卡牌，玩家先手出牌<br>
• 普通防御卡：70%完全防御，30%抵消75%伤害<br>
• 特殊防御卡：完全防御并反弹伤害<br><br>
<b>注意：</b>物品只能在商店中使用，不可带入战斗！
    )";
    QMessageBox::information(this, "游戏说明", help);
}

void MainWindow::onShowCredits() {
    QString credits = R"(
<b>《千里走单骑》</b><br><br>
<b>开发团队：野兽王子队</b><br><br>
蒋亦丰<br>
王天睿<br>
卢宇帆<br><br>

程序设计实习大作业
    )";
    QMessageBox::information(this, "制作人员", credits);
}

void MainWindow::onGameOver(bool victory) {
    // 游戏结束处理
}

void MainWindow::onReturnToMenu() {
    showMenu();
}
