#ifndef INVENTORY_H
#define INVENTORY_H

#include "models/item.h"
#include "models/player.h"
#include <QMap>
#include <QDataStream>

class Inventory 
{
public:
    void addItem(ItemType type);
    bool useItem(ItemType type, Player &player);
    int count(ItemType type) const;
    bool hasItem(ItemType type) const;
    QMap<ItemType, int> getAllItems() const;
    void clear();

    friend QDataStream &operator<<(QDataStream &out, const Inventory &inv);
    friend QDataStream &operator>>(QDataStream &in, Inventory &inv);

private:
    QMap<ItemType, int> m_items;
};

#endif 
