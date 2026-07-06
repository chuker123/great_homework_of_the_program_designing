#include "shopsystem.h"

ShopSystem::ShopSystem(Player *player, Inventory *inventory, QObject *parent)
    : QObject(parent), m_player(player), m_inventory(inventory) {
    m_items = ItemFactory::getShopItems();
}

void ShopSystem::openShop() 
{
    emit shopOpened(m_items, m_player->stats.money);
}

bool ShopSystem::buyItem(int itemIndex) 
{
    if (itemIndex < 0 || itemIndex >= m_items.size()) return false;

    Item &item = m_items[itemIndex];
    if (m_player->stats.money < item.price) 
    {
        emit message("您的余额不足以购买该物品！");
        return false;
    }

    if (item.buyLimit > 0 && item.remainingStock <= 0) 
    {
        emit message("该物品已售罄！");
        return false;
    }

    if (item.type == ItemType::Forge && m_player->stats.weaponLevel >= 5) 
    {
        emit message("武器已达最高等级，无需再锻造！");
        return false;
    }

    if (item.type == ItemType::Forge) 
    {
        m_player->upgradeWeapon();
        emit message(QString("武器等级提升至 %1 级！").arg(m_player->stats.weaponLevel));
    } 
    else 
    {
        m_inventory->addItem(item.type);
        emit message(QString("购买了 %1").arg(item.name));
    }

    m_player->stats.money -= item.price;
    if (item.buyLimit > 0) 
        item.remainingStock--;

    emit moneyChanged(m_player->stats.money);
    emit itemBought(itemIndex, true);
    return true;
}

bool ShopSystem::useItemInShop(ItemType type) 
{
    if (!m_inventory->hasItem(type)) 
    {
        emit message("背包中没有该物品！");
        return false;
    }

    bool success = m_inventory->useItem(type, *m_player);
    if (success) 
    {
        emit message("使用成功！");
        emit moneyChanged(m_player->stats.money);
    }
    emit itemUsed(success);
    return success;
}

bool ShopSystem::canAfford(int itemIndex) const 
{
    if (itemIndex < 0 || itemIndex >= m_items.size()) return false;
    const Item &item = m_items[itemIndex];
    if (item.buyLimit > 0 && item.remainingStock <= 0) return false;
    return m_player->stats.money >= item.price;
}

int ShopSystem::getPlayerMoney() const 
{
    return m_player->stats.money;
}
