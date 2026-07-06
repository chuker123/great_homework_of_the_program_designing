#include "shopwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QTimer>

ShopWidget::ShopWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void ShopWidget::setupUI() {
    setStyleSheet("background-color: #2c3e50; color: white;");

    // 背景标签（不参与布局）
    m_bgLabel = new QLabel(this);
    m_bgLabel->setPixmap(QPixmap("shopbackground.png")); 
    m_bgLabel->setScaledContents(true);
    m_bgLabel->setGeometry(rect());
    m_bgLabel->lower();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 20, 30, 20);

    // 标题
    QLabel *title = new QLabel("商店");
    title->setStyleSheet("font-family: 'zihunwulinjianghuti'; "
                         "font-size: 64px; "
                         "font-weight: bold; "
                         "color: #f3e012; "
                         "background: transparent;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(15);
    statsLayout->setAlignment(Qt::AlignCenter);

    m_hpLabel = new QLabel("HP: 0/0");
    m_hpLabel->setStyleSheet("font-family: 'zihunshuyasong_T'; font-size: 28px; color: #e74c3c; background: transparent;");
    statsLayout->addWidget(m_hpLabel);

    m_phyLabel = new QLabel("PHY: 0/0");
    m_phyLabel->setStyleSheet("font-family: 'zihunshuyasong_T'; font-size: 28px; color: #2ecc71; background: transparent;");
    statsLayout->addWidget(m_phyLabel);

    m_dmgLabel = new QLabel("攻击力: 0");
    m_dmgLabel->setStyleSheet("font-family: 'zihunshuyasong_T'; font-size: 28px; color: #3498db; background: transparent;");
    statsLayout->addWidget(m_dmgLabel);

    m_wlLabel = new QLabel("武器等级: 0");
    m_wlLabel->setStyleSheet("font-family: 'zihunshuyasong_T'; font-size: 28px; color: #c413e8; background: transparent;");
    statsLayout->addWidget(m_wlLabel);

    m_moneyLabel = new QLabel("金钱: 0");
    m_moneyLabel->setStyleSheet("font-family: 'zihunshuyasong_T'; font-size: 24px; color: #f1c40f; background: transparent;");
    statsLayout->addWidget(m_moneyLabel);

    mainLayout->addLayout(statsLayout);

    // 消息标签
    m_messageLabel = new QLabel();
    m_messageLabel->setFixedHeight(30);
    m_messageLabel->setStyleSheet(
        "font-size: 14px; "
        "color: #e74c3c; "
        "padding: 5px; "
        "background: transparent; "
        "border: none;"
    );
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setText("");
    mainLayout->addWidget(m_messageLabel);

    // 物品容器
    m_itemsContainer = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(m_itemsContainer);
    gridLayout->setSpacing(15);
    mainLayout->addWidget(m_itemsContainer);

    // 按钮区域
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);

    QPushButton *backpackBtn = new QPushButton("背包");
    backpackBtn->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; padding: 12px 30px; "
                               "font-size: 14px; border-radius: 5px; }"
                               "QPushButton:hover { background-color: #8e44ad; }");
    connect(backpackBtn, &QPushButton::clicked, this, &ShopWidget::onOpenBackpack);
    btnLayout->addWidget(backpackBtn);

    QPushButton *closeBtn = new QPushButton("离开商店");
    closeBtn->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 12px 30px; "
                            "font-size: 14px; border-radius: 5px; }"
                            "QPushButton:hover { background-color: #c0392b; }");
    connect(closeBtn, &QPushButton::clicked, this, &ShopWidget::onCloseShop);
    btnLayout->addWidget(closeBtn);

    mainLayout->addLayout(btnLayout);
}

// 关键：重写 resizeEvent，让背景标签始终填充整个窗口
void ShopWidget::resizeEvent(QResizeEvent *event) {
    if (m_bgLabel) {
        m_bgLabel->setGeometry(rect());
    }
    QWidget::resizeEvent(event);
}

// 以下是你原有的槽函数，保持不变
void ShopWidget::setupShop(Player *player, Inventory *inventory) {
    m_player = player;
    m_inventory = inventory;

    if (m_shopSystem) {
        delete m_shopSystem;
    }
    m_shopSystem = new ShopSystem(player, inventory, this);

    connect(m_shopSystem, &ShopSystem::shopOpened, this, &ShopWidget::onShopOpened);
    connect(m_shopSystem, &ShopSystem::itemBought, this, &ShopWidget::onItemBought);
    connect(m_shopSystem, &ShopSystem::moneyChanged, this, &ShopWidget::onMoneyChanged);
    connect(m_shopSystem, &ShopSystem::message, this, &ShopWidget::onMessage);

    updateStates();
    m_shopSystem->openShop();
}

void ShopWidget::onShopOpened(const QList<Item> &items, int money) {
    m_moneyLabel->setText(QString("金钱: %1").arg(money));

    // 清除旧的物品按钮
    for (auto *btn : m_itemButtons) btn->deleteLater();
    for (auto *lbl : m_itemLabels) lbl->deleteLater();
    m_itemButtons.clear();
    m_itemLabels.clear();

    QGridLayout *grid = qobject_cast<QGridLayout*>(m_itemsContainer->layout());
    while (grid->count() > 0) {
        QLayoutItem *item = grid->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    for (int i = 0; i < items.size(); ++i) {
        const Item &item = items[i];

        QWidget *itemWidget = new QWidget();
        itemWidget->setStyleSheet("background-color: #34495e; border-radius: 8px; padding: 10px;");
        QVBoxLayout *itemLayout = new QVBoxLayout(itemWidget);

        QLabel *nameLabel = new QLabel(item.name);
        nameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ecf0f1;");
        itemLayout->addWidget(nameLabel);

        QLabel *descLabel = new QLabel(item.description);
        descLabel->setStyleSheet("font-size: 12px; color: #bdc3c7;");
        descLabel->setWordWrap(true);
        itemLayout->addWidget(descLabel);

        QLabel *effectLabel = new QLabel(item.effectDesc);
        effectLabel->setStyleSheet("font-size: 11px; color: #2ecc71;");
        itemLayout->addWidget(effectLabel);

        QPushButton *buyBtn = new QPushButton(QString("购买 (%1)").arg(item.price));
        bool canAfford = m_shopSystem->canAfford(i);
        bool inStock = item.buyLimit < 0 || item.remainingStock > 0;

        if (!canAfford || !inStock) {
            buyBtn->setEnabled(false);
            buyBtn->setStyleSheet("QPushButton { background-color: #7f8c8d; color: #bdc3c7; padding: 8px; "
                                  "font-size: 13px; border-radius: 5px; }");
        } else {
            buyBtn->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 8px; "
                                  "font-size: 13px; border-radius: 5px; }"
                                  "QPushButton:hover { background-color: #2ecc71; }");
        }

        if (item.buyLimit > 0 && item.remainingStock <= 0) {
            buyBtn->setText("已售罄");
        }

        connect(buyBtn, &QPushButton::clicked, [this, i]() { onBuyItem(i); });
        itemLayout->addWidget(buyBtn);
        m_itemButtons.append(buyBtn);

        grid->addWidget(itemWidget, i / 3, i % 3);
    }
}

void ShopWidget::onBuyItem(int index) {
    m_shopSystem->buyItem(index);
}

void ShopWidget::onItemBought(int itemIndex, bool success) {
    if (success) {
        updateItemDisplay();
        updateStates();
    }
}

void ShopWidget::updateItemDisplay() {
    m_shopSystem->openShop();
}

void ShopWidget::onMoneyChanged(int newMoney) {
    m_moneyLabel->setText(QString("金钱: %1").arg(newMoney));
}

void ShopWidget::onMessage(QString msg) {
    m_messageLabel->setText(msg);
    static QTimer *timer = nullptr;
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, [this]() {
        m_messageLabel->setText("");
    });
    timer->start(3000);
}

void ShopWidget::onOpenBackpack() {
    emit openBackpack();
}

void ShopWidget::onCloseShop() {
    emit shopClosed();
}

void ShopWidget::updateStates() {
    if (!m_player) return;
    PlayerStats &stats = m_player->stats;
    m_hpLabel->setText(QString("HP: %1/%2").arg(stats.hp).arg(stats.maxHp));
    m_phyLabel->setText(QString("PHY: %1/%2").arg(stats.phy).arg(stats.maxPhy));
    m_dmgLabel->setText(QString("攻击力: %1").arg(stats.getEffectiveDmg()));
    m_wlLabel->setText(QString("武器等级: %1").arg(stats.weaponLevel));
}