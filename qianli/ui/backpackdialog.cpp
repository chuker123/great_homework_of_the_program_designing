#include "backpackdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "models/item.h"

BackpackDialog::BackpackDialog(Player *player, Inventory *inventory, QWidget *parent)
    : QDialog(parent), m_player(player), m_inventory(inventory) {
    setupUI();
    updateItems();
}

void BackpackDialog::setupUI() {
    setWindowTitle("背包");
    setMinimumSize(400, 400);
    setStyleSheet("background-color: #fdf5e6;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 标题
    QLabel *title = new QLabel("背包");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // 滚动区域
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    m_itemsContainer = new QWidget();
    m_itemsContainer->setStyleSheet("background-color: #fdf5e6;");
    scrollArea->setWidget(m_itemsContainer);
    mainLayout->addWidget(scrollArea);

    // 空背包提示
    m_emptyLabel = new QLabel("背包是空的");
    m_emptyLabel->setStyleSheet("font-size: 16px; color: #a0a0a0;");
    m_emptyLabel->setAlignment(Qt::AlignCenter);

    // 关闭按钮
    QPushButton *closeBtn = new QPushButton("关闭");
    closeBtn->setStyleSheet("QPushButton { background-color: #8B4513; color: white; padding: 10px 30px; "
                            "font-size: 14px; border-radius: 5px; }"
                            "QPushButton:hover { background-color: #A0522D; }");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeBtn, 0, Qt::AlignCenter);
}

void BackpackDialog::updateItems() {
    // 清除旧内容
    QLayout *oldLayout = m_itemsContainer->layout();
    if (oldLayout) {
        while (QLayoutItem *item = oldLayout->takeAt(0)) {
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout *layout = new QVBoxLayout(m_itemsContainer);
    layout->setSpacing(10);

    auto items = m_inventory->getAllItems();
    if (items.isEmpty()) {
        layout->addWidget(m_emptyLabel);
        layout->addStretch();
        return;
    }

    for (auto it = items.begin(); it != items.end(); ++it) {
        ItemType type = it.key();
        int count = it.value();

        Item item = ItemFactory::createItem(type);

        QWidget *itemWidget = new QWidget();
        itemWidget->setStyleSheet("background-color: #fff8dc; border: 1px solid #d2b48c; border-radius: 8px; padding: 10px;");
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);

        QVBoxLayout *infoLayout = new QVBoxLayout();
        QLabel *nameLabel = new QLabel(QString("%1 x%2").arg(item.name).arg(count));
        nameLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #5D4037;");
        infoLayout->addWidget(nameLabel);

        QLabel *effectLabel = new QLabel(item.effectDesc);
        effectLabel->setStyleSheet("font-size: 12px; color: #795548;");
        infoLayout->addWidget(effectLabel);

        itemLayout->addLayout(infoLayout);
        itemLayout->addStretch();

        QPushButton *useBtn = new QPushButton("使用");
        useBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 6px 20px; "
                              "font-size: 13px; border-radius: 4px; }"
                              "QPushButton:hover { background-color: #45a049; }");
        connect(useBtn, &QPushButton::clicked, [this, type]() {
            if (m_inventory->useItem(type, *m_player)) {
                emit itemUsed();
                refresh();
            }
        });
        itemLayout->addWidget(useBtn);

        layout->addWidget(itemWidget);
    }

    layout->addStretch();
}

void BackpackDialog::refresh() {
    updateItems();
}
