#ifndef SHOPWIDGET_H
#define SHOPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include "systems/shopsystem.h"
#include "systems/inventory.h"

class ShopWidget : public QWidget 
{
    Q_OBJECT
public:
    explicit ShopWidget(QWidget *parent = nullptr);
    void setupShop(Player *player, Inventory *inventory);

signals:
    void shopClosed();
    void openBackpack();

private slots:
    void onShopOpened(const QList<Item> &items, int money);
    void onItemBought(int itemIndex, bool success);
    void onMoneyChanged(int newMoney);
    void onMessage(QString msg);
    void onBuyItem(int index);
    void onOpenBackpack();
    void onCloseShop();

private:
    ShopSystem *m_shopSystem = nullptr;
    Player *m_player = nullptr;
    Inventory *m_inventory = nullptr;

    void resizeEvent(QResizeEvent *event) override;

    QLabel *m_hpLabel;
    QLabel *m_phyLabel;
    QLabel *m_dmgLabel;
    QLabel *m_wlLabel;
    QLabel *m_moneyLabel;
    QLabel *m_messageLabel;
    QWidget *m_itemsContainer;
    QList<QPushButton*> m_itemButtons;
    QList<QLabel*> m_itemLabels;
    QLabel *m_bgLabel;

    void setupUI();
    void updateItemDisplay();
    void updateStates();
};

#endif 
