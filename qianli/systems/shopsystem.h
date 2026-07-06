#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include "models/item.h"
#include "models/player.h"
#include "inventory.h"
#include <QObject>

class ShopSystem : public QObject 
{
    Q_OBJECT
public:
    explicit ShopSystem(Player *player, Inventory *inventory, QObject *parent = nullptr);

    void openShop();
    bool buyItem(int itemIndex);
    bool useItemInShop(ItemType type);
    bool canAfford(int itemIndex) const;
    int getPlayerMoney() const;
    const QList<Item> &getItems() const { return m_items; }

signals:
    void shopOpened(const QList<Item> &items, int money);
    void itemBought(int itemIndex, bool success);
    void itemUsed(bool success);
    void moneyChanged(int newMoney);
    void message(QString msg);

private:
    Player *m_player;
    Inventory *m_inventory;
    QList<Item> m_items;
};

#endif 
