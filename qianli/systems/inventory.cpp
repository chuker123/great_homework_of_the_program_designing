#include "inventory.h"
#include <QDataStream>

void Inventory::addItem(ItemType type) {
    if (type != ItemType::None && type != ItemType::Forge) {
        m_items[type]++;
    }
}

bool Inventory::useItem(ItemType type, Player &player) {
    if (!hasItem(type)) return false;

    switch (type) {
    case ItemType::Bandage:
        player.healHp(25);
        break;
    case ItemType::MedicinalWine:
        player.stats.maxHp += 15;
        player.healHp(20);
        player.stats.hp = std::min(player.stats.hp, player.stats.maxHp);
        break;
    case ItemType::Food:
        player.restorePhy(25);
        break;
    case ItemType::Ginseng:
        player.stats.maxPhy += 20;
        player.stats.phy = std::min(player.stats.phy, player.stats.maxPhy);
        break;
    case ItemType::Wine:
        player.applyWineBoost(20);
        break;
    default:
        return false;
    }

    m_items[type]--;
    if (m_items[type] <= 0) {
        m_items.remove(type);
    }
    return true;
}

int Inventory::count(ItemType type) const {
    return m_items.value(type, 0);
}

bool Inventory::hasItem(ItemType type) const {
    return m_items.value(type, 0) > 0;
}

QMap<ItemType, int> Inventory::getAllItems() const {
    return m_items;
}

void Inventory::clear() {
    m_items.clear();
}

QDataStream &operator<<(QDataStream &out, const Inventory &inv) {
    out << inv.m_items;
    return out;
}

QDataStream &operator>>(QDataStream &in, Inventory &inv) {
    in >> inv.m_items;
    return in;
}
